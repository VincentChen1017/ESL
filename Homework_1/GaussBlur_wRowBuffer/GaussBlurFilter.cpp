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
  int i, row, index;
  bool First_Time = 1;
  while (true)
  {
    // The first 3 row
    if (First_Time)
    {
      for (row = 0; row < 3; row++)
      {
        for (i = 0; i < 258; i++)
        {
          // std::cout << "first 3 AVAI: " << i_r.num_available() << std::endl;
          r_row[0][i] = r_row[1][i];
          g_row[0][i] = g_row[1][i];
          b_row[0][i] = b_row[1][i];
          r_row[1][i] = r_row[2][i];
          g_row[1][i] = g_row[2][i];
          b_row[1][i] = b_row[2][i];
          r_row[2][i] = i_r.read();
          g_row[2][i] = i_g.read();
          b_row[2][i] = i_b.read();
        }
      }
      First_Time = 0;
      wait(258 * 3);
    }
    else
    {
      for (i = 0; i < 258; i++)
      {
        // std::cout << "not first 3 AVAI: " << i_r.num_available() << std::endl;
        r_row[0][i] = r_row[1][i];
        g_row[0][i] = g_row[1][i];
        b_row[0][i] = b_row[1][i];
        r_row[1][i] = r_row[2][i];
        g_row[1][i] = g_row[2][i];
        b_row[1][i] = b_row[2][i];
        r_row[2][i] = i_r.read();
        g_row[2][i] = i_g.read();
        b_row[2][i] = i_b.read();
      }
      wait(258);
    }

    for (index = 1; index < 257; index++)
    {
      // std::cout << "HERE222" << index << std::endl;
      r_val = r_row[0][index - 1] * mask[0][0] + r_row[0][index] * mask[0][1] + r_row[0][index + 1] * mask[0][2] +
              r_row[1][index - 1] * mask[1][0] + r_row[1][index] * mask[1][1] + r_row[1][index + 1] * mask[1][2] +
              r_row[2][index - 1] * mask[2][0] + r_row[2][index] * mask[2][1] + r_row[2][index + 1] * mask[2][2];
      g_val = g_row[0][index - 1] * mask[0][0] + g_row[0][index] * mask[0][1] + g_row[0][index + 1] * mask[0][2] +
              g_row[1][index - 1] * mask[1][0] + g_row[1][index] * mask[1][1] + g_row[1][index + 1] * mask[1][2] +
              g_row[2][index - 1] * mask[2][0] + g_row[2][index] * mask[2][1] + g_row[2][index + 1] * mask[2][2];
      b_val = b_row[0][index - 1] * mask[0][0] + b_row[0][index] * mask[0][1] + b_row[0][index + 1] * mask[0][2] +
              b_row[1][index - 1] * mask[1][0] + b_row[1][index] * mask[1][1] + b_row[1][index + 1] * mask[1][2] +
              b_row[2][index - 1] * mask[2][0] + b_row[2][index] * mask[2][1] + b_row[2][index + 1] * mask[2][2];
      or_result.write(r_val);
      og_result.write(g_val);
      ob_result.write(b_val);
      // std::cout << "value: " << r_val << std::endl;
      // std::cout << "HERE free: " << or_result.num_free() << std::endl;
      wait(1);
    }
  }
}
