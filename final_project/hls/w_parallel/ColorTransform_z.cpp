#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "ColorTransform_z.h"

ZColorTransform::ZColorTransform( sc_module_name n ): sc_module( n )
{
	SC_THREAD( transform_rgb_to_z );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_rgb.clk_rst(i_clk, i_rst);

  o_z.clk_rst(i_clk, i_rst);
#endif
}

void ZColorTransform::transform_rgb_to_z() {
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();

		o_z.reset();
#endif
		wait();
	}
	while (true) {
#ifndef NATIVE_SYSTEMC
		sc_dt::sc_uint<24> rgb = i_rgb.get();
#else
		sc_dt::sc_uint<24> rgb = i_rgb.read();
#endif

		unsigned char z = (rgb.range(7,0)*19334 + rgb.range(15,8)*119193 + rgb.range(23, 16)*950227)/1000000;
		wait();
#ifndef NATIVE_SYSTEMC

		o_z.put(z);
#else

		o_z.write(z);

#endif
	}
}
