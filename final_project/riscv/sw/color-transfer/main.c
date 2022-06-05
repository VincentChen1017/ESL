#include <stdbool.h>

#include "assert.h"
#include "math.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

unsigned char header[54] = {
    0x42,           // identity : B
    0x4d,           // identity : M
    0,    0, 0, 0,  // file size
    0,    0,        // reserved1
    0,    0,        // reserved2
    54,   0, 0, 0,  // RGB data offset
    40,   0, 0, 0,  // struct BITMAPINFOHEADER size
    0,    0, 0, 0,  // bmp width
    0,    0, 0, 0,  // bmp height
    1,    0,        // planes
    24,   0,        // bit per pixel
    0,    0, 0, 0,  // compression
    0,    0, 0, 0,  // data size
    0,    0, 0, 0,  // h resolution
    0,    0, 0, 0,  // v resolution
    0,    0, 0, 0,  // used colors
    0,    0, 0, 0   // important colors
};

union word {
	float flt;
	int sint;
	unsigned int uint;
	unsigned char uc[4];
};

unsigned int input_rgb_raw_data_offset;
unsigned int input_rgb_raw_data_offset_ref;
const unsigned int output_rgb_raw_data_offset = 54;
int width;
int width_ref;
int height;
int height_ref;
unsigned int width_bytes;
unsigned int width_bytes_ref;
unsigned char bits_per_pixel;
unsigned char bits_per_pixel_ref;
unsigned short bytes_per_pixel;
unsigned short bytes_per_pixel_ref;
unsigned char* source_bitmap;
unsigned char* source_bitmap_ref;
unsigned char* target_bitmap;
unsigned char* xyz_bitmap;
unsigned char* xyz_bitmap_ref;

// COLOR_TRANSFER ACC
// phase1_R
static char* const COLORTRANSFER_P1R_START_ADDR_0 = (char* const)0x73000000;
static char* const COLORTRANSFER_P1R_READ_ADDR_0 = (char* const)0x73000004;
static char* const COLORTRANSFER_P1R_START_ADDR_1 = (char* const)0x73100000;
static char* const COLORTRANSFER_P1R_READ_ADDR_1 = (char* const)0x73100004;
// phase1_G
static char* const COLORTRANSFER_P1G_START_ADDR_0 = (char* const)0x73200000;
static char* const COLORTRANSFER_P1G_READ_ADDR_0 = (char* const)0x73200004;
static char* const COLORTRANSFER_P1G_START_ADDR_1 = (char* const)0x73300000;
static char* const COLORTRANSFER_P1G_READ_ADDR_1 = (char* const)0x73300004;
// phase1_B
static char* const COLORTRANSFER_P1B_START_ADDR_0 = (char* const)0x73400000;
static char* const COLORTRANSFER_P1B_READ_ADDR_0 = (char* const)0x73400004;
static char* const COLORTRANSFER_P1B_START_ADDR_1 = (char* const)0x73500000;
static char* const COLORTRANSFER_P1B_READ_ADDR_1 = (char* const)0x73500004;

// phase2_R
static char* const COLORTRANSFER_P2R_START_ADDR_0 = (char* const)0x73600000;
static char* const COLORTRANSFER_P2R_READ_ADDR_0 = (char* const)0x73600004;
static char* const COLORTRANSFER_P2R_START_ADDR_1 = (char* const)0x73700000;
static char* const COLORTRANSFER_P2R_READ_ADDR_1 = (char* const)0x73700004;
// phase2_G
static char* const COLORTRANSFER_P2G_START_ADDR_0 = (char* const)0x73800000;
static char* const COLORTRANSFER_P2G_READ_ADDR_0 = (char* const)0x73800004;
static char* const COLORTRANSFER_P2G_START_ADDR_1 = (char* const)0x73900000;
static char* const COLORTRANSFER_P2G_READ_ADDR_1 = (char* const)0x73900004;
// phase2_B
static char* const COLORTRANSFER_P2B_START_ADDR_0 = (char* const)0x73a00000;
static char* const COLORTRANSFER_P2B_READ_ADDR_0 = (char* const)0x73a00004;
static char* const COLORTRANSFER_P2B_START_ADDR_1 = (char* const)0x73b00000;
static char* const COLORTRANSFER_P2B_READ_ADDR_1 = (char* const)0x73b00004;

