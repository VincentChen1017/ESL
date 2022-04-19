#include <cassert>
#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Testbench.h"
#include "def.h"

Testbench::Testbench(sc_module_name n)
    : sc_module(n), initiator("initiator")
{
  SC_THREAD(do_merge);
}

Testbench::~Testbench() {}

void Testbench::do_merge()
{
  unsigned char mask[N];
  int min = 1;
  int max = 255;
  word data;
  srand(time(NULL));
  // bool first = true;
  for (int i = 0; i < 5; i++) // sort 5 array
  {
    for (int i = 0; i < N; i++)
      unsorted_array[i] = rand() % (max - min + 1) + min;
    /*
        unsorted_array[0] = 25;
        unsorted_array[1] = 43;
        unsorted_array[2] = 27;
        unsorted_array[3] = 64;
        unsorted_array[4] = 79;
        unsorted_array[5] = 5;
        unsorted_array[6] = 13;
        unsorted_array[7] = 69;
    */
    std::cout << "The unsorted data"
              << " " << i << " :" << endl;
    for (int i = 0; i < N; i++)
      std::cout << unsorted_array[i] << " ";
    std::cout << endl;

    for (int i = 0; i < N; i++)
    {
      data.uc[i] = unsorted_array[i];
      mask[i] = 0xff;
    }

    initiator.write_to_socket(MERGE_SORT_MM_BASE + MERGE_SORT_R_ADDR, mask, data.uc, N);
    wait(8 * 10, SC_NS);

    initiator.read_from_socket(MERGE_SORT_MM_BASE + MERGE_SORT_RESULT_ADDR, mask, data.uc, N);

    // show the sorted data
    for (int i = 0; i < N; i++)
      sorted_array[i] = data.uc[i];

    std::cout << "The sorted data"
              << " " << i << " :" << endl;
    for (int i = 0; i < N; i++)
      std::cout << sorted_array[i] << " ";
    std::cout << "\n"
              << endl;
  }

  sc_stop();
}
