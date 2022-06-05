#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "ColorTransform.h"

ColorTransform::ColorTransform( sc_module_name n ): sc_module( n )
{
	SC_THREAD( transform_rgb_to_xyz );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_rgb.clk_rst(i_clk, i_rst);
	rgb_select.clk_rst(i_clk, i_rst);
  o.clk_rst(i_clk, i_rst);

#endif
}

void ColorTransform::transform_rgb_to_xyz() {
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		rgb_select.reset();
		o.reset();


#endif
		wait();
	}
	while (true) {

#ifndef NATIVE_SYSTEMC
		sc_dt::sc_uint<24> rgb = i_rgb.get();
		sc_dt::sc_uint<2> select = rgb_select.get();
#else
		sc_dt::sc_uint<24> rgb = i_rgb.read();
		sc_dt::sc_uint<2> select = rgb_select.read();
#endif
		if(select==0)
			data = (rgb.range(7,0)*412453 + rgb.range(15,8)*357580 + rgb.range(23, 16)*180423)/1000000;
		else if(select==1)
			data = (rgb.range(7,0)*212671 + rgb.range(15,8)*715160 + rgb.range(23, 16)*72169)/1000000;
		else
			data = (rgb.range(7,0)*19334 + rgb.range(15,8)*119193 + rgb.range(23, 16)*950227)/1000000;
		wait();
#ifndef NATIVE_SYSTEMC
		o.put(data);


#else
		o.write(data);



#endif
		
	}
}
