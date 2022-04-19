#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "SimpleBus.h"
#include "MergeSort.h"
#include "Testbench.h"

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

// int main(int argc, char *argv[])
int sc_main(int argc, char **argv)
{
  cout << "Start MergeSort to sort 8 number" << endl;
  Testbench tb("tb");
  SimpleBus<1, 1> bus("bus");
  bus.set_clock_period(sc_time(CLOCK_PERIOD, SC_NS));
  MergeSort merge_sort("merge_sort");
  tb.initiator.i_skt(bus.t_skt[0]);
  bus.setDecode(0, MERGE_SORT_MM_BASE, MERGE_SORT_MM_BASE + MERGE_SORT_MM_SIZE - 1);
  bus.i_skt[0](merge_sort.t_skt);
  sc_start();
  std::cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;

  return 0;
}
