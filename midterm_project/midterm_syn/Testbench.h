#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

#include "def.h"

class Testbench : public sc_module
{
public:
	sc_in_clk i_clk;
	sc_out < bool >  o_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<8> >::base_out o_usort0;
	cynw_p2p< sc_dt::sc_uint<8> >::base_out o_usort1;
	cynw_p2p< sc_dt::sc_uint<8> >::base_out o_usort2;
	cynw_p2p< sc_dt::sc_uint<8> >::base_out o_usort3;
	cynw_p2p< sc_dt::sc_uint<8> >::base_out o_usort4;
	cynw_p2p< sc_dt::sc_uint<8> >::base_out o_usort5;
	cynw_p2p< sc_dt::sc_uint<8> >::base_out o_usort6;
	cynw_p2p< sc_dt::sc_uint<8> >::base_out o_usort7;
	cynw_p2p< sc_dt::sc_uint<8> >::base_in i_sort0;
	cynw_p2p< sc_dt::sc_uint<8> >::base_in i_sort1;
	cynw_p2p< sc_dt::sc_uint<8> >::base_in i_sort2;
	cynw_p2p< sc_dt::sc_uint<8> >::base_in i_sort3;
	cynw_p2p< sc_dt::sc_uint<8> >::base_in i_sort4;
	cynw_p2p< sc_dt::sc_uint<8> >::base_in i_sort5;
	cynw_p2p< sc_dt::sc_uint<8> >::base_in i_sort6;
	cynw_p2p< sc_dt::sc_uint<8> >::base_in i_sort7;
#else
  sc_fifo<unsigned int> o_usort0;
  sc_fifo<unsigned int> o_usort1;
  sc_fifo<unsigned int> o_usort2;
  sc_fifo<unsigned int> o_usort3;
  sc_fifo<unsigned int> o_usort4;
  sc_fifo<unsigned int> o_usort5;
  sc_fifo<unsigned int> o_usort6;
  sc_fifo<unsigned int> o_usort7;
  sc_fifo<unsigned int> i_sort0;
  sc_fifo<unsigned int> i_sort1;
  sc_fifo<unsigned int> i_sort2;
  sc_fifo<unsigned int> i_sort3;
  sc_fifo<unsigned int> i_sort4;
  sc_fifo<unsigned int> i_sort5;
  sc_fifo<unsigned int> i_sort6;
  sc_fifo<unsigned int> i_sort7;
#endif

  SC_HAS_PROCESS(Testbench);
  Testbench(sc_module_name n);

  ~Testbench();

private:
  int unsorted_array[N];
  int sorted_array[N];

	sc_time total_start_time;
	sc_time total_run_time;
  void feed_unsorted_array();
	void fetch_sorted_array();
  void sink();
  int clock_cycle( sc_time time );
};

#endif

