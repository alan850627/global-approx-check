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

	MyInstruction () {
		root = 0;
		approxStatus = ApproxStatus::pending;
	}

	MyInstruction(Instruction* in) {
		root = (Value*)in;
		approxStatus = ApproxStatus::pending;
	}

	MyInstruction(Value* in) {
		root = in;
		approxStatus = ApproxStatus::pending;
	}

	MyInstruction(const MyInstruction& copy_from) {
		root = copy_from.root;
		approxStatus = copy_from.approxStatus;
	}

	MyInstruction& operator=(const MyInstruction& copy_from) {
		root = copy_from.root;
		approxStatus = copy_from.approxStatus;
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

	MyInstruction getAddressDependency() {
		std::string opcode = getOpcodeName();
		if (opcode == "load") {
			User::op_iterator defI = vi->op_begin();
			return MyInstruction(*defI);
		}
		else if (opcode == "store") {
			User::op_iterator defI = vi->op_begin();
			defI++;
			return MyInstruction(*defI);
		}
		return MyInstruction();
	}

	std::vector<MyInstruction> getUseDef() {
		std::vector<MyInstruction> vec;
		Instruction* instr = getInstruction();
		if (instr == 0) {
			// instr is not a llvm::Instruction, therefore it has no usedef.
			return vec;
		}
		for (User::op_iterator i = instr->op_begin(); i != instr->op_end(); i++) {
			MyInstruction mi(*i);
			vec.push_back(mi);
		}
		return vec;
	}

	std::vector<MyInstruction> getDefUse() {
		std::vector<MyInstruction> vec;
		for (Value::user_iterator i = root->user_begin(); i != root->user_end(); i++) {
			MyInstruction mi(*i);
			vec.push_back(mi);
		}
		return vec;
	}

	void markAsNonApprox() {
		approxStatus = ApproxStatus::nonApproxable;
		Instruction* instr = getInstruction();
		if (instr != 0) {
			LLVMContext& C = instr->getContext();
			MDNode* N = MDNode::get(C, MDString::get(C, "no"));
			instr->setMetadata("approx", N);
		}
	}

	void print() {
		switch (approxStatus) {
			case ApproxStatus::approxable:
				errs() << "//Instruction:" << *root << " //Status: " << "Approxable\n";
				break;
			case ApproxStatus::nonApproxable:
				errs() << "//Instruction:" << *root << " //Status: " << "Not Approxable\n";
				break;
			case ApproxStatus::pending:
				errs() << "//Instruction:" << *root << " //Status: " << "Pending\n";
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
