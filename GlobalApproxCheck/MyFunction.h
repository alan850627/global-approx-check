#ifndef _my_function_h
#define _my_function_h

#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include "MyInstruction.h"
#include "MyTypes.h"
#include "MyHelper.h"

#include <vector>
#include <utility>
#include <cassert>
#include <iostream>
using namespace llvm;

class MyFunction {
public:
  Function* root;
  std::vector<MyInstruction*> args;
  std::vector<MyInstruction*> insts;
  std::vector<MyInstruction*> critAddrVec;
  std::vector<MyInstruction*> globals;
  std::vector<MyFunction*> childs;
  std::vector<MyFunction*> parents;
  std::string name;

  MyFunction(Function* fun) {
    root = fun;
    name = std::string(fun->getName());
    childs.clear();
    parents.clear();
    critAddrVec.clear();
    globals.clear();
    initializeInstructions();
    initializeArguments();
  }

  MyFunction(const MyFunction& copy_from) {
    root = copy_from.root;
    name = copy_from.name;
    args = copy_from.args;
    critAddrVec = copy_from.critAddrVec;
    globals = copy_from.globals;
    insts = copy_from.insts;
    childs = copy_from.childs;
    parents = copy_from.parents;
  }

  MyFunction& operator=(const MyFunction& copy_from) {
    root = copy_from.root;
    name = copy_from.name;
    args = copy_from.args;
    critAddrVec = copy_from.critAddrVec;
    globals = copy_from.globals;
    insts = copy_from.insts;
    childs = copy_from.childs;
    parents = copy_from.parents;
    return *this;
  }

