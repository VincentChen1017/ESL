#ifndef GAUSSBLUR_FILTER_H_
#define GAUSSBLUR_FILTER_H_
#include <systemc>
using namespace sc_core;

#include "filter_def.h"

class GaussBlurFilter : public sc_module
{
public:
  sc_in_clk i_clk;
  sc_in<bool> i_rst;
  sc_fifo_in<unsigned char> i_r;
  sc_fifo_in<unsigned char> i_g;
  sc_fifo_in<unsigned char> i_b;
  sc_fifo_out<double> or_result;
  sc_fifo_out<double> og_result;
  sc_fifo_out<double> ob_result;

  SC_HAS_PROCESS(GaussBlurFilter);
  GaussBlurFilter(sc_module_name n);
  ~GaussBlurFilter() = default;

private:
  void do_filter();
  double r_val;
  double g_val;
  double b_val;
};
#endif