// DMA
static volatile uint32_t* const DMA_SRC_ADDR = (uint32_t* const)0x70000000;
static volatile uint32_t* const DMA_DST_ADDR = (uint32_t* const)0x70000004;
static volatile uint32_t* const DMA_LEN_ADDR = (uint32_t* const)0x70000008;
static volatile uint32_t* const DMA_OP_ADDR = (uint32_t* const)0x7000000C;
static volatile uint32_t* const DMA_STAT_ADDR = (uint32_t* const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

bool _is_using_dma = false;
int read_bmp(const char* infile_name) {
	FILE* fp_s = NULL;  // source file handler
	fp_s = fopen(infile_name, "rb");
	if (fp_s == NULL) {
		printf("fopen %s error\n", infile_name);
		return -1;
	}
	// move offset to 10 to find rgb raw data offset
	fseek(fp_s, 10, SEEK_SET);
	assert(fread(&input_rgb_raw_data_offset, sizeof(unsigned int), 1, fp_s));

	// move offset to 18 to get width & height;
	fseek(fp_s, 18, SEEK_SET);
	assert(fread(&width, sizeof(unsigned int), 1, fp_s));
	assert(fread(&height, sizeof(unsigned int), 1, fp_s));

	// get bit per pixel
	fseek(fp_s, 28, SEEK_SET);
	assert(fread(&bits_per_pixel, sizeof(unsigned short), 1, fp_s));
	bytes_per_pixel = bits_per_pixel / 8;

	// move offset to input_rgb_raw_data_offset to get RGB raw data
	fseek(fp_s, input_rgb_raw_data_offset, SEEK_SET);

	source_bitmap = (unsigned char*)malloc((size_t)width * height * bytes_per_pixel);
	if (source_bitmap == NULL) {
		printf("malloc images_s error\n");
		return -1;
	}

	xyz_bitmap = (unsigned char*)malloc((size_t)width * height * bytes_per_pixel);
	if (xyz_bitmap == NULL) {
		printf("malloc lab error\n");
		return -1;
	}

	target_bitmap = (unsigned char*)malloc((size_t)width * height * bytes_per_pixel);
	if (target_bitmap == NULL) {
		printf("malloc target_bitmap error\n");
		return -1;
	}

	assert(fread(source_bitmap, sizeof(unsigned char), (size_t)(long)width * height * bytes_per_pixel, fp_s));
	fclose(fp_s);

	unsigned int file_size;  // file size
	// file size
	file_size = width * height * bytes_per_pixel + output_rgb_raw_data_offset;
	header[2] = (unsigned char)(file_size & 0x000000ff);
	header[3] = (file_size >> 8) & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;

	// width
	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) & 0x000000ff;
	header[20] = (width >> 16) & 0x000000ff;
	header[21] = (width >> 24) & 0x000000ff;

	// height
	header[22] = height & 0x000000ff;
	header[23] = (height >> 8) & 0x000000ff;
	header[24] = (height >> 16) & 0x000000ff;
	header[25] = (height >> 24) & 0x000000ff;

	// bit per pixel
	header[28] = bits_per_pixel;

	return 0;
}

int read_bmp_reference(const char* infile_name) {
	FILE* fp_s = NULL;  // source file handler
	fp_s = fopen(infile_name, "rb");
	if (fp_s == NULL) {
		printf("fopen %s error\n", infile_name);
		return -1;
	}
	// move offset to 10 to find rgb raw data offset
	fseek(fp_s, 10, SEEK_SET);
	assert(fread(&input_rgb_raw_data_offset_ref, sizeof(unsigned int), 1, fp_s));
	// move offset to 18 to get width & height;
	fseek(fp_s, 18, SEEK_SET);
	assert(fread(&width_ref, sizeof(unsigned int), 1, fp_s));
	assert(fread(&height_ref, sizeof(unsigned int), 1, fp_s));

	bytes_per_pixel_ref = bytes_per_pixel;

	// move offset to input_rgb_raw_data_offset to get RGB raw data
	fseek(fp_s, input_rgb_raw_data_offset_ref, SEEK_SET);

	source_bitmap_ref = (unsigned char*)malloc((size_t)width_ref * height_ref * bytes_per_pixel_ref);
	if (source_bitmap_ref == NULL) {
		printf("malloc images_s_ref error\n");
		return -1;
	}

	xyz_bitmap_ref = (unsigned char*)malloc((size_t)width_ref * height_ref * bytes_per_pixel_ref);
	if (xyz_bitmap_ref == NULL) {
		printf("malloc lab_ref error\n");
		return -1;
	}

	assert(fread(source_bitmap_ref, sizeof(unsigned char), (size_t)(long)width_ref * height_ref * bytes_per_pixel_ref,
	             fp_s));
	fclose(fp_s);

	return 0;
}

