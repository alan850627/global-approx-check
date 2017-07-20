#ifndef _my_function_h
#define _my_function_h

#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"

#include "MyInstruction.h"
#include "MyTypes.h"

#include <vector>
#include <utility>
using namespace llvm;

class MyFunction {
public:
  Function* root;
  std::vector<std::pair<MyInstruction, ApproxStatus>> args;

  MyFunction(Function* fun) {
    root = fun;
    initializeArguments();
  }

private:
  void initializeArguments() {
    const int NUM_ARGS = (root->arg_end() - root->arg_begin());
    inst_iterator ii = inst_begin(root);
    for (int i = 0; i < NUM_ARGS; i++) {
			MyInstruction mi(&*ii);
			args.push_back(std::make_pair(mi, ApproxStatus::pending));
      ii++;
    }
  }

};

#endif
