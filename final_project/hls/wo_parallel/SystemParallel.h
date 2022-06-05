#ifndef SYSTEM_PARALLEL_H_
#define SYSTEM_PARALLEL_H_
#include <systemc>

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "ColorTransform_wrap.h"
#else
#include "ColorTransform.h"
#endif

using namespace sc_core;
class SystemParallel: public sc_module
{
public:
	SC_HAS_PROCESS( SystemParallel );
	SystemParallel( sc_module_name n, std::string input_bmp, std::string output_bmp );
	~SystemParallel();
private:
  Testbench tb;
#ifndef NATIVE_SYSTEMC
	ColorTransform_wrapper color_transform;
#else
	ColorTransform color_transform;
#endif
	sc_clock clk;
	sc_signal<bool> rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<2> > select;
	cynw_p2p< sc_dt::sc_uint<24> > rgb;
	cynw_p2p< sc_dt::sc_uint<8> > data;

#else
	sc_fifo< sc_dt::sc_uint<2> > select;
	sc_fifo< sc_dt::sc_uint<24> > rgb;
	sc_fifo< sc_dt::sc_uint<8> > data;


#endif

	std::string _output_bmp;
};
#endif
