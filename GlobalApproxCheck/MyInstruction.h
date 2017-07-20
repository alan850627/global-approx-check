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

	bool equals(MyInstruction comp) {
		// TODO
		return false;
	}

	bool equals_deep(MyInstruction comp) {
		// TODO
		return false;
	}

	MyInstruction getAddressDependency() {
		// TODO
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
};

#endif
