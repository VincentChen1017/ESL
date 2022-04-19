#include <cmath>
#include <iomanip>

#include "MergeSort.h"
#include "def.h"

MergeSort::MergeSort(sc_module_name n)
    : sc_module(n), t_skt("t_skt"), base_offset(0)
{
  SC_THREAD(do_sort);

  t_skt.register_b_transport(this, &MergeSort::blocking_transport);
}

MergeSort::~MergeSort() = default;

void MergeSort::do_sort()
{
  while (true)
  {
    unsigned int tmp_l1[4][2];
    unsigned int tmp_l2[2][4];
    unsigned int tmp_l3[N];
    unsigned int tmp_l4[N];
    unsigned int tmp_l5[N];
    unsigned int input_buffer[N];

    // input
    input_buffer[0] = i_usort0.read();
    input_buffer[1] = i_usort1.read();
    input_buffer[2] = i_usort2.read();
    input_buffer[3] = i_usort3.read();
    input_buffer[4] = i_usort4.read();
    input_buffer[5] = i_usort5.read();
    input_buffer[6] = i_usort6.read();
    input_buffer[7] = i_usort7.read();

    // check input
    /*std::cout << "CHECK INPUT: " << std::endl;
    for (int i = 0; i < N; i++)
      std::cout << input_buffer[i] << " ";
    std::cout << std::endl;*/

    // level 1
    for (int i = 0; i < 4; i++)
    {
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

    // check level 1
    /*
    std::cout << "CHECK LEVEL1: " << std::endl;
    for (int i = 0; i < 4; i++)
      std::cout << tmp_l1[i][0] << " " << tmp_l1[i][1] << std::endl;
    std::cout << std::endl;
    */

    // level 2
    for (int i = 0; i < 2; i++)
    {
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
    // check level 2
    /*
        std::cout << "CHECK LEVEL2: " << std::endl;
        for (int i = 0; i < 2; i++)
          std::cout << tmp_l2[i][0] << " " << tmp_l2[i][1] << " " << tmp_l2[i][2] << " " << tmp_l2[i][3] << std::endl;
        std::cout << std::endl;
    */
    // level 3
    for (int i = 0; i < 2; i++) // comparator 1 & 4
    {
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

    for (int i = 0; i < 2; i++)
    {
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

    for (int i = 0; i < 2; i++)
    {
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
    unsigned int output[N];
    output[0] = tmp_l5[0];
    output[7] = tmp_l5[7];
    for (int i = 0; i < 3; i++)
    {
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

    o_sort0.write(output[0]);
    o_sort1.write(output[1]);
    o_sort2.write(output[2]);
    o_sort3.write(output[3]);
    o_sort4.write(output[4]);
    o_sort5.write(output[5]);
    o_sort6.write(output[6]);
    o_sort7.write(output[7]);
  }
}

void MergeSort::blocking_transport(tlm::tlm_generic_payload &payload,
                                   sc_core::sc_time &delay)
{
  sc_dt::uint64 addr = payload.get_address();
  addr -= base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  word buffer;
  switch (payload.get_command())
  {
  case tlm::TLM_READ_COMMAND:
    switch (addr)
    {
    case MERGE_SORT_RESULT_ADDR:
      buffer.uc[0] = o_sort0.read();
      buffer.uc[1] = o_sort1.read();
      buffer.uc[2] = o_sort2.read();
      buffer.uc[3] = o_sort3.read();
      buffer.uc[4] = o_sort4.read();
      buffer.uc[5] = o_sort5.read();
      buffer.uc[6] = o_sort6.read();
      buffer.uc[7] = o_sort7.read();
      break;
    case MERGE_SORT_CHECK_ADDR:
      buffer.uc[0] = o_sort0.num_available();
      buffer.uc[1] = o_sort1.num_available();
      buffer.uc[2] = o_sort2.num_available();
      buffer.uc[3] = o_sort3.num_available();
      buffer.uc[4] = o_sort4.num_available();
      buffer.uc[5] = o_sort5.num_available();
      buffer.uc[6] = o_sort6.num_available();
      buffer.uc[7] = o_sort7.num_available();
      break;
    default:
      std::cerr << "Error! MergeSort::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
    data_ptr[4] = buffer.uc[4];
    data_ptr[5] = buffer.uc[5];
    data_ptr[6] = buffer.uc[6];
    data_ptr[7] = buffer.uc[7];

    break;
  case tlm::TLM_WRITE_COMMAND:
    switch (addr)
    {
    case MERGE_SORT_R_ADDR:
      if (mask_ptr[0] == 0xff)
      {
        i_usort0.write(data_ptr[0]);
      }
      if (mask_ptr[1] == 0xff)
      {
        i_usort1.write(data_ptr[1]);
      }
      if (mask_ptr[2] == 0xff)
      {
        i_usort2.write(data_ptr[2]);
      }
      if (mask_ptr[3] == 0xff)
      {
        i_usort3.write(data_ptr[3]);
      }
      if (mask_ptr[4] == 0xff)
      {
        i_usort4.write(data_ptr[4]);
      }
      if (mask_ptr[5] == 0xff)
      {
        i_usort5.write(data_ptr[5]);
      }
      if (mask_ptr[6] == 0xff)
      {
        i_usort6.write(data_ptr[6]);
      }
      if (mask_ptr[7] == 0xff)
      {
        i_usort7.write(data_ptr[7]);
      }
      break;
    default:
      std::cerr << "Error! MergeSort::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    break;
  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}