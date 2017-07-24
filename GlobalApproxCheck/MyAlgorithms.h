/*
* This file contains helper functions that will do the nitty gritty work of
* looping through vectors, finding indexes, and other operations. Purpose
* similar to Lodash in javascript.
*/

#ifndef _my_alroithms_h
#define _my_alroithms_h

#include "MyFunction.h"

#include <vector>

/*
* Run num cycles
*/
void pause(Value* vi) {
  if (cycle_count == 0) {
    std::cin >> cycle_count;
  }
  cycle_count -= 1;
}

/*
* Checks whether the Function is in the vector or not.
* If it is, it will return the index that contains.
* if not, it will return -1.
*/
int getFunctionIndex(std::vector<MyFunction*>, Function*) {

}

#endif
