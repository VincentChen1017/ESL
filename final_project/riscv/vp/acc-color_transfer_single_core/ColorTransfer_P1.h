#ifndef COLOR_TRANSFER_P1_H_
#define COLOR_TRANSFER_P1_H_
#include <cmath>
#include <iomanip>
#include <systemc>
using namespace sc_core;

#include <tlm_utils/simple_target_socket.h>

#include <tlm>

#include "transfer_def.h"

struct COLORTRANSFER_P1 : public sc_module {
	tlm_utils::simple_target_socket<COLORTRANSFER_P1> tsock;

	sc_fifo<unsigned char> i_r;
	sc_fifo<unsigned char> i_g;
	sc_fifo<unsigned char> i_b;
	sc_fifo<float> o_result;

	SC_HAS_PROCESS(COLORTRANSFER_P1);

	COLORTRANSFER_P1(sc_module_name n) : sc_module(n), tsock("t_skt"), base_offset(0) {
		tsock.register_b_transport(this, &COLORTRANSFER_P1::blocking_transport);
		SC_THREAD(do_transformation);
	}

	~COLORTRANSFER_P1() {}

	float x;
	unsigned int base_offset;

	void do_transformation() {
		{ wait(CLOCK_PERIOD, SC_NS); }
		while (true) {
			for (int i = 0; i < 3; i++) {
				x = 0;
				wait(CLOCK_PERIOD, SC_NS);
				if (i == 0) {
					x = 0.412453 * i_r.read() + 0.357580 * i_g.read() + 0.180423 * i_b.read();
				} else if (i == 1) {
					x = 0.212671 * i_r.read() + 0.715160 * i_g.read() + 0.072169 * i_b.read();
				} else if (i == 2) {
					x = 0.019334 * i_r.read() + 0.119193 * i_g.read() + 0.950227 * i_b.read();
				}
				wait(CLOCK_PERIOD, SC_NS);
				o_result.write(x);
			}
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
		word buffer;
		switch (cmd) {
			case tlm::TLM_READ_COMMAND:
				// cout << "READ" << endl;
				switch (addr) {
					case COLORTRANSFER_P1_RESULT_ADDR:
						buffer.sint = o_result.read();
						break;
					default:
						std::cerr << "READ Error! COLORTRANSFER_P1::blocking_transport: address 0x" << std::setfill('0')
						          << std::setw(8) << std::hex << addr << std::dec << " is not valid" << std::endl;
				}
				data_ptr[0] = buffer.uc[0];
				data_ptr[1] = 0;
				data_ptr[2] = 0;
				data_ptr[3] = 0;
				break;
			case tlm::TLM_WRITE_COMMAND:
				// cout << "WRITE" << endl;
				switch (addr) {
					case COLORTRANSFER_P1_ADDR:
						i_r.write(data_ptr[0]);
						i_g.write(data_ptr[1]);
						i_b.write(data_ptr[2]);
						break;
					default:
						std::cerr << "WRITE Error! COLORTRANSFER_P1::blocking_transport: address 0x"
						          << std::setfill('0') << std::setw(8) << std::hex << addr << std::dec
						          << " is not valid" << std::endl;
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
