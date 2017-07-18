#define DEBUG_TYPE "GlobalApproxCheck"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Analysis/CallGraph.h"
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <iostream>
using namespace llvm;

namespace {
	struct GlobalApproxCheck : public ModulePass {
		static char ID;
		GlobalApproxCheck() : ModulePass(ID) {}

		// Global Variables
		int cycle_count = 0;


		/*
		* Run num cycles
		*/
		void pause(Value* vi) {
			if (cycle_count == 0) {
				cin >> cycle_count;
			}
			cycle_count -= 1;
		}

		/*
		* Main Pass
		*/
		virtual bool runOnModule(Module &M) {
			errs() << "\n===================" << "Module " << M.getName() << "===================\n\n";
			CallGraph cg = CallGraph(M);
			cg.dump();
			return false;
		};
	};
}

char GlobalApproxCheck::ID = 0;
static RegisterPass<GlobalApproxCheck> X("GlobalApproxCheck", "Looks for dependencies in modules");
