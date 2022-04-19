#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), merge_sort("merge_sort"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	merge_sort.i_clk(clk);
	merge_sort.i_rst(rst);
	tb.o_usort0(usort0);
	tb.o_usort1(usort1);
	tb.o_usort2(usort2);
	tb.o_usort3(usort3);
	tb.o_usort4(usort4);
	tb.o_usort5(usort5);
	tb.o_usort6(usort6);
	tb.o_usort7(usort7);
	tb.i_sort0(sort0);
	tb.i_sort1(sort1);
	tb.i_sort2(sort2);
	tb.i_sort3(sort3);
	tb.i_sort4(sort4);
	tb.i_sort5(sort5);
	tb.i_sort6(sort6);
	tb.i_sort7(sort7);
	merge_sort.o_sort0(sort0);
	merge_sort.o_sort1(sort1);
	merge_sort.o_sort2(sort2);
	merge_sort.o_sort3(sort3);
	merge_sort.o_sort4(sort4);
	merge_sort.o_sort5(sort5);
	merge_sort.o_sort6(sort6);
	merge_sort.o_sort7(sort7);
	merge_sort.i_usort0(usort0);
	merge_sort.i_usort1(usort1);
	merge_sort.i_usort2(usort2);
	merge_sort.i_usort3(usort3);
	merge_sort.i_usort4(usort4);
	merge_sort.i_usort5(usort5);
	merge_sort.i_usort6(usort6);
	merge_sort.i_usort7(usort7);
}

System::~System() {}
