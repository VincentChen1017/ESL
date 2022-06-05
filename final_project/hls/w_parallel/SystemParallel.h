#ifndef SYSTEM_PARALLEL_H_
#define SYSTEM_PARALLEL_H_
#include <systemc>

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "ColorTransform_x_wrap.h"
#include "ColorTransform_y_wrap.h"
#include "ColorTransform_z_wrap.h"
#else
#include "ColorTransform_x.h"
#include "ColorTransform_y.h"
#include "ColorTransform_z.h"
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
	XColorTransform_wrapper color_transform_x;
	YColorTransform_wrapper color_transform_y;
	ZColorTransform_wrapper color_transform_z;
#else
	XColorTransform color_transform_x;
	YColorTransform color_transform_y;
	ZColorTransform color_transform_z;
#endif
	sc_clock clk;
	sc_signal<bool> rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<24> > rgbx;
	cynw_p2p< sc_dt::sc_uint<24> > rgby;
	cynw_p2p< sc_dt::sc_uint<24> > rgbz;
	cynw_p2p< sc_dt::sc_uint<8> > x;
	cynw_p2p< sc_dt::sc_uint<8> > y;
	cynw_p2p< sc_dt::sc_uint<8> > z;
#else
	sc_fifo< sc_dt::sc_uint<24> > rgbx;
	sc_fifo< sc_dt::sc_uint<24> > rgby;
	sc_fifo< sc_dt::sc_uint<24> > rgbz;
	sc_fifo< sc_dt::sc_uint<8> > x;
	sc_fifo< sc_dt::sc_uint<8> > y;
	sc_fifo< sc_dt::sc_uint<8> > z;
#endif

	std::string _output_bmp;
};
#endif
