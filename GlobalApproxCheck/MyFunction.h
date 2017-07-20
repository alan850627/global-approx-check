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
  std::vector<MyInstruction> myInsts;

  MyFunction(Function* fun) {
    root = fun;
    initializeInstructions();
    initializeArguments();
  }

private:
  void initializeArguments() {
    args.clear();
    const int NUM_ARGS = (root->arg_end() - root->arg_begin());
    for (int i = 0; i < NUM_ARGS; i++) {
			args.push_back(std::make_pair(myInsts[i], ApproxStatus::pending));
    }
  }

  void initializeInstructions() {
    myInsts.clear();
    for (inst_iterator ii = inst_begin(*root); ii != inst_end(*root); ii++) {
      myInsts.push_back(MyInstruction(&*ii));;
    }
  }

};

#endif