  ~MyFunction() {
    for (MyInstruction* inst : insts) {
      delete inst;
    }
    for (MyInstruction* inst : globals) {
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
        debug(inst);
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
                  args[i]->propagated = true;
                  parent->debug(vi);
                  vi->markAsNonApprox();
                  parent->propagateUp(vi);
                }
              }
            }
          }
        }
      }
    }
  }

  void propagateFromParent(int arg_num) {
    MyInstruction* mi = args[arg_num];
    propagateDown(mi);
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
    for (MyInstruction* mi : globals) {
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
      if (getMyInstruction(*defI) == 0) {
        addGlobalVariable(*defI);
      }
      return getMyInstruction(*defI);
    }
    else if (opcode == "store") {
      User::op_iterator defI = vi->op_begin();
      defI++;
      if (getMyInstruction(*defI) == 0) {
        addGlobalVariable(*defI);
      }
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
      if (nmi != 0 && !isa<GlobalVariable>(*i)) {
        vec.push_back(nmi);
      } else if (isa<GlobalVariable>(*i)) {
        // Might be a global variable
        addGlobalVariable(*i);
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
      Value* vf = vi->getInstruction()->getOperand(vi->getInstruction()->getNumOperands() - 1); // the function is always the last element.
      assert(isa<Function>(vf));
      MyFunction* mf = getMyFunctionFromVector(vf, childs);
      mf->markRet();
      return;
    }
    if (vi->getOpcodeName() == "alloca") {
      //TODO
      return;
    }
    if (vi->getOpcodeName() == "load") {
      //Found some "address" stored in memory
      MyInstruction* crit = getAddressDependency(vi);
      if (!isInstructionInVector(crit, critAddrVec)) {
        critAddrVec.push_back(crit);
      }
      vi->propagated = true;
      return;
    }
    if (isInstructionInVector(vi, globals)) {
      return;
    }

    vi->propagated = true;
    std::vector<MyInstruction*> dep = getUseDef(vi);
    for (MyInstruction* mi : dep) {
      debug(mi);
      mi->markAsNonApprox();
      propagateUp(mi);
    }
  }

  /*
  * This function takes an instruction as input, and looks at the def-
  * use chain. If we see any instructions that modifies addresses (given
  * in the critAddrVec), then we mark those instructions as nonapproxable.
  */
  void propagateDown(MyInstruction* vi) {
    std::vector<MyInstruction*> uses = getDefUse(vi);
    for (MyInstruction* use : uses) {
      if (use->getOpcodeName() == "call") {
        // continue to propagate in child?
        // 1) Find out which argument is being linked
        // 2) Get the function
        // 3) Call that function's propagateDown
        Value* vf = use->getInstruction()->getOperand(use->getInstruction()->getNumOperands() - 1); // the function is always the last element.
        MyFunction* mf = getMyFunctionFromVector(vf, childs);
        for (int i = 0; i < use->getInstruction()->getNumOperands(); i++) {
          Value* v = use->getInstruction()->getOperand(i);
          if (isa<Instruction>(v)) {
            Instruction* ii = dyn_cast<Instruction>(v);
            if (ii == vi->root) {
              //Found the argument dependency
              mf->propagateFromParent(i);
            }
          }
        }
      } else if (use->getOpcodeName() == "store") {
        MyInstruction* adddep = getAddressDependency(use);
        bool isCritical;
        if (isa<GlobalVariable>(adddep->root) || adddep->getOpcodeName() == "alloca") {
          isCritical = isInstructionInVector(adddep, critAddrVec);
        } else {
          isCritical = isInstructionInVectorDeep(adddep, critAddrVec);
        }
        if (isCritical) {
          // Found a store instruction that stores a new value into
          // critical address.
          if(use->approxStatus == ApproxStatus::nonApproxable && use->propagated) {
            return; //the work is already done
          }
          debug(use);
          use->markAsNonApprox();
          propagateUp2(use);
        }
      } else {
        propagateDown(use);
      }
    }
  }

  /*
  * This function takes an instruction as input, and looks at the def-
  * use chain. If we see any instructions that modifies addresses (given
  * in the critAddrVec), then we mark those instructions as nonapproxable.
  */
  void propagateGlobalsDown(MyInstruction* vi) {
    std::vector<MyInstruction*> uses = getDefUse(vi);
    for (MyInstruction* use : uses) {
      if (use->getOpcodeName() == "call") {
        // continue to propagate in child?
        // 1) Find out which argument is being linked
        // 2) Get the function
        // 3) Call that function's propagateDown
        Value* vf = use->getInstruction()->getOperand(use->getInstruction()->getNumOperands() - 1); // the function is always the last element.
        MyFunction* mf = getMyFunctionFromVector(vf, childs);
        for (int i = 0; i < use->getInstruction()->getNumOperands(); i++) {
          Value* v = use->getInstruction()->getOperand(i);
          if (isa<Instruction>(v)) {
            Instruction* ii = dyn_cast<Instruction>(v);
            if (ii == vi->root) {
              //Found the argument dependency
              mf->propagateFromParent(i);
            }
          }
        }
      } else if (use->getOpcodeName() == "store") {
        MyInstruction* adddep = getAddressDependency(use);
        if (adddep == vi) {
          // Found a store instruction that stores a new value into
          // critical address.
          if(use->approxStatus == ApproxStatus::nonApproxable && use->propagated) {
            return; //the work is already done
          }
          debug(use);
          use->markAsNonApprox();
          propagateUp(use);
        }
      } else {
        propagateGlobalsDown(use);
      }
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
    errs() << " *Critical Addresses:\n";
    for (MyInstruction* crit : critAddrVec) {
      if (isa<GlobalVariable>(crit->root)) {
        errs() << "  ";
      }
      crit->print();
    }
    errs() << " *Global Variables:\n";
    for (MyInstruction* gl : globals) {
      errs() << "  ";
      gl->print();
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

  /*
  * Run num cycles
  */
  void debug(MyInstruction* vi) {
    if (cycle_count == 0) {
      errs() << "**DEBUG**";
      vi->print();
      print();
      std::cin >> cycle_count;
    }
    cycle_count -= 1;
  }

  void addGlobalVariable(Value* v) {
    for (MyInstruction* g : globals) {
      if (g->root == v) {
        return;
      }
    }
    MyInstruction* mi = new MyInstruction(v);
    globals.push_back(mi);
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

  /*
  * This function is very similar to propagateUp, except is only used when we
  * see a "store" when propagating down. In other words, this function only
  * marks those instructions that modify what's pointed by critAddrVec, and
  * cannot add to that vector. That's the primary reason why we have functions
  * that do very similar things.
  */
  void propagateUp2(MyInstruction* vi) {
    if (vi->approxStatus == ApproxStatus::nonApproxable && vi->propagated) {
      // already marked and propagated. No need to do it again.
      return;
    }
    if (vi->getOpcodeName() == "call") {
      Value* vf = vi->getInstruction()->getOperand(vi->getInstruction()->getNumOperands() - 1); // the function is always the last element.
      assert(isa<Function>(vf));
      MyFunction* mf = getMyFunctionFromVector(vf, childs);
      mf->markRet();
      return;
    }
    if (vi->getOpcodeName() == "alloca") {
      //TODO
      return;
    }
    if (vi->getOpcodeName() == "load") {
      //Found some "address" stored in memory
      return;
    }
    if (isInstructionInVector(vi, globals)) {
      return;
    }

    vi->propagated = true;
    std::vector<MyInstruction*> dep = getUseDef(vi);
    for (MyInstruction* mi : dep) {
      debug(mi);
      mi->markAsNonApprox();
      propagateUp2(mi);
    }
  }

  MyFunction* getMyFunctionFromVector(Value* f, std::vector<MyFunction*> v) {
    for (MyFunction* mf : v) {
      if (f == mf->root) {
        return mf;
      }
    }
    return 0;
  }

  bool isInstructionInVector(MyInstruction* mi, std::vector<MyInstruction*> v) {
    for (MyInstruction* i : v) {
      if (mi == i) {
        return true;
      }
    }
    return false;
  }

  bool isInstructionInVectorDeep(MyInstruction* mi, std::vector<MyInstruction*> v) {
    for (MyInstruction* i : v) {
      if (mi->hasSameOperandsDeep(*i)) {
        return true;
      }
    }
    return false;
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
