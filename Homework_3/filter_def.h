#ifndef FILTER_DEF_H_
#define FILTER_DEF_H_

const int MASK_X = 3;
const int MASK_Y = 3;

// gauss mask
const int mask[MASK_X][MASK_Y] = {{77847, 123317, 77847}, {123317, 195346, 123317}, {77847, 123317, 77847}};
#endif
