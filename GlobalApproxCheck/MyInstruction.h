#ifndef MYINSTRUCTION_H
#define MYINSTRUCTION_H

#include "llvm/IR/Instructions.h"
#include <vector>
using namespace llvm;

class MyInstruction {
public:
	Value* root;

	ApproxStatus approxStatus;

	MyInstruction(Instruction* in);

	MyInstruction(Value* in);

	MyInstruction(const MyInstruction& copy_from);

	MyInstruction& operator=(const MyInstruction& copy_from);

	~MyInstruction();

	Instruction* getInstruction();

	std::vector<MyInstruction> getUseDef();

	std::vector<MyInstruction> getDefUse();

	void markAsNonApprox();

  void print();
};


#endif
