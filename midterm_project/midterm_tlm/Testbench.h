#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#include "Initiator.h"
#include "MemoryMap.h"
#include "def.h"

class Testbench : public sc_module
{
public:
  Initiator initiator;

  SC_HAS_PROCESS(Testbench);
  Testbench(sc_module_name n);

  ~Testbench();

private:
  int unsorted_array[N];
  int sorted_array[N];
  void do_merge();
};
#endif
