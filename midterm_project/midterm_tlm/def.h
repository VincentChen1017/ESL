#ifndef SORT_DEF_H_
#define SORT_DEF_H_
#define N 8
#include <cstdint>

//  transport addresses
// Used between blocking_transport() & do_merge()
const int MERGE_SORT_R_ADDR = 0x00000000;
const int MERGE_SORT_RESULT_ADDR = 0x00000004;
const int MERGE_SORT_CHECK_ADDR = 0x00000008;

union word
{
  int64_t sint;
  unsigned int uint;
  unsigned char uc[N];
};

#endif
