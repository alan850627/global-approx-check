#ifndef _my_function_h
#define _my_function_h

#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
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
  std::vector<MyFunction*> childs;
  std::vector<MyFunction*> parents;
  std::string name;

  MyFunction(Function* fun) {
    root = fun;
    name = std::string(fun->getName());
    childs.clear();
    parents.clear();
    initializeInstructions();
    initializeArguments();
  }

  MyFunction(const MyFunction& copy_from) {
    root = copy_from.root;
    name = copy_from.name;
    args = copy_from.args;
    insts = copy_from.insts;
    childs = copy_from.childs;
    parents = copy_from.parents;
  }

  MyFunction& operator=(const MyFunction& copy_from) {
    root = copy_from.root;
    name = copy_from.name;
    args = copy_from.args;
    insts = copy_from.insts;
    childs = copy_from.childs;
    parents = copy_from.parents;
    return *this;
  }

  MyFunction() {
    root = 0;
    args.clear();
    insts.clear();
    childs.clear();
    parents.clear();
    name = "";
  }

  ~MyFunction() {
    for (MyInstruction* inst : insts) {
      delete inst;
    }
  }

  bool operator==(const MyFunction& mf) {
    return (root == mf.root);
  }

  bool operator!=(const MyFunction& mf) {
    return (root != mf.root);
  }

  void markRet() {
    for (MyInstruction* inst : insts) {
      if (inst->getOpcodeName() == "ret") {
        inst->markAsNonApprox();
      }
    }
  }

  void propagateToParent() {
    // 1) Find all call instructions in parents
    // 2) Find ones that match the root function name
    // 3) Using use-def chain, mark appropriate instructions based on input.
    for (MyFunction* parent : parents) {
      for (MyInstruction* inst : parent->insts) {
        if (inst->getOpcodeName() == "call") {
          inst->print();
        }
      }
    }
  }

  void propagateToChild(int arg_num) {
    //TODO: This is to see if critical 'variable' is being modified. If it is,
    // mark all those instructions. Not sure if this function is needed.
  }

  MyFunction* getChild(std::string n) {
    for (MyFunction* child : childs) {
      if (child->name == n) {
        return child;
      }
    }
    return 0;
  }

  MyFunction* getParent(std::string n) {
    for (MyFunction* parent : parents) {
      if (parent->name == n) {
        return parent;
      }
    }
    return 0;
  }

  MyInstruction* getInstruction(Value* vi) {
    for (MyInstruction* mi : insts) {
      if (mi->root == vi) {
        return mi;
      }
    }
    return 0;
  }

  MyInstruction* getAddressDependency(MyInstruction* mi) {
    Instruction* vi = mi->getInstruction();
    std::string opcode = mi->getOpcodeName();
    if (opcode == "load") {
      User::op_iterator defI = vi->op_begin();
      return getInstruction(*defI);
    }
    else if (opcode == "store") {
      User::op_iterator defI = vi->op_begin();
      defI++;
      return getInstruction(*defI);
    }
    return 0;
  }

  void print() {
    errs() << "#Function: " << name << "\n";
    errs() << "#Parents:\n";
    for (MyFunction* mf : parents) {
      errs() << "  -" << mf->name << "\n";
    }
    errs() << "#Childs:\n";
    for (MyFunction* mf : childs) {
      errs() << "  -" << mf->name << "\n";
    }
    errs() << "#Arguments:\n";
    for (MyInstruction* arg : args) {
      arg->print();
    }
    errs() << "#Instructions:\n";
    for (MyInstruction* inst : insts) {
      inst->print();
    }
    errs() << "\n";
  }

  void printSimple() {
    errs() << "#Function: " << name << "\n";
  }

private:
  void initializeArguments() {
    args.clear();
    int const NUM_ARGS = std::distance(root->arg_begin(), root->arg_end());
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
