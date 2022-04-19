#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "MergeSort.h"
#include "directives.h"
#include "def.h"

MergeSort::MergeSort(sc_module_name n): sc_module(n)
{
#ifndef NATIVE_SYSTEMC
	HLS_FLATTEN_ARRAY(input_buffer);
        HLS_FLATTEN_ARRAY(tmp_l1);
        HLS_FLATTEN_ARRAY(tmp_l2);
        HLS_FLATTEN_ARRAY(tmp_l3);
        HLS_FLATTEN_ARRAY(tmp_l4);
        HLS_FLATTEN_ARRAY(tmp_l5);
        HLS_FLATTEN_ARRAY(output);
#endif
  SC_THREAD(do_sort);

	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_usort0.clk_rst(i_clk, i_rst);
	i_usort1.clk_rst(i_clk, i_rst);
	i_usort2.clk_rst(i_clk, i_rst);
	i_usort3.clk_rst(i_clk, i_rst);
	i_usort4.clk_rst(i_clk, i_rst);
	i_usort5.clk_rst(i_clk, i_rst);
	i_usort6.clk_rst(i_clk, i_rst);
	i_usort7.clk_rst(i_clk, i_rst);
  o_sort0.clk_rst(i_clk, i_rst);
  o_sort1.clk_rst(i_clk, i_rst);
  o_sort2.clk_rst(i_clk, i_rst);
  o_sort3.clk_rst(i_clk, i_rst);
  o_sort4.clk_rst(i_clk, i_rst);
  o_sort5.clk_rst(i_clk, i_rst);
  o_sort6.clk_rst(i_clk, i_rst);
  o_sort7.clk_rst(i_clk, i_rst);
#endif

}

MergeSort::~MergeSort(){}

