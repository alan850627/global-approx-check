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
#include <cassert>
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
          Instruction* instr = inst->getInstruction();
          Value *f = instr->getOperand(instr->getNumOperands() - 1); // the function is always the last element.
          assert(isa<Function>(f));
          if (f == root) {
            for (int i = 0; i < args.size(); i++) {
              if (args[i]->approxStatus == ApproxStatus::nonApproxable && !args[i]->propagated) {
                // Propagate this info to parent
                if (isa<Instruction>(instr->getOperand(i))) {
                  MyInstruction* vi = parent->getMyInstruction(instr->getOperand(i));
                  parent->propagateUp(vi);
                }
              }
            }
          }
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

  MyInstruction* getMyInstruction(Value* vi) {
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
      return getMyInstruction(*defI);
    }
    else if (opcode == "store") {
      User::op_iterator defI = vi->op_begin();
      defI++;
      return getMyInstruction(*defI);
    }
    return 0;
  }

  std::vector<MyInstruction*> getUseDef(MyInstruction* mi) {
		std::vector<MyInstruction*> vec;
		Instruction* instr = mi->getInstruction();
		if (instr == 0) {
			// instr is not a llvm::Instruction, therefore it has no usedef.
			return vec;
		}
		for (User::op_iterator i = instr->op_begin(); i != instr->op_end(); i++) {
			MyInstruction* nmi = getMyInstruction(*i);
      if (nmi != 0) {
        vec.push_back(nmi);
      }
		}
		return vec;
	}

	std::vector<MyInstruction*> getDefUse(MyInstruction* mi) {
		std::vector<MyInstruction*> vec;
		for (Value::user_iterator i = mi->root->user_begin(); i != mi->root->user_end(); i++) {
			MyInstruction* nmi = getMyInstruction(*i);
      if (nmi != 0) {
        vec.push_back(nmi);
      }
		}
		return vec;
	}

  /*
  * This function takes an instruction as input, and looks at the use-
  * def chain and mark all those instructions as non-approxable. The
  * state of the input function will not be changed.
  */
  void propagateUp(MyInstruction* vi) {
    if (vi->approxStatus == ApproxStatus::nonApproxable && vi->propagated) {
      // already marked and propagated. No need to do it again.
      return;
    }
    if (vi->getOpcodeName() == "call") {
      //TODO
    }

    vi->propagated = true;
    std::vector<MyInstruction*> dep = getUseDef(vi);
    for (MyInstruction* mi : dep) {
      mi->markAsNonApprox();
      propagateUp(mi);
    }
  }

  void print() {
    errs() << "#Function: " << name << "\n";
    errs() << " *Parents:\n";
    for (MyFunction* mf : parents) {
      errs() << "   -" << mf->name << "\n";
    }
    errs() << " *Childs:\n";
    for (MyFunction* mf : childs) {
      errs() << "   -" << mf->name << "\n";
    }
    errs() << " *Arguments:\n";
    for (MyInstruction* arg : args) {
      arg->print();
    }
    errs() << " *Instructions:\n";
    for (int i = args.size(); i < insts.size(); i++) {
      insts[i]->print();
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

  // void recurPropagateUp(MyInstruction* vi, std::vector<MyInstruction*> history) {
  //   if (getInstructionIndex(history, vi) != -1) {
  //     return;
  //   }
  //   if (vi->approxStatus == ApproxStatus::nonApproxable && vi->propagated) {
  //     // already marked and propagated. No need to do it again.
  //     return;
  //   }
  //   if (vi->getOpcodeName() == "call") {
  //     //TODO
  //   }
  //
  //   vi->propagated = true;
  //   history.push_back(vi);
  //   std::vector<MyInstruction*> dep = getUseDef(vi);
  //   for (MyInstruction* mi : dep) {
  //     mi->markAsNonApprox();
  //     recurPropagateUp(mi, history);
  //   }
  // }
  //
  // int getInstructionIndex(std::vector<MyInstruction*> v, MyInstruction* mi) {
  //   for (int i = 0; i < v.size(); i++) {
  //     if (v[i] == mi) {
  //       return i;
  //     }
  //   }
  //   return -1;
  // }

};

#endif
