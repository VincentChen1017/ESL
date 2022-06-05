all : main.c 
	riscv32-unknown-elf-gcc main.c  -o main -lm  -march=rv32ima -mabi=ilp32
	
sim: all
	$(EE6470)/riscv-vp/vp/build/bin/riscv-vp-acc-color_transfer_single_core --intercept-syscalls main

	
clean:
	rm -f main
