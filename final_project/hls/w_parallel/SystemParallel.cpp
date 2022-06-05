#include "SystemParallel.h"
SystemParallel::SystemParallel( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ),
	tb("tb"), color_transform_x("color_transform_x"),color_transform_y("color_transform_y"),color_transform_z("color_transform_z"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	color_transform_x.i_clk(clk);
	color_transform_x.i_rst(rst);
	color_transform_y.i_clk(clk);
	color_transform_y.i_rst(rst);
	color_transform_z.i_clk(clk);
	color_transform_z.i_rst(rst);
	tb.o_rgbx(rgbx);
	tb.o_rgby(rgby);
	tb.o_rgbz(rgbz);
	color_transform_x.i_rgb(rgbx);
	color_transform_y.i_rgb(rgby);
	color_transform_z.i_rgb(rgbz);
	color_transform_x.o_x(x);
	color_transform_y.o_y(y);
	color_transform_z.o_z(z);
	tb.ir_result(x);
	tb.ig_result(y);
	tb.ib_result(z);

	tb.read_bmp(input_bmp);
}

SystemParallel::~SystemParallel() {
  tb.write_bmp(_output_bmp);
}