int write_bmp(const char* outfile_name) {
	FILE* fp_t = NULL;  // target file handler

	fp_t = fopen(outfile_name, "wb");
	if (fp_t == NULL) {
		printf("fopen %s error\n", outfile_name);
		return -1;
	}

	// write header
	fwrite(header, sizeof(unsigned char), output_rgb_raw_data_offset, fp_t);

	// write image
	fwrite(target_bitmap, sizeof(unsigned char), (size_t)(long)width * height * bytes_per_pixel, fp_t);

	fclose(fp_t);
	return 0;
}

int write_bmp_xyz(const char* outfile_name) {
	FILE* fp_t = NULL;   // target file handler
	FILE* fp_t2 = NULL;  // target file handler

	fp_t = fopen(outfile_name, "wb");
	if (fp_t == NULL) {
		printf("fopen %s error\n", outfile_name);
		return -1;
	}

	// write header
	fwrite(header, sizeof(unsigned char), output_rgb_raw_data_offset, fp_t);
	// write image
	fwrite(xyz_bitmap, sizeof(unsigned char), (size_t)(long)width * height * bytes_per_pixel, fp_t);
	fclose(fp_t);
	return 0;
}

// semaphore and barrier
int sem_init(uint32_t* __sem, uint32_t count) __THROW {
	*__sem = count;
	return 0;
}

int sem_wait(uint32_t* __sem) __THROW {
	uint32_t value, success;  // RV32A
	__asm__ __volatile__(
	    "\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     beqz %[value],L%=                   # if zero, try again\n\t\
     addi %[value],%[value],-1           # value --\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
	    : [value] "=r"(value), [success] "=r"(success)
	    : [__sem] "r"(__sem)
	    : "memory");
	return 0;
}

int sem_post(uint32_t* __sem) __THROW {
	uint32_t value, success;  // RV32A
	__asm__ __volatile__(
	    "\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     addi %[value],%[value], 1           # value ++\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
	    : [value] "=r"(value), [success] "=r"(success)
	    : [__sem] "r"(__sem)
	    : "memory");
	return 0;
}

int barrier(uint32_t* __sem, uint32_t* __lock, uint32_t* counter, uint32_t thread_count) {
	sem_wait(__lock);
	if (*counter == thread_count - 1) {  // all finished
		*counter = 0;
		sem_post(__lock);
		for (int j = 0; j < thread_count - 1; ++j) sem_post(__sem);
	} else {
		(*counter)++;
		sem_post(__lock);
		sem_wait(__sem);
	}
	return 0;
}

// dma lock for dma access
uint32_t dma_lock;

void write_data_to_ACC(char* ADDR, unsigned char* buffer, int len) {
	if (_is_using_dma) {
		// Using DMA
		sem_wait(&dma_lock);
		*DMA_SRC_ADDR = (uint32_t)(buffer);
		*DMA_DST_ADDR = (uint32_t)(ADDR);
		*DMA_LEN_ADDR = len;
		*DMA_OP_ADDR = DMA_OP_MEMCPY;
		sem_post(&dma_lock);
	} else {
		// Directly Send
		memcpy(ADDR, buffer, sizeof(unsigned char) * len);
	}
}

