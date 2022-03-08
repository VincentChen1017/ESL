#include <cmath>

#include "GaussBlurFilter.h"

GaussBlurFilter::GaussBlurFilter(sc_module_name n) : sc_module(n)
{
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// gauss mask
const double mask[MASK_X][MASK_Y] = {{0.077847, 0.123317, 0.077847}, {0.123317, 0.195346, 0.123317}, {0.077847, 0.123317, 0.077847}};

void GaussBlurFilter::do_filter()
{
  while (true)
  {
    r_val = 0;
    g_val = 0;
    b_val = 0;

    for (unsigned int v = 0; v < MASK_Y; ++v)
    {
      for (unsigned int u = 0; u < MASK_X; ++u)
      {
        r_val += i_r.read() * mask[u][v];
        g_val += i_g.read() * mask[u][v];
        b_val += i_b.read() * mask[u][v];
      }
    }
    or_result.write(r_val);
    og_result.write(g_val);
    ob_result.write(b_val);
    //std::cout << "value: " << r_val << std::endl;
    wait(10); // emulate module delay
  }
}
