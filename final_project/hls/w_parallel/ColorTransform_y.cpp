#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "ColorTransform_y.h"

YColorTransform::YColorTransform( sc_module_name n ): sc_module( n )
{
	SC_THREAD( transform_rgb_to_y );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_rgb.clk_rst(i_clk, i_rst);

  o_y.clk_rst(i_clk, i_rst);

#endif
}

void YColorTransform::transform_rgb_to_y() {
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();

		o_y.reset();

#endif
		wait();
	}
	while (true) {
#ifndef NATIVE_SYSTEMC
		sc_dt::sc_uint<24> rgb = i_rgb.get();
#else
		sc_dt::sc_uint<24> rgb = i_rgb.read();
#endif

		unsigned char y = (rgb.range(7,0)*212671 + rgb.range(15,8)*715160 + rgb.range(23, 16)*72169)/1000000;

		wait();
#ifndef NATIVE_SYSTEMC

		o_y.put(y);

#else

		o_y.write(y);


#endif
	}
}
