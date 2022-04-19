#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "MergeSort_wrap.h"
#else
#include "MergeSort.h"
#endif

class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n );
	~System();
private:
  Testbench tb;
#ifndef NATIVE_SYSTEMC
	MergeSort_wrapper merge_sort;
#else
	MergeSort merge_sort;
#endif
	sc_clock clk;
	sc_signal<bool> rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<8> > usort0;
	cynw_p2p< sc_dt::sc_uint<8> > usort1;
	cynw_p2p< sc_dt::sc_uint<8> > usort2;
	cynw_p2p< sc_dt::sc_uint<8> > usort3;
	cynw_p2p< sc_dt::sc_uint<8> > usort4;
	cynw_p2p< sc_dt::sc_uint<8> > usort5;
	cynw_p2p< sc_dt::sc_uint<8> > usort6;
	cynw_p2p< sc_dt::sc_uint<8> > usort7;
	cynw_p2p< sc_dt::sc_uint<8> > sort0;
	cynw_p2p< sc_dt::sc_uint<8> > sort1;
	cynw_p2p< sc_dt::sc_uint<8> > sort2;
	cynw_p2p< sc_dt::sc_uint<8> > sort3;
	cynw_p2p< sc_dt::sc_uint<8> > sort4;
	cynw_p2p< sc_dt::sc_uint<8> > sort5;
	cynw_p2p< sc_dt::sc_uint<8> > sort6;
	cynw_p2p< sc_dt::sc_uint<8> > sort7;
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


};
#endif
