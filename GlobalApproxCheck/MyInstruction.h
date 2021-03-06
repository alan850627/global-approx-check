#ifndef _my_instruction_h
#define _my_instruction_h

#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"

#include "MyTypes.h"
#include "MyInstruction.h"

#include <vector>
#include <string>
using namespace llvm;

class MyInstruction {
public:
  Value* root;
  ApproxStatus approxStatus;
  int traversePts;
  bool propagated;

  MyInstruction(Instruction* in) {
    root = (Value*)in;
    approxStatus = ApproxStatus::pending;
    propagated = false;
    traversePts = 0;
  }

  MyInstruction(Value* in) {
    root = in;
    approxStatus = ApproxStatus::pending;
    propagated = false;
    traversePts = 0;
  }

  MyInstruction(const MyInstruction& copy_from) {
    root = copy_from.root;
    approxStatus = copy_from.approxStatus;
    propagated = copy_from.propagated;
    traversePts = copy_from.traversePts;
  }

  MyInstruction& operator=(const MyInstruction& copy_from) {
    root = copy_from.root;
    approxStatus = copy_from.approxStatus;
    propagated = copy_from.propagated;
    traversePts = copy_from.traversePts;
    return *this;
  }

  ~MyInstruction() {
  }

  Instruction* getInstruction() {
    Instruction* i = 0;
    if (isa<Instruction>(root)) {
      i = dyn_cast<Instruction>(root);
    }
    return i;
  }

  bool operator==(const MyInstruction& comp) {
    return (root == comp.root);
  }

  bool hasSameOperands(MyInstruction comp) {
    return hasSameOperands(root, comp.root, false);
  }

  bool hasSameOperandsDeep(MyInstruction comp) {
    return hasSameOperands(root, comp.root, true);
  }

  std::string getOpcodeName() {
    Instruction* vi = getInstruction();
    if (vi == 0) {
      return std::string("");
    }
    return std::string(vi->getOpcodeName());
  }

  void markAsNonApprox() {
    if (approxStatus == ApproxStatus::nonApproxable) {
      return;
    }
    propagated = false;
    approxStatus = ApproxStatus::nonApproxable;
    // TODO: This part is commented out to improve print.
    // Instruction* instr = getInstruction();
    // if (instr != 0) {
    //   LLVMContext& C = instr->getContext();
    //   MDNode* N = MDNode::get(C, MDString::get(C, "no"));
    //   instr->setMetadata("approx", N);
    // }
  }

  void print() {
    for (int i = 0; i < traversePts; i++) {
      errs () << "=";
    }
    switch (approxStatus) {
      case ApproxStatus::approxable:
        errs() << *root << " //Approxable\n";
        break;
      case ApproxStatus::nonApproxable:
        errs() << *root << " //NotApproxable:" << propagated << "\n";
        break;
      case ApproxStatus::pending:
        errs() << *root << " \n";
        break;
    }
  }

private:
  bool hasSameOperands(Value* a, Value* b, bool deep) {
    if(a == b) {
      return true;
    }

    if (!isa<Instruction>(a) || !isa<Instruction>(b)) {
      return false;
    }

    Instruction *a_inst = dyn_cast<Instruction>(a);
    Instruction *b_inst = dyn_cast<Instruction>(b);
    if (a_inst->getOpcode() != b_inst->getOpcode() || a_inst->getNumOperands() != b_inst->getNumOperands() || a_inst->getType() != b_inst->getType()) {
      return false;
    }
    // If both instructions have no operands, they are identical.
    if (a_inst->getNumOperands() == 0 && b_inst->getNumOperands() == 0) {
      return true;
    }

    // We have two instructions of identical opcode and #operands.  Check to see
    // if all operands are the same.
    if (!std::equal(a_inst->op_begin(), a_inst->op_end(), b_inst->op_begin())) {
      if (!deep) {
        return false;
      }

      for (User::op_iterator i = a_inst->op_begin(), j = b_inst->op_begin(); i != a_inst->op_end() && j != b_inst->op_end(); i++, j++) {
        if(!hasSameOperands(*i, *j, true)) {
          return false;
        }
      }
      return true;
    }
    return true;
  }
};

#endif
