#ifndef _my_function_tree_node_h
#define _my_function_tree_node_h

#include "MyFunction.h"

#include <vector>
#include <string>

class MyFunctionGraphNode {
public:
  MyFunction* root;
  std::vector<MyFunction*> childs;
  std::vector<MyFunction*> parents;

  MyFunctionGraphNode(MyFunction* mf) {
    //TODO
  }

  MyFunctionGraphNode(const MyFunctionGraphNode& copy_from) {
    //TODO
  }

  MyFunctionGraphNode& operator=(const MyFunctionGraphNode& copy_from) {
    //TODO
    return *this;
  }

  void propagateToParent() {
    //TODO
    // 1) Find all call instructions in parents
    // 2) Find ones that match the root function name
    // 3) Using use-def chain, mark appropriate instructions based on input.
  }

  void propagateToChild(int arg_num) {
    //TODO: This is to see if critical 'variable' is being modified. If it is,
    // mark all those instructions. Not sure if this function is needed.
  }

};

#endif
