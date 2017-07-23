#ifndef _my_function_h
#define _my_function_h

#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/raw_ostream.h"

#include "MyInstruction.h"
#include "MyTypes.h"

#include <vector>
#include <utility>
using namespace llvm;

class MyFunction {
public:
  Function* root;
  std::vector<MyInstruction*> args;
  std::vector<MyInstruction*> insts;
  std::string name;

  MyFunction(Function* fun) {
    root = fun;
    name = std::string(fun.getName());
    initializeInstructions();
    initializeArguments();
  }

  MyFunction(const MyFunction& copy_from) {
    root = copy_from.root;
    name = std::string(copy_from.getName());
    args = copy_from.args;
    insts = copy_from.insts;
  }

  MyFunction& operator=(const MyFunction& copy_from) {
    root = copy_from.root;
    name = std::string(copy_from.getName());
    args = copy_from.args;
    insts = copy_from.insts;
		return *this;
	}

  ~MyFUnction() {
    for (inst : insts) {
      delete inst;
    }
  }

  void markRet() {
    for (inst : insts) {
      if (inst->getOpcodeName() == "ret") {
        inst->markAsNonApprox();
      }
    }
  }

  void print() {
    errs() << "Function Name: " << name << "\n\n";
    errs() << "Arguments:\n"
    for (arg : args) {
      arg->print();
    }
    errs() << "\nInstructions:\n"
    for (inst : insts) {
      inst->print();
    }
    errs() << "\n"
  }

  void printSimple() {
    errs() << "Function: " << name << "\n";
  }

private:
  void initializeArguments() {
    args.clear();
    const int NUM_ARGS = (root->arg_end() - root->arg_begin());
    for (int i = 0; i < NUM_ARGS; i++) {
			args.push_back(insts[i]);
    }
  }

  void initializeInstructions() {
    insts.clear();
    for (inst_iterator ii = inst_begin(*root); ii != inst_end(*root); ii++) {
      MyInstruction* mi = new MyInstruction(&*ii);
      insts.push_back(mi);
    }
  }

};

#endif
