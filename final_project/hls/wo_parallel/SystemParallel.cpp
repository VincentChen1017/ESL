#include "SystemParallel.h"
SystemParallel::SystemParallel( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ),
	tb("tb"), color_transform("color_transform"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	color_transform.i_clk(clk);
	color_transform.i_rst(rst);
	tb.o_rgb(rgb);
	color_transform.i_rgb(rgb);
	color_transform.o(data);

	tb.i_result(data);

	tb.read_bmp(input_bmp);
	tb.select(select);
	color_transform.rgb_select(select);
}	

SystemParallel::~SystemParallel() {
  tb.write_bmp(_output_bmp);
}