void read_data_from_ACC(char* ADDR, unsigned char* buffer, int len) {
	if (_is_using_dma) {
		// Using DMA
		sem_wait(&dma_lock);
		*DMA_SRC_ADDR = (uint32_t)(ADDR);
		*DMA_DST_ADDR = (uint32_t)(buffer);
		*DMA_LEN_ADDR = len;
		*DMA_OP_ADDR = DMA_OP_MEMCPY;
		sem_post(&dma_lock);
	} else {
		// Directly Read
		memcpy(buffer, ADDR, sizeof(unsigned char) * len);
	}
}

// Total number of cores
// static const int PROCESSORS = 3;
#define PROCESSORS 3  // 3 processors to do the 3 channel operations
// the barrier synchronization objects
uint32_t barrier_counter = 0;
uint32_t barrier_lock;
uint32_t barrier_sem;
// the mutex object to control "global" related variable
uint32_t lock;
// print synchronication semaphore (print in core order)
uint32_t print_sem[PROCESSORS];

unsigned int mean_l_src;
unsigned int mean_l_ref;
unsigned int std_l_src = 0;
unsigned int std_l_ref = 0;
unsigned int mean_a_src;
unsigned int mean_a_ref;
unsigned int std_a_src = 0;
unsigned int std_a_ref = 0;
unsigned int mean_b_src;
unsigned int mean_b_ref;
unsigned int std_b_src = 0;
unsigned int std_b_ref = 0;

