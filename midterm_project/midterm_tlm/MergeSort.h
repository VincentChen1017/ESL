#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
using namespace sc_core;

#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "def.h"

class MergeSort : public sc_module
{
public:
  tlm_utils::simple_target_socket<MergeSort> t_skt;

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

  SC_HAS_PROCESS(MergeSort);
  MergeSort(sc_module_name n);
  ~MergeSort();

private:
  void do_sort();
  unsigned int base_offset;
  void blocking_transport(tlm::tlm_generic_payload &payload,
                          sc_core::sc_time &delay);
};
#endif
