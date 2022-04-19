#include "System.h"
System::System( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ), 
	tb("tb"), gauss_filter("gauss_filter"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	gauss_filter.i_clk(clk);
	gauss_filter.i_rst(rst);
	tb.o_rgb(rgb);
	tb.ir_result(r_result);
	tb.ig_result(g_result);
	tb.ib_result(b_result);
	gauss_filter.i_rgb(rgb);
	gauss_filter.or_result(r_result);
	gauss_filter.og_result(g_result);
	gauss_filter.ob_result(b_result);

  tb.read_bmp(input_bmp);
}

System::~System() {
  tb.write_bmp(_output_bmp);
}
