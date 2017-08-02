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
#include <cassert>
using namespace llvm;

namespace {
  struct GlobalApproxCheck : public ModulePass {
    static char ID;
    GlobalApproxCheck() : ModulePass(ID) {}

    // Global Variables
    std::vector<MyFunction*> allFunctions;
    std::vector<MyFunction*> stack;

    /*
    * Checks whether the Function is in the vector or not.
    * If it is, it will return the index that contains.
    * if not, it will return -1.
    */
    int getFunctionIndex(std::vector<MyFunction*> v, Function* f) {
      for (int i = 0; i < v.size(); i++) {
        if (v[i]->root == f) {
          return i;
        }
      }
      return -1;
    }

    /*
    * Load child functions
    */
    void loadChildFunctions(MyFunction* root, CallGraphNode* cgn) {
      for (int i = 0; i < cgn->size(); i++) {
        int j = getFunctionIndex(allFunctions, (*cgn)[i]->getFunction());
        if (j == -1) {
          MyFunction* mf = new MyFunction((*cgn)[i]->getFunction());
          allFunctions.push_back(mf);
          root->childs.push_back(mf);
          mf->parents.push_back(root);
          loadChildFunctions(mf, (*cgn)[i]);
        } else {
          MyFunction* mf = allFunctions[j];
          root->childs.push_back(mf);
          mf->parents.push_back(root);
        }
      }
    }

    void setupGlobals(Module &M) {
      for (MyFunction* f : allFunctions) {
        for (Module::global_iterator i = M.global_begin(); i != M.global_end(); i++) {
          f->addGlobalVariable(&*i);
        }
      }
    }

    void findAddressUsageAndPropagateUp(MyFunction* mf) {
      int storeCount = 0;
      for (MyInstruction* mi : mf->insts) {
        std::string oc = mi->getOpcodeName();
        if (oc == "load" || oc == "br") {
          mi->traversePts++;
          mf->debug(mi);
          std::vector<MyInstruction*> dep = mf->getUseDef(mi);
          for (MyInstruction* d : dep) {
            // We don't want to mark alloca instructions unless they are "critical".
            // Since for each "use" (aka load) instruction, the alloca instructions
            // will be referenced as a pointer. At the same time, we also use alloca
            // instructions as function arguments, so marking them this will cause
            // propagateToParent function propagate approxable information.
            if (d->getOpcodeName() != "alloca") {
              d->traversePts++;
              mf->debug(d);
              d->markAsNonApprox();
              mf->propagateUp(d);
              d->traversePts--;
            }
          }
          mi->traversePts--;
        } else if (oc == "store") {
          mi->traversePts++;
          mf->debug(mi);
          Instruction* instr = mi->getInstruction();
          for (User::op_iterator i = instr->op_begin() + 1; i != instr->op_end(); i++) {
            MyInstruction* nmi = mf->getMyInstruction(*i);
            if (nmi != 0 && !isa<GlobalVariable>(*i)) {
              // We don't want to mark alloca instructions unless they are "critical".
              // Since for each "use" (aka load) instruction, the alloca instructions
              // will be referenced as a pointer. At the same time, we also use alloca
              // instructions as function arguments, so marking them this will cause
              // propagateToParent function propagate approxable information.
              if (nmi->getOpcodeName() != "alloca") {
                nmi->traversePts++;
                mf->debug(nmi);
                nmi->markAsNonApprox();
                mf->propagateUp(nmi);
                nmi->traversePts--;
              }
            } else if (isa<GlobalVariable>(*i)) {
              // Might be a global variable
              mf->addGlobalVariable(*i);
            }
          }
          mi->traversePts--;
        }
      }
    }

    void findUnpropagatedInstructionsAndPropagateUp(MyFunction* mf) {
      for (MyInstruction* mi : mf->insts) {
        if (mi->approxStatus == ApproxStatus::nonApproxable && !mi->propagated) {
          mi->traversePts++;
          mf->debug(mi);
          mf->propagateUp(mi);
          mi->traversePts--;
        }
      }
    }

    void findAddressBeingUsedAsData(MyFunction* mf) {
      std::vector<MyInstruction*> cav = mf->critAddrVec;
      for (MyInstruction* mi : cav) {
        mi->traversePts++;
        mf->debug(mi);
        mf->propagateDown(mi);
        for (MyInstruction* comp : mf->insts) {
          if (mi->getOpcodeName() != "alloca" && mi->hasSameOperands(*comp) && (comp != mi)) {
            comp->traversePts++;
            mf->debug(comp);
            mf->propagateDown(comp);
            comp->traversePts--;
          }
        }
        mi->traversePts--;
      }
    }

    void findAllUsesOfGlobalVariable(MyFunction* mf) {
      for (MyInstruction* mi : mf->globals) {
        if (mi->approxStatus == ApproxStatus::nonApproxable) {
          mi->traversePts++;
          mf->debug(mi);
          mf->propagateDown(mi);
          mi->traversePts--;
        }
      }
    }

    void analyzeFunction(MyFunction* mf) {
      if (getFunctionIndex(stack, mf->root) != -1) {
        return;
      }
      stack.push_back(mf);
      findAddressUsageAndPropagateUp(mf);
      findAddressBeingUsedAsData(mf);
      findAllUsesOfGlobalVariable(mf);
      for (MyFunction* child : mf->childs) {
        analyzeFunction(child);
      }
      findUnpropagatedInstructionsAndPropagateUp(mf);
      findAddressBeingUsedAsData(mf);
      findAllUsesOfGlobalVariable(mf);
      mf->propagateToParent();
      stack.pop_back();
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
      setupGlobals(M);

      // GLOBAL-APPROX-CHECK ALGORITHM HERE
      analyzeFunction(root);
      for (MyFunction* mf : allFunctions) {
        analyzeFunction(mf);
      }


      // END GLOBAL-APPROX-CHECK ALGORITHM
      errs() << M.getName() << " ANALYSIS COMPLETE\n";
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
