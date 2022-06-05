#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "ColorTransform_x.h"

XColorTransform::XColorTransform( sc_module_name n ): sc_module( n )
{
	SC_THREAD( transform_rgb_to_x );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_rgb.clk_rst(i_clk, i_rst);
  o_x.clk_rst(i_clk, i_rst);
#endif
}

void XColorTransform::transform_rgb_to_x() {
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		o_x.reset();
#endif
		wait();
	}
	while (true) {
#ifndef NATIVE_SYSTEMC
		sc_dt::sc_uint<24> rgb = i_rgb.get();
#else
		sc_dt::sc_uint<24> rgb = i_rgb.read();
#endif
		unsigned char x = (rgb.range(7,0)*412453 + rgb.range(15,8)*357580 + rgb.range(23, 16)*180423)/1000000;
		wait();
#ifndef NATIVE_SYSTEMC
		o_x.put(x);

#else
		o_x.write(x);
#endif
	}
}