void MergeSort::do_sort()
{
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_usort0.reset();
		i_usort1.reset();
		i_usort2.reset();
		i_usort3.reset();
		i_usort4.reset();
		i_usort5.reset();
		i_usort6.reset();
		i_usort7.reset();
		o_sort0.reset();
		o_sort1.reset();
		o_sort2.reset();
		o_sort3.reset();
		o_sort4.reset();
		o_sort5.reset();
		o_sort6.reset();
		o_sort7.reset();
#endif
		wait();
	}
  while (true)
  {
 	LOOP_PIPELINE;       	
#ifndef NATIVE_SYSTEMC 
	{
	HLS_DEFINE_PROTOCOL("input");
    	input_buffer[0] = i_usort0.get();
    	input_buffer[1] = i_usort1.get();
    	input_buffer[2] = i_usort2.get();
    	input_buffer[3] = i_usort3.get();
    	input_buffer[4] = i_usort4.get();
    	input_buffer[5] = i_usort5.get();
    	input_buffer[6] = i_usort6.get();
    	input_buffer[7] = i_usort7.get();
			wait();
	}
#else
    // input
    input_buffer[0] = i_usort0.read();
    input_buffer[1] = i_usort1.read();
    input_buffer[2] = i_usort2.read();
    input_buffer[3] = i_usort3.read();
    input_buffer[4] = i_usort4.read();
    input_buffer[5] = i_usort5.read();
    input_buffer[6] = i_usort6.read();
    input_buffer[7] = i_usort7.read();
#endif
    
    // check input
    /*std::cout << "CHECK INPUT: " << std::endl;
    for (int i = 0; i < N; i++)
      std::cout << input_buffer[i] << " ";
    std::cout << std::endl;*/

 //   LOOP_PIPELINE;
    // level 1
{
    LOOP_LATENCY;
    for (int i = 0; i < 4; i++)
    {
      LOOP_UNROLL_L1;
      if (input_buffer[2 * i] < input_buffer[2 * i + 1])
      {
        tmp_l1[i][0] = input_buffer[2 * i];
        tmp_l1[i][1] = input_buffer[2 * i + 1];
      }
      else
      {
        tmp_l1[i][0] = input_buffer[2 * i + 1];
        tmp_l1[i][1] = input_buffer[2 * i];
      }
    }
}
    // check level 1
    /*
    std::cout << "CHECK LEVEL1: " << std::endl;
    for (int i = 0; i < 4; i++)
      std::cout << tmp_l1[i][0] << " " << tmp_l1[i][1] << std::endl;
    std::cout << std::endl;
    */

    // level 2
{
    LOOP_LATENCY;
    for (int i = 0; i < 2; i++)
    {
		  LOOP_UNROLL_L2;
      if (tmp_l1[2 * i][0] < tmp_l1[2 * i + 1][0])
      {
        tmp_l2[i][0] = tmp_l1[2 * i][0];
        tmp_l2[i][1] = tmp_l1[2 * i + 1][0];
      }
      else
      {
        tmp_l2[i][0] = tmp_l1[2 * i + 1][0];
        tmp_l2[i][1] = tmp_l1[2 * i][0];
      }

      if (tmp_l1[2 * i][1] < tmp_l1[2 * i + 1][1])
      {
        tmp_l2[i][2] = tmp_l1[2 * i][1];
        tmp_l2[i][3] = tmp_l1[2 * i + 1][1];
      }
      else
      {
        tmp_l2[i][2] = tmp_l1[2 * i + 1][1];
        tmp_l2[i][3] = tmp_l1[2 * i][1];
      }    
     }
}
    // check level 2
    /*
        std::cout << "CHECK LEVEL2: " << std::endl;
        for (int i = 0; i < 2; i++)
          std::cout << tmp_l2[i][0] << " " << tmp_l2[i][1] << " " << tmp_l2[i][2] << " " << tmp_l2[i][3] << std::endl;
        std::cout << std::endl;
    */
    // level 3
{
    LOOP_LATENCY;
    for (int i = 0; i < 2; i++) // comparator 1 & 4
    {
		  LOOP_UNROLL_L3;
      if (tmp_l2[0][3 * i] < tmp_l2[1][3 * i])
      {
        tmp_l3[0 + 6 * i] = tmp_l2[0][3 * i];
        tmp_l3[1 + 6 * i] = tmp_l2[1][3 * i];
      }
      else
      {
        tmp_l3[0 + 6 * i] = tmp_l2[1][3 * i];
        tmp_l3[1 + 6 * i] = tmp_l2[0][3 * i];
      }
    }

    for (int i = 0; i < 2; i++) // comparator 2 & 3
    {
		  LOOP_UNROLL_L3;
      if (tmp_l2[i][1] < tmp_l2[i][2])
      {
        tmp_l3[2 + 2 * i] = tmp_l2[i][1];
        tmp_l3[3 + 2 * i] = tmp_l2[i][2];
      }
      else
      {
        tmp_l3[2 + 2 * i] = tmp_l2[i][2];
        tmp_l3[3 + 2 * i] = tmp_l2[i][1];
      }
     }    
}
    /*
        if (tmp_l2[0][0] < tmp_l2[1][0])
        {
          tmp_l3[0] = tmp_l2[0][0];
          tmp_l3[1] = tmp_l2[1][0];
        }
        else
        {
          tmp_l3[0] = tmp_l2[1][0];
          tmp_l3[1] = tmp_l2[0][0];
        }
        if (tmp_l2[0][1] < tmp_l2[0][2])
        {
          tmp_l3[2] = tmp_l2[0][1];
          tmp_l3[3] = tmp_l2[0][2];
        }
        else
        {
          tmp_l3[2] = tmp_l2[0][2];
          tmp_l3[3] = tmp_l2[0][1];
        }
        if (tmp_l2[1][1] < tmp_l2[1][2])
        {
          tmp_l3[4] = tmp_l2[1][1];
          tmp_l3[5] = tmp_l2[1][2];
        }
        else
        {
          tmp_l3[4] = tmp_l2[1][2];
          tmp_l3[5] = tmp_l2[1][1];
        }
        if (tmp_l2[0][3] < tmp_l2[1][3])
        {
          tmp_l3[6] = tmp_l2[0][3];
          tmp_l3[7] = tmp_l2[1][3];
        }
        else
        {
          tmp_l3[6] = tmp_l2[1][3];
          tmp_l3[7] = tmp_l2[0][3];
        }
    */

    // check level 3
    /*
        std::cout << "CHECK LEVEL3: " << std::endl;
        for (int i = 0; i < 8; i++)
          std::cout << tmp_l3[i] << " " << std::endl;
        std::cout << std::endl;
    */
    // level 4
    tmp_l4[0] = tmp_l3[0];
    tmp_l4[1] = tmp_l3[1];
    tmp_l4[6] = tmp_l3[6];
    tmp_l4[7] = tmp_l3[7];
{
    LOOP_LATENCY;
    for (int i = 0; i < 2; i++)
    {
			LOOP_UNROLL_L4;
      if (tmp_l3[3 - i] < tmp_l3[5 - i])
      {
        tmp_l4[2 + 2 * i] = tmp_l3[3 - i];
        tmp_l4[3 + 2 * i] = tmp_l3[5 - i];
      }
      else
      {
        tmp_l4[2 + 2 * i] = tmp_l3[5 - i];
        tmp_l4[3 + 2 * i] = tmp_l3[3 - i];
      }
    }
}
    /*
        if (tmp_l3[3] < tmp_l3[5])
        {
          tmp_l4[2] = tmp_l3[3];
          tmp_l4[3] = tmp_l3[5];
        }
        else
        {
          tmp_l4[2] = tmp_l3[5];
          tmp_l4[3] = tmp_l3[3];
        }
        if (tmp_l3[2] < tmp_l3[4])
        {
          tmp_l4[4] = tmp_l3[2];
          tmp_l4[5] = tmp_l3[4];
        }
        else
        {
          tmp_l4[4] = tmp_l3[4];
          tmp_l4[5] = tmp_l3[2];
        }
    */

    // check level 4
    /*
        std::cout << "CHECK LEVEL4: " << std::endl;
        for (int i = 0; i < 8; i++)
          std::cout << tmp_l4[i] << " " << std::endl;
        std::cout << std::endl;
    */
    // level 5
    tmp_l5[0] = tmp_l4[0];
    tmp_l5[3] = tmp_l4[3];
    tmp_l5[4] = tmp_l4[4];
    tmp_l5[7] = tmp_l4[7];
{
    LOOP_LATENCY;
    for (int i = 0; i < 2; i++)
    {
			LOOP_UNROLL_L5;
      if (tmp_l4[1 + 4 * i] < tmp_l4[2 + 4 * i])
      {
        tmp_l5[1 + 4 * i] = tmp_l4[1 + 4 * i];
        tmp_l5[2 + 4 * i] = tmp_l4[2 + 4 * i];
      }
      else
      {
        tmp_l5[1 + 4 * i] = tmp_l4[2 + 4 * i];
        tmp_l5[2 + 4 * i] = tmp_l4[1 + 4 * i];
      }
     }    
}
    /*
        if (tmp_l4[1] < tmp_l4[2])
        {
          tmp_l5[1] = tmp_l4[1];
          tmp_l5[2] = tmp_l4[2];
        }
        else
        {
          tmp_l5[1] = tmp_l4[2];
          tmp_l5[2] = tmp_l4[1];
        }
        if (tmp_l4[5] < tmp_l4[6])
        {
          tmp_l5[5] = tmp_l4[5];
          tmp_l5[6] = tmp_l4[6];
        }
        else
        {
          tmp_l5[5] = tmp_l4[6];
          tmp_l5[6] = tmp_l4[5];
        }
    */

    // check level 5
    /*
        std::cout << "CHECK LEVEL5: " << std::endl;
        for (int i = 0; i < 8; i++)
          std::cout << tmp_l5[i] << " " << std::endl;
        std::cout << std::endl;
    */
    // output
    output[0] = tmp_l5[0];
    output[7] = tmp_l5[7];
{
   LOOP_LATENCY;
   for (int i = 0; i < 3; i++)
    {
			LOOP_UNROLL_OUTPUT;
      if (tmp_l5[1 + i] < tmp_l5[4 + i])
      {
        output[1 + 2 * i] = tmp_l5[1 + i];
        output[2 + 2 * i] = tmp_l5[4 + i];
      }
      else
      {
        output[1 + 2 * i] = tmp_l5[4 + i];
        output[2 + 2 * i] = tmp_l5[1 + i];
      }
    }
}
//		wait();   // if use for loop the wait() need to be add

    /*
        if (tmp_l5[1] < tmp_l5[4])
        {
          output[1] = tmp_l5[1];
          output[2] = tmp_l5[4];
        }
        else
        {
          output[1] = tmp_l5[4];
          output[2] = tmp_l5[1];
        }
        if (tmp_l5[2] < tmp_l5[5])
        {
          output[3] = tmp_l5[2];
          output[4] = tmp_l5[5];
        }
        else
        {
          output[3] = tmp_l5[5];
          output[4] = tmp_l5[2];
        }
        if (tmp_l5[3] < tmp_l5[6])
        {
          output[5] = tmp_l5[3];
          output[6] = tmp_l5[6];
        }
        else
        {
          output[5] = tmp_l5[6];
          output[6] = tmp_l5[3];
        }
    */

    // check output
    /*std::cout << "CHECK OUTPUT: " << std::endl;
    for (int i = 0; i < N; i++)
      std::cout << output[i] << " ";
    std::cout << std::endl;*/

#ifndef NATIVE_SYSTEMC
		{	
			HLS_DEFINE_PROTOCOL("output");
    	o_sort0.put(output[0]);
    	o_sort1.put(output[1]);
    	o_sort2.put(output[2]);
    	o_sort3.put(output[3]);
    	o_sort4.put(output[4]);
    	o_sort5.put(output[5]);
    	o_sort6.put(output[6]);
    	o_sort7.put(output[7]);
			wait();
		}
#else
    o_sort0.write(output[0]);
    o_sort1.write(output[1]);
    o_sort2.write(output[2]);
    o_sort3.write(output[3]);
    o_sort4.write(output[4]);
    o_sort5.write(output[5]);
    o_sort6.write(output[6]);
    o_sort7.write(output[7]);
#endif

  }
}


