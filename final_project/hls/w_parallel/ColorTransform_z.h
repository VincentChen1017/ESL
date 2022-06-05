#ifndef COLOR_TRANSFORM_H_
#define COLOR_TRANSFORM_H_
#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class ZColorTransform: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<24> >::in i_rgb;

	cynw_p2p< sc_dt::sc_uint<8> >::out o_z;
#else
	sc_fifo_in< sc_dt::sc_uint<24> > i_rgb;

	sc_fifo_out< sc_dt::sc_uint<8> > o_z;
#endif

	SC_HAS_PROCESS( ZColorTransform );
	ZColorTransform( sc_module_name n );
private:
	void transform_rgb_to_z();
};
#endif