union word data_1;
union word data_2;
union word data_3;
unsigned char w_buffer[4] = {0};
unsigned char w_buffer_ref[4] = {0};
int main(unsigned int hart_id) {
	///////////////////////////////////////////////////////////////
	// thread、barrier init and read the bmp. file in the core 0 //
	//////////////////////////////////////////////////////////////
	if (hart_id == 0) {
		read_bmp("lena.bmp");  // read the color transfer source .bmp file
		printf("======================================\n");
		printf("\t  Reading from array\n");
		printf("======================================\n");
		printf(" input_rgb_raw_data_offset\t= %d\n", input_rgb_raw_data_offset);
		printf(" width\t\t\t\t= %d\n", width);
		printf(" height\t\t\t\t= %d\n", height);
		printf(" bytes_per_pixel\t\t= %d\n", bytes_per_pixel);
		printf("======================================\n");
		/*
		read_bmp_reference("r120.bmp");  // read the color transfer reference .bmp file
		printf("======================================\n");
		printf("\t  Reading from array\n");
		printf("======================================\n");
		printf(" input_rgb_raw_data_offset_ref\t= %d\n", input_rgb_raw_data_offset_ref);
		printf(" width_ref\t\t\t= %d\n", width_ref);
		printf(" height_ref\t\t\t= %d\n", height_ref);
		printf(" bytes_per_pixel_ref\t\t= %d\n", bytes_per_pixel_ref);
		printf("======================================\n");
		*/
		// create a barrier object with a count of PROCESSORS
		sem_init(&barrier_lock, 1);
		sem_init(&dma_lock, 1);
		sem_init(&barrier_sem, 0);  // lock all cores initially
		for (int i = 0; i < PROCESSORS; ++i) {
			sem_init(&print_sem[i], 0);  // lock printing initially
		}
		// Create mutex lock
		sem_init(&lock, 1);
	}

	////////////////////////////
	// barrier to synchronize //
	////////////////////////////
	// Wait for all threads to finish
	barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);

	union word data;
	union word data_ref;
	unsigned int sum_src = 0;
	unsigned int sum_ref = 0;
	unsigned char r_buffer[4] = {0};
	unsigned char r_buffer_ref[4] = {0};

	// version 1: assume the source and the reference .bmp file have the same width and height,
	// then they process by the 3 PEs at the same time
	// for each computation, only pass 1 pixel's data to calculate
	/*if (hart_id == 0) {
	    printf("Core[%d] Start Processing Phase-1...(%d, %d)\n", hart_id, width, height);
	    sem_post(&print_sem[1]);  // Allow Core 1 to print
	} else if (hart_id == 1) {
	    sem_wait(&print_sem[1]);
	    printf("Core[%d] Start Processing Phase-1...(%d, %d)\n", hart_id, width, height);
	    sem_post(&print_sem[2]);  // Allow Core 2 to print
	} else if (hart_id == 2) {
	    sem_wait(&print_sem[2]);
	    printf("Core[%d] Start Processing Phase-1...(%d, %d)\n", hart_id, width, height);
	    sem_post(&print_sem[0]);
	} */

	for (int y = 0; y < height; y++) {
		/*if (hart_id == 0) {
		    printf("y: %d\n", y);
		}*/
		/*sem_wait(&lock);
		printf("Core: %d\n", hart_id);
		printf("sum_src: %d\n", sum_src);
		printf("sum_ref: %d\n", sum_ref);
		sem_post(&lock);*/

		for (int x = 0; x < width; x++) {
			////////////////////////////
			// barrier to synchronize //
			////////////////////////////
			// Wait for all threads to finish
			barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);

			// L/L
			if (hart_id == 0) {
				w_buffer[0] = *(source_bitmap + bytes_per_pixel * (y * width + x) + 2);  // R
				// w_buffer_ref[0] = *(source_bitmap_ref + bytes_per_pixel_ref * (y * width_ref + x) + 2);  // R
				//  sem_post(&print_sem[1]);  // Allow Core 1 to print
				sum_src += w_buffer[0];
				// sum_ref += w_buffer_ref[0];
			}
			// M/A
			else if (hart_id == 1) {
				// sem_wait(&print_sem[1]);
				w_buffer[1] = *(source_bitmap + bytes_per_pixel * (y * width + x) + 1);  // G
				// w_buffer_ref[1] = *(source_bitmap_ref + bytes_per_pixel_ref * (y * width_ref + x) + 1);  // G
				//  sem_post(&print_sem[2]);  // Allow Core 2 to print
				sum_src += w_buffer[1];
				// sum_ref += w_buffer_ref[1];
			}
			// S/B
			else if (hart_id == 2) {
				// sem_wait(&print_sem[2]);
				w_buffer[2] = *(source_bitmap + bytes_per_pixel * (y * width + x) + 0);  // B
				// w_buffer_ref[2] = *(source_bitmap_ref + bytes_per_pixel_ref * (y * width_ref + x) + 0);  // B
				sum_src += w_buffer[2];
				// sum_ref += w_buffer_ref[2];
			}

			////////////////////////////
			// barrier to synchronize //
			////////////////////////////
			// Wait for all threads to finish
			barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);

			// L/L
			if (hart_id == 0) {
				// sem_wait(&print_sem[0]);
				// printf("w_buffer[0]: %d\n", w_buffer[0]);
				// printf("w_buffer[1]: %d\n", w_buffer[1]);
				// printf("w_buffer[2]: %d\n", w_buffer[2]);
				// printf("w_buffer_ref[0]: %d\n", w_buffer_ref[0]);
				// printf("w_buffer_ref[1]: %d\n", w_buffer_ref[1]);
				// printf("w_buffer_ref[2]: %d\n", w_buffer_ref[2]);
				write_data_to_ACC(COLORTRANSFER_P1R_START_ADDR_0, w_buffer, 4);
				read_data_from_ACC(COLORTRANSFER_P1R_READ_ADDR_0, r_buffer, 4);
				// printf("r_buffer[0]: %d\n", r_buffer[0]);
				// write_data_to_ACC(COLORTRANSFER_P1R_START_ADDR_0, w_buffer_ref, 4);
				// read_data_from_ACC(COLORTRANSFER_P1R_READ_ADDR_0, r_buffer_ref, 4);
				// printf("r_buffer_ref[0]: %d\n", r_buffer_ref[0]);
				//    sem_post(&print_sem[1]);
			}
			// M/A
			else if (hart_id == 1) {
				// sem_wait(&print_sem[1]);
				//  printf("w_buffer[0]: %d\n", w_buffer[0]);
				//  printf("w_buffer[1]: %d\n", w_buffer[1]);
				//  printf("w_buffer[2]: %d\n", w_buffer[2]);
				//  printf("w_buffer[3]: %d\n", w_buffer[3]);
				//  printf("w_buffer[4]: %d\n", w_buffer[4]);
				//  printf("w_buffer[5]: %d\n", w_buffer[5]);
				write_data_to_ACC(COLORTRANSFER_P1G_START_ADDR_0, w_buffer, 4);
				read_data_from_ACC(COLORTRANSFER_P1G_READ_ADDR_0, r_buffer, 4);
				// write_data_to_ACC(COLORTRANSFER_P1G_START_ADDR_0, w_buffer_ref, 4);
				// read_data_from_ACC(COLORTRANSFER_P1G_READ_ADDR_0, r_buffer_ref, 4);
				//  //printf("r_buffer[0]: %d\n", r_buffer[0]);
				//  printf("r_buffer[1]: %d\n", r_buffer[1]);
				//  printf("r_buffer[2]: %d\n", r_buffer[2]);
				//  printf("r_buffer[3]: %d\n", r_buffer[3]);
				//  sem_post(&print_sem[2]);
			}
			// S/B
			else if (hart_id == 2) {
				// printf("w_buffer[0]: %d\n", w_buffer[0]);
				// printf("w_buffer[1]: %d\n", w_buffer[1]);
				// printf("w_buffer[2]: %d\n", w_buffer[2]);
				// printf("w_buffer[3]: %d\n", w_buffer[3]);
				// printf("w_buffer[4]: %d\n", w_buffer[4]);
				// printf("w_buffer[5]: %d\n", w_buffer[5]);
				write_data_to_ACC(COLORTRANSFER_P1B_START_ADDR_0, w_buffer, 4);
				read_data_from_ACC(COLORTRANSFER_P1B_READ_ADDR_0, r_buffer, 4);
				// write_data_to_ACC(COLORTRANSFER_P1B_START_ADDR_0, w_buffer_ref, 4);
				// read_data_from_ACC(COLORTRANSFER_P1B_READ_ADDR_0, r_buffer_ref, 4);
				//  sem_wait(&print_sem[2]);
				//   printf("r_buffer[0]: %d\n", r_buffer[0]);
				//   printf("r_buffer[1]: %d\n", r_buffer[1]);
				//   printf("r_buffer[2]: %d\n", r_buffer[2]);
				//   printf("r_buffer[3]: %d\n", r_buffer[3]);
			}

			////////////////////////////
			// barrier to synchronize //
			////////////////////////////
			// Wait for all threads to finish
			barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);

			if (hart_id == 0) {
				*(xyz_bitmap + bytes_per_pixel * (width * y + x) + 2) = r_buffer[0];
				//*(xyz_bitmap_ref + bytes_per_pixel_ref * (width_ref * y + x) + 2) = r_buffer_ref[0];
			} else if (hart_id == 1) {
				*(xyz_bitmap + bytes_per_pixel * (width * y + x) + 1) = r_buffer[0];
				//*(xyz_bitmap_ref + bytes_per_pixel_ref * (width_ref * y + x) + 1) = r_buffer_ref[0];

			} else if (hart_id == 2) {
				*(xyz_bitmap + bytes_per_pixel * (width * y + x) + 0) = r_buffer[0];
				//*(xyz_bitmap_ref + bytes_per_pixel_ref * (width_ref * y + x) + 0) = r_buffer_ref[0];
			}

			/*
			            sem_wait(&lock);
			            printf("data.flt: %f\n", data.flt);
			            printf("data_ref.flt: %f\n", data_ref.flt);
			            sem_post(&lock);
			            */

			/*sem_wait(&lock);
			if (sum_src == -INFINITY || sum_src == NAN) {
			    printf("w_buffer[0]: %d\n", w_buffer[0]);
			    printf("w_buffer[1]: %d\n", w_buffer[1]);
			    printf("w_buffer[2]: %d\n", w_buffer[2]);
			    printf("data.flt: %f, data_ref.flt: %f\n", data.flt, data_ref.flt);
			    printf("sum_src: %f, sum_ref: %f\n", sum_src, sum_ref);
			}
			sem_post(&lock);*/
		}
	}

	////////////////////////////
	// barrier to synchronize //
	////////////////////////////
	// Wait for all threads to finish
	barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);

	if (hart_id == 0)
		write_bmp_xyz("xyzSpace_out.bmp");
	return 0;
}
