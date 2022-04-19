#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

#include "def.h"

class MergeSort : public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;

#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<8> >::in i_usort0;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_usort1;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_usort2;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_usort3;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_usort4;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_usort5;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_usort6;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_usort7;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_sort0;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_sort1;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_sort2;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_sort3;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_sort4;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_sort5;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_sort6;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_sort7;
#else
  sc_fifo<unsigned int> i_usort0;
  sc_fifo<unsigned int> i_usort1;
  sc_fifo<unsigned int> i_usort2;
  sc_fifo<unsigned int> i_usort3;
  sc_fifo<unsigned int> i_usort4;
  sc_fifo<unsigned int> i_usort5;
  sc_fifo<unsigned int> i_usort6;
  sc_fifo<unsigned int> i_usort7;
  sc_fifo<unsigned int> o_sort0;
  sc_fifo<unsigned int> o_sort1;
  sc_fifo<unsigned int> o_sort2;
  sc_fifo<unsigned int> o_sort3;
  sc_fifo<unsigned int> o_sort4;
  sc_fifo<unsigned int> o_sort5;
  sc_fifo<unsigned int> o_sort6;
  sc_fifo<unsigned int> o_sort7;
#endif

  SC_HAS_PROCESS(MergeSort);
  MergeSort(sc_module_name n);
  ~MergeSort();

private:
  void do_sort();
  unsigned int tmp_l1[4][2];
  unsigned int tmp_l2[2][4];
  unsigned int tmp_l3[N];
  unsigned int tmp_l4[N];
  unsigned int tmp_l5[N];
  unsigned int input_buffer[N];
  unsigned int output[N];
};
#endif

