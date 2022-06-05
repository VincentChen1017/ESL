#ifndef COLOR_TRANSFER_P1B_1_H_
#define COLOR_TRANSFER_P1B_1_H_
#include <cmath>
#include <iomanip>
#include <systemc>
using namespace sc_core;

#include <tlm_utils/simple_target_socket.h>

#include <tlm>

#include "transfer_def.h"

struct COLORTRANSFER_P1B_1 : public sc_module {
	tlm_utils::simple_target_socket<COLORTRANSFER_P1B_1> tsock;

	sc_fifo<unsigned char> i_r;
	sc_fifo<unsigned char> i_g;
	sc_fifo<unsigned char> i_b;
	sc_fifo<unsigned char> i_r_ref;
	sc_fifo<unsigned char> i_g_ref;
	sc_fifo<unsigned char> i_b_ref;
	sc_fifo<float> ob_result;

	SC_HAS_PROCESS(COLORTRANSFER_P1B_1);

	COLORTRANSFER_P1B_1(sc_module_name n) : sc_module(n), tsock("t_skt"), base_offset(0) {
		tsock.register_b_transport(this, &COLORTRANSFER_P1B_1::blocking_transport);
		SC_THREAD(do_transformation);
	}

	~COLORTRANSFER_P1B_1() {}

	float b;
	unsigned char r, g, x;
	unsigned int base_offset;

	void do_transformation() {
		{ wait(CLOCK_PERIOD, SC_NS); }
		while (true) {
			b = 0;
			wait(CLOCK_PERIOD, SC_NS);

			if (i_r.read() == '\0')
				r = 0;
			else
				r = i_r.read();
			if (i_g.read() == '\0')
				g = 0;
			else
				g = i_g.read();
			if (i_b.read() == '\0')
				x = 0;
			else
				x = i_b.read();

			b = (sqrt(2) / 2) * log10(r) + (-sqrt(2) / 2) * log10(g) + (0) * log10(x);
			wait(CLOCK_PERIOD, SC_NS);

			ob_result.write(b);
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
					case COLORTRANSFER_P1B_1_RESULT_ADDR:
						buffer.flt = ob_result.read();
						break;
					default:
						std::cerr << "READ Error! COLORTRANSFER_P1B_1::blocking_transport: address 0x"
						          << std::setfill('0') << std::setw(8) << std::hex << addr << std::dec
						          << " is not valid" << std::endl;
				}
				data_ptr[0] = buffer.uc[0];
				data_ptr[1] = buffer.uc[1];
				data_ptr[2] = buffer.uc[2];
				data_ptr[3] = buffer.uc[3];
				break;
			case tlm::TLM_WRITE_COMMAND:
				// cout << "WRITE" << endl;
				switch (addr) {
					case COLORTRANSFER_P1B_1_R_ADDR:
						i_r.write(data_ptr[0]);
						i_g.write(data_ptr[1]);
						i_b.write(data_ptr[2]);
						break;
					default:
						std::cerr << "WRITE Error! COLORTRANSFER_P1B_1::blocking_transport: address 0x"
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
