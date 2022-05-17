#ifndef GAUSS_FILTER_H_
#define GAUSS_FILTER_H_
#include <cmath>
#include <iomanip>
#include <systemc>
using namespace sc_core;

#include <tlm_utils/simple_target_socket.h>

#include <tlm>

#include "filter_def.h"

struct GaussBlurFilter : public sc_module {
	tlm_utils::simple_target_socket<GaussBlurFilter> tsock;

	sc_fifo<unsigned char> i_r;
	sc_fifo<unsigned char> i_g;
	sc_fifo<unsigned char> i_b;
	sc_fifo<double> or_result;
	sc_fifo<double> og_result;
	sc_fifo<double> ob_result;

	SC_HAS_PROCESS(GaussBlurFilter);

	GaussBlurFilter(sc_module_name n) : sc_module(n), tsock("t_skt"), base_offset(0) {
		tsock.register_b_transport(this, &GaussBlurFilter::blocking_transport);
		SC_THREAD(do_filter);
	}

	~GaussBlurFilter() {}

	double r_val;
	double g_val;
	double b_val;
	unsigned int base_offset;

	void do_filter() {
		{ wait(CLOCK_PERIOD, SC_NS); }
		while (true) {
			r_val = 0;
			g_val = 0;
			b_val = 0;
			wait(CLOCK_PERIOD, SC_NS);
			for (unsigned int v = 0; v < MASK_Y; ++v) {
				for (unsigned int u = 0; u < MASK_X; ++u) {
					r_val += i_r.read() * mask[u][v];
					g_val += i_g.read() * mask[u][v];
					b_val += i_b.read() * mask[u][v];
					wait(CLOCK_PERIOD, SC_NS);
				}
			}
			or_result.write(r_val);
			og_result.write(g_val);
			ob_result.write(b_val);
		}
	}

	void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
		wait(delay);
		// unsigned char *mask_ptr = payload.get_byte_enable_ptr();
		// auto len = payload.get_data_length();
		tlm::tlm_command cmd = payload.get_command();
		sc_dt::uint64 addr = payload.get_address();
		unsigned char *data_ptr = payload.get_data_ptr();

		addr -= base_offset;

		// cout << (int)data_ptr[0] << endl;
		// cout << (int)data_ptr[1] << endl;
		// cout << (int)data_ptr[2] << endl;
		word buffer1, buffer2, buffer3;

		switch (cmd) {
			case tlm::TLM_READ_COMMAND:
				// cout << "READ" << endl;
				switch (addr) {
					case GAUSS_FILTER_RESULT_ADDR:
						buffer1.uint = or_result.read();
						buffer2.uint = og_result.read();
						buffer3.uint = ob_result.read();
						break;
					default:
						std::cerr << "READ Error! GaussBlurFilter::blocking_transport: address 0x" << std::setfill('0')
						          << std::setw(8) << std::hex << addr << std::dec << " is not valid" << std::endl;
				}
				data_ptr[0] = buffer1.uc[0];
				data_ptr[1] = buffer2.uc[0];
				data_ptr[2] = buffer3.uc[0];
				data_ptr[3] = 0;
				break;
			case tlm::TLM_WRITE_COMMAND:
				// cout << "WRITE" << endl;
				switch (addr) {
					case GAUSS_FILTER_R_ADDR:
						i_r.write(data_ptr[0]);
						i_g.write(data_ptr[1]);
						i_b.write(data_ptr[2]);
						break;
					default:
						std::cerr << "WRITE Error! GaussBlurFilter::blocking_transport: address 0x" << std::setfill('0')
						          << std::setw(8) << std::hex << addr << std::dec << " is not valid" << std::endl;
				}
				break;
			case tlm::TLM_IGNORE_COMMAND:
				payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
				return;
			default:
				payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
				return;
		}
		payload.set_response_status(tlm::TLM_OK_RESPONSE);  // Always OK
	}
};
#endif
