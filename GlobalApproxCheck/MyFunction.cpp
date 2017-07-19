#ifndef MYFUNCTION_CPP
#define MYFUNCTION_CPP

#include "llvm/IR/Function.h"

#include "MyInstruction.cpp"
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

	}



};

#endif
