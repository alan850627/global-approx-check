#define DEBUG_TYPE "GlobalApproxCheck"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Analysis/CallGraph.h"

#include "MyTypes.h"
#include "MyInstruction.h"
#include "MyFunction.h"

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>
using namespace llvm;

namespace {
	struct GlobalApproxCheck : public ModulePass {
		static char ID;
		GlobalApproxCheck() : ModulePass(ID) {}

		// Global Variables
		int cycle_count = 0;
		std::vector<MyFunction*> allFunctions;


		/*
		* Run num cycles
		*/
		void pause(Value* vi) {
			if (cycle_count == 0) {
				std::cin >> cycle_count;
			}
			cycle_count -= 1;
		}

		/*
		* Load child functions
		*/
		void loadChildFunctions(MyFunction* root, CallGraphNode* cgn) {
			for (int i = 0; i < cgn->size(); i++) {
				//TODO: STOP ON RECURSIVE FUNCTIONS
				MyFunction* mf = new MyFunction((*cgn)[i]->getFunction());
				allFunctions.push_back(mf);
				root->childs.push_back(mf);
				mf->parents.push_back(root);
				loadChildFunctions(mf, (*cgn)[i]);
			}
		}

		/*
		* Main Pass
		*/
		virtual bool runOnModule(Module &M) {
			errs() << "\n===================" << "Module " << M.getName() << "===================\n";

			// Variables
			CallGraph cg = CallGraph(M);
			CallGraphNode* cgn = cg.getExternalCallingNode();
			MyFunction* root = 0;

			// locate main.
			for (int i = 0; i < cgn->size(); i++) {
				if (std::string((*cgn)[i]->getFunction()->getName()) == "main") {
					// Found the main function
					root = new MyFunction((*cgn)[i]->getFunction());
					allFunctions.push_back(root);
					cgn = (*cgn)[i];
					break;
				}
			}
			assert(root != 0);

			// Get info out of callgraph.
			loadChildFunctions(root, cgn);

			for (MyFunction* mf : allFunctions) {
				mf->print();
			}

			// Clear Memory
			for (MyFunction* mf : allFunctions) {
				delete mf;
			}
			return false;
		};
	};
}

char GlobalApproxCheck::ID = 0;
static RegisterPass<GlobalApproxCheck> X("GlobalApproxCheck", "Looks for dependencies in modules");
