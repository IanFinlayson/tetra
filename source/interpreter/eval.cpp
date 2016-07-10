/*
 * This program interprets a tetra file by getting its tree representation
 * Takes a single filename containing the Tetra code as a parameter
 */

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "backend.h"
#include "comparisonMap.h"
#include "frontend.h"
#include "operationMap.h"
#include "parallel.h"

// Forward declarations of certain functions. Refer to the function bodies for
// comments
template <typename T>
T paste(const Node*, const Node*, TetraContext&);
template <typename T>
T paste(const Node*, const Node*, TetraScope&, TetraContext&);
template <typename T>
void evaluateNode(const Node*, TData<T>&, TetraContext&);
void aliasArray(const Node*, TArray&, TetraContext&);
template <typename T>
void evaluateAddress(const Node*, TData<T>&, TetraContext&);

// Simplifies an indexed vector value (i.e. x[5][2])
// T should be the expected return type of evaluating the reference
template <typename T>
TArray* evaluateVecVal(const Node* node, TData<T>& ret,
                    TetraContext& context, bool top = true) {
  

  TArray* vec;
  //get the index
  TData<int> indexNum;
  evaluateNode<int>(node->child(1), indexNum, context);
   
  //if there are more indices below this
  if (node->child(0)->kind() == NODE_INDEX) {
    //evaluate them first
    vec = evaluateVecVal(node->child(0), ret, context, false);
  //otherwise get the array
  } else {
    vec = context.lookupVar<TArray>(node->child(0));
  } 

  T value;
  try {
    value = *static_cast<T*>(vec->elementAt(indexNum.getData()).getData());
    if (!top) {
      vec = static_cast<TArray*>(vec->elementAt(indexNum.getData()).getData());
    }
  } catch (Error e) {
    RuntimeError e2(e.getMessage(), node->getLine(), context);
    throw e2;
  }
  ret.setData(value);
  return vec;
}

// Executes structural statements
// i.e. Executes any NodeKind classified as structure
template <typename T>
void evaluateStatement(const Node* node, TData<T>& ret, TetraContext& context) {
  switch (node->kind()) {
    // When entering into a function
    case NODE_FUNCTION: {
      // If there were arguments, meaning that child(0) contains the body of
      // code and child 1 is empty
      if (node->child(1) != NULL) {
        evaluateNode<T>(node->child(1), ret, context);
      } else {
        evaluateNode<T>(node->child(0), ret, context);
      }

    } break;
    case NODE_STATEMENT: {
      // If we are in a parallel block, spawn a thread for this!
      if (context.queryExecutionStatus() == PARALLEL) {
        pthread_t newThread = spawnThread(node->child(0), ret, context);
        // std::cout << ">>>>>>>>>>>>>>>>>" << newThread << std::endl;
        context.addThread(newThread);
        // If the next child node is not a statement,
        //(i.e. we don;t need to spawn another thread)
        // we should get rid of our parallel status
        if (node->child(1)->kind() != NODE_STATEMENT) {
          context.normalizeStatus();
        }
      } else {
        evaluateNode<T>(node->child(0), ret, context);
      }
      // did the statement result in a break, continue, return, etc?
      ExecutionStatus status = context.queryExecutionStatus();
      // Possible optimization, just status == NORMAL || == PARALLEL
      if (status != RETURN && status != BREAK && status != CONTINUE) {
        evaluateNode<T>(node->child(1), ret, context);
      }
    } break;
    case NODE_IF: {
      TData<bool> conditional;
      // evaluate the condition
      evaluateNode<bool>(node->child(0), conditional, context);

      // if the condition was found to be true, execute the 2nd child (child(1))
      if (conditional.getData() == true) {
        evaluateNode<T>(node->child(1), ret, context);
      } else {
        // check for else block and execute it (child(2)) if it exists
        if (node->child(2) != NULL) {
          evaluateNode<T>(node->child(2), ret, context);
        }
      }
    } break;
    case NODE_WHILE: {
      TData<bool> conditional;
      // evaluate the condition
      evaluateNode<bool>(node->child(0), conditional, context);

      // if the condition was found to be true, execute the 2nd branch
      // Then, reevaluate the condition
      while (conditional.getData() == true) {
        // Loop body
        evaluateNode<T>(node->child(1), ret, context);

        // Check is any flags are set, and take the appropriate action
        ExecutionStatus status = context.queryExecutionStatus();
        // If returning, stop the loop and return
        if (status == RETURN) {
          break;
        }
        // If breaking, notify the TetraContext that we have reached the outside
        // of the loop, then  stop the loop
        if (status == BREAK) {
          // Note that is the status was formerly ELIF, the status flag was set
          // to be normalized anyways (since we have to take a branch to
          // encounter a break statement)
          context.normalizeStatus();
          break;
        }
        // If we are continuing, notify the context that we reached the end of
        // the loop, then continue on (reevaluaate condition, etc)
        if (status == CONTINUE) {
          context.normalizeStatus();
        }

        // check the condition again, and store the result in 'condiitonal'
        evaluateNode<bool>(node->child(0), conditional, context);
      }
    } break;
    case NODE_ELIF: {
      context.notifyElif();

      // Traverse the first branch
      evaluateNode<T>(node->child(0), ret, context);

      // Check the execution status
      ExecutionStatus status = context.queryExecutionStatus();

      // evaluate the if-else chain if the first elif clause was false
      if (status == ELIF) {
        evaluateNode<T>(node->child(1), ret, context);
      }

      // Again, check the flag to see if we need to execute the catchall else
      // statement
      status = context.queryExecutionStatus();

      // Also check if the statement actually exists
      if (status == ELIF && node->child(2) != NULL) {
        evaluateNode<T>(node->child(2), ret, context);
      }
    } break;
    case NODE_ELIF_CHAIN: {
      // Try to execute the given case of the ELIF statement
      evaluateNode<T>(node->child(0), ret, context);

      // After evaluating the clause, check the flag to see if we need to try
      // the next clause
      ExecutionStatus status = context.queryExecutionStatus();

      if (status == ELIF) {
        evaluateNode<T>(node->child(1), ret, context);
      }
    } break;
    case NODE_ELIF_CLAUSE: {
      TData<bool> condition;
      // evaluate the condition for this ELIF clause
      evaluateNode<bool>(node->child(0), condition, context);
      // if condiiton is true ,execute the body
      if (condition.getData() == true) {
        // If we have successfully taken a branch, we should notify the
        // interpreter that it no longer needs to check other branches
        context.normalizeStatus();
        evaluateNode<T>(node->child(1), ret, context);
      }
    } break;
    case NODE_INDEX: {
      // Here, T should be the type that needs to be returned, if not, then we
      // really didn;t need it anyways
      //evaluateStatement<T>(node->child(0), ret, context);
      //TArray* lookupArray =
       //   context.lookupVar<TArray>(node->child(0) /*->getString()*/);
      evaluateVecVal<T>(node, ret, context);
    } break;
    case NODE_FOR: {
      // Obtain the list of elements we need to loop over
      TData<TArray> actualCollection;  // If the collection does not exist yet,
                                       // then we will need to place it in
                                       // memory
      TData<TArray*> collection;

      // Check to see if the array can be evaluated by reference, or see if it
      // is a literal that must be evaluated before we can use it
      if (node->child(1)->kind() == NODE_IDENTIFIER ||
          node->child(1)->kind() == NODE_INDEX) {
        // Put the address of the addressable vector into collection
        evaluateAddress<TArray*>(node->child(1), collection, context);
        // aliasArray(node->child(0), collection.getData(), context);
      } else {
        // If we must evaluate by value, we need to evaluate what we are
        // iterating over
        evaluateNode<TArray>(node->child(1), actualCollection, context);

        // Set collection to point to where the actual collection is in memory
        TArray* collection_ptr = &actualCollection.getData();
        collection.setData<TArray*>(collection_ptr);
      }

      assert(collection.getData() != NULL);

      ExecutionStatus status = context.queryExecutionStatus();

      // For_each loop with some extra conditions tacked on to check whether we
      // are returning or breaking
      for (std::vector<TData<void*>, gc_allocator<TData<void*> > /*, mmap_allocator<TData<void*> >*/>::
               const_iterator iter = collection.getData()->begin();
           iter != collection.getData()->end() && status != BREAK &&
           status != RETURN;
           iter++) {
        context.normalizeStatus();

        // Give the variable element in the for_each loop the proper value
        // Must perform type checking
        switch (node->child(0)->type()->getKind()) {
          case TYPE_INT:
            *(context.lookupVar<int>(node->child(0) /*->getString()*/)) =
                *static_cast<int*>((*iter).getData());
            evaluateNode<T>(node->child(2), ret, context);
            break;
          case TYPE_REAL:
            *(context.lookupVar<double>(node->child(0) /*->getString()*/)) =
                *static_cast<double*>((*iter).getData());
            evaluateNode<T>(node->child(2), ret, context);
            break;
          case TYPE_BOOL:
            *(context.lookupVar<bool>(node->child(0) /*->getString()*/)) =
                *static_cast<bool*>((*iter).getData());
            evaluateNode<T>(node->child(2), ret, context);
            break;
          case TYPE_STRING:
            *(context.lookupVar<tstring>(node->child(0) /*->getString()*/)) =
                *static_cast<tstring*>((*iter).getData());
            evaluateNode<T>(node->child(2), ret, context);
            break;
          case TYPE_VECTOR:
            // Alias the array with collection. Note that this means that
            // changes to this array will affect the original array
            // aliasArray(node->child(0),
            // *static_cast<TArray*>((*iter).getData()), context);
            *(context.lookupVar<TArray>(node->child(0) /*->getString()*/)) =
                *static_cast<TArray*>((*iter).getData());
            evaluateNode<T>(node->child(2), ret, context);
            break;
          default:
            tstring message = "Attempting to assign to unknown DataType in for loop.";
            SystemError e(message, node->getLine(), node);
            throw e;
        }
        // Check the current status flag to see if we have to break out
        status = context.queryExecutionStatus();
      }

      // If we broke out of the loop, or continued to the condition and the
      // condiiton failed, normalize the flag
      if (status == BREAK || status == CONTINUE) {
        context.normalizeStatus();
      }

    } break;
    case NODE_PASS:
      // do nothing, just return
      return;
      break;
    default:
      tstring message = "Encountered unsupported NodeKind in statement.";
      Error e(message, node->getLine());
  }
}

// Takes the values denoted by node2 in the context of the sourceContext,
// copies them with new names (deifned by node1) into the TetraScope
// desitnationScope
// Used for passing arguments between function calls
void pasteArgList(const Node* node1, const Node* node2,
                  TetraScope& destinationScope, TetraContext& sourceContext) {
  // Check if we are a NODE_FORMAL_PARAM_LIST (structure), or an actual value
  // that must receive an actual value
  if (node1->kind() == NODE_FORMAL_PARAM_LIST) {  // must have 2 children, as
                                                  // does the corresponding tree
                                                  // of NODE_ACTUAL_PARAM_LIST
    pasteArgList(node1->child(0), node2->child(0), destinationScope,
                 sourceContext);
    if (node1->child(1)) {
      pasteArgList(node1->child(1), node2->child(1), destinationScope,
                 sourceContext);
    }
  } else {
    assert(node1->type() != NULL);
    // Paste the value from the source to the destination
    switch (node1->type()->getKind()) {
      case TYPE_INT:
        paste<int>(node1, node2, destinationScope, sourceContext);
        break;
      case TYPE_REAL:
        paste<double>(node1, node2, destinationScope, sourceContext);
        break;
      case TYPE_BOOL:
        paste<bool>(node1, node2, destinationScope, sourceContext);
        break;
      case TYPE_STRING:
        paste<tstring>(node1, node2, destinationScope, sourceContext);
        break;
      case TYPE_VECTOR:
        // Check if the array exists in memory by the fact that it has a name.
        // If it was built on the fly (i.e. array literal) then we will have to
        // bind it the old fashioned way
        /*if(node2->kind() == NODE_IDENTIFIER) {
          aliasArray(node1,node2,destinationScope,sourceContext);
          }
          else {*/
        paste<TArray>(node1, node2, destinationScope, sourceContext);
        //}
        break;
      default:
        tstring message = "Attempting to pass unknown DataType to function.";
        SystemError e(message, node1->getLine(), node1);
        throw e;
    }
  }
}

// calls a function, returns the return value
template <typename T>
void evaluateFunction(const Node* node, TData<T>& ret, TetraContext& context) {
  // Check to see if it is a TSL function
  // This is a point for optimization. We could agree to some similarity of all
  // TSL names to check 1 condiitonal
  // std::tstring funcName = node->getString();

  tstring funcName = node->child(0)->getString();
  if (funcName == "print") {
    if (node->child(1) != NULL) {
      print(node->child(1), context);
    }
    // Each print does NOT end with a line break
    // std::cout << endl;
  } else if (funcName == "read_int") {
    ret.setData(readInt(context.getThreadID()));
  } else if (funcName == "read_real") {
    ret.setData(readReal(context.getThreadID()));
  } else if (funcName == "read_string") {
    ret.setData(readString(context.getThreadID()));
  } else if (funcName == "read_bool") {
    ret.setData(readBool(context.getThreadID()));
  } else if (funcName == "len") {
    if (node->child(1)->child(0)->type()->getKind() == TYPE_STRING) {
      TData<tstring> value;
      evaluateNode(node->child(1)->child(0), value, context);
      ret.setData(len(value.getData()));
    } else if (node->child(1)->child(0)->type()->getKind() == TYPE_VECTOR) {
      TData<TArray> value;
      evaluateNode(node->child(1)->child(0), value, context);
      ret.setData(len(value.getData()));
    } else {  // attempting to take length of another type is an error
      tstring message = "Attempted to obtain length of unknown type.";
      SystemError e(message, node->getLine(), node);
      throw e;
    }
  } else {
      // USER DEFINED FUNCTION
      // gets the node where the body of the called function begins
      ///////////////	//const Node* funcNode =
      ///FunctionMap::getFunctionNode(FunctionMap::getFunctionSignature(node));

      const Node* funcNode = functions.getFunctionNode(node);

      // check if there are parameters to be passed, and do so if needed
      // This call will have arguments if and only if the calling node has
      // children
      if (node->child(1) != NULL) {
        // When copying arg list, we must have handles to both scopes
        // Calling scope handle
        TetraScope destScope(node);

        // cout << "Old scope X: " << *(context.lookupVar<TArray>("x")) << endl;

        // Initialize the new scope with the passed parameters
        pasteArgList(funcNode->child(0), node->child(1), destScope, context);

        // Set the new scope to the scope we created containing all the parameters
        // cout << "x: " << destScope.lookupVar<int>("x") << endl;
        context.initializeNewScope(destScope);
      } else {  // if there are no args, we still need to initialize a new scope!
        context.initializeNewScope(node);
      }

      // Place this node on the call stack, so it can be printed in the stack
      // trace
      context.getCurrentScope().setCallNode(node);

      // transfer control to the function
      evaluateNode<T>(funcNode, ret, context);

      // returns to the old scope once the function has finished evaluating
      context.exitScope();
  }
}

// evaluates operations on data types
template <typename T>
void evaluateExpression(const Node* node, TData<T>& ret,
                        TetraContext& context) {
  // Static tables containing what actions to perform given which operation node
  static OperationList<T> execData;
  ret.setData(
      execData.execute(node->kind(), node->child(0), node->child(1), context));
}

// Simplifies a vector reference (i.e. x[1] in "x[1] = 7)
// Note that T is going to be a pointer type
template <typename T>
TArray* evaluateVecRef(const Node* node, TData<T>& ret,
                    TetraContext& context, bool top=true) {

  TArray* vec;

  //get the index
  TData<int> indexNum;
  evaluateNode<int>(node->child(1), indexNum, context);
   
  //if there are more indices below this
  if (node->child(0)->kind() == NODE_INDEX) {
    //evaluate them first
    vec = evaluateVecRef(node->child(0), ret, context, false);
  //otherwise get the array
  } else {
    vec = context.lookupVar<TArray>(node->child(0));
  } 

  T value;
  try {
    value = static_cast<T>(vec->elementAt(indexNum.getData()).getData());
    if (!top) {
      vec = static_cast<TArray*>(vec->elementAt(indexNum.getData()).getData());
    }
  } catch (Error e) {
    RuntimeError e2(e.getMessage(), node->getLine(), context);
    throw e2;
  }
  ret.setData(value);
  return vec;
}

// used to get references to variables (for assignment, and possibly other
// features in the future)
// Expected a pointer type for T
template <typename T>
void evaluateAddress(const Node* node, TData<T>& ret, TetraContext& context) {
  // If the node is a Vecref or a possible future type, then we must evaluate
  // the LHS first
  // Must check type so we can cast the info from lookupVar to the appropriate
  // type
  if (node->kind() == NODE_IDENTIFIER || node->kind() == NODE_DECLARATION) {
    assert(node->type() != NULL);

    switch (node->type()->getKind()) {
      case TYPE_INT:
        ret.setData(context.lookupVar<int>(node /*->getString()*/));
        break;
      case TYPE_REAL:
        ret.setData(context.lookupVar<double>(node /*->getString()*/));
        break;
      case TYPE_BOOL:
        ret.setData(context.lookupVar<bool>(node /*->getString()*/));
        break;
      case TYPE_STRING:
        ret.setData(context.lookupVar<tstring>(node /*->getString()*/));
        break;
      case TYPE_VECTOR:
        ret.setData(context.lookupVar<TArray>(node /*->getString()*/));
        break;
      default:
        tstring message = "Attempting to evaluate reference to unknown.";
        SystemError e(message, node->getLine(), node);
        throw e;
    }
  } else if (node->kind() == NODE_INDEX) {  // If LHS is an array index (i.e.
                                             // x[12]), then we must evaluate
                                             // its address before we can paste
                                             // to it
    // Note that node->shild(1) MUST be a NODE_INDEX
    evaluateVecRef<T>(node, ret, context);
  } else {
    tstring message = "Attempted to obtain address of unrecognized LHS NodeKind.";
    SystemError e(message, node->getLine(), node);
    throw e;
  }
}

// puts the value of op2 into the address denoted by op1
// used for performing assignment
// Returns the value of the assignment, with an eye towards chaining assignments
// (x = y = 2)
// Assumes that node1 is a NODE_IDENTIFIER or NODE_INDEX, and node2 is an
// appropriate value
template <typename T>
T paste(const Node* node1, const Node* node2, TetraContext& context) {
  TData<T*> op1;
  TData<T> op2;
  // cout <<"Paste1"<<endl;
  // Evaluate address where the value should be stored, and the value to be
  // stored
  evaluateAddress<T*>(node1, op1, context);
  evaluateNode<T>(node2, op2, context);

  // Perform assignment
  T ret = op2.getData();
  *(op1.getData()) = ret;
  return ret;
}

// This method overloads the basic paste(Node*, Node*, TetraContext*) function,
// except that it is made for cross-scope assignment (i.e. function calls)
// node1 is assumed to be a NODE_FORMAL_PARAM while node2 is a NODE_ACTUAL_PARAM
// Because of this, we don;t have to check for NODE_INDEX or the likes that
// evaluateAddress provides us with
template <typename T>
T paste(const Node* node1, const Node* node2, TetraScope& destinationScope,
        TetraContext& sourceContext) {
  TData<T*> op1;
  TData<T> op2;
  // cout <<"Paste2"<<endl;
  // Get address to store in new scope, and value to paste in

  evaluateNode<T>(node2, op2, sourceContext);

  // cout <<op1.getData()<<endl;
  // cout << "Val: " << node1->getString() << endl;
  // cout << "Address: " << (sourceContext.lookupVar<T>(node1)) << endl;
  op1.setData(destinationScope.lookupVar<T>(node1 /*->getString()*/));
  // Perform assignment
  T ret = op2.getData();
  *(op1.getData()) = ret;
  // cout <<"End val: " << *(op1.getData()) <<" at "<<op1.getData()<<endl;
  return ret;
}

// Called when the interpreter encounters a '=' node, as in x = 5
// Assigns the value of node->child(1) into the address denoted by
// node->child(0)
// At the end, ret will point to whatever value was assigned
template <typename T>
void performAssignment(const Node* node, TData<T>& ret, TetraContext& context) {

  assert(node->child(0)->type() != NULL);

  // We must typecheck here because parent node does not know what we are trying
  // to assign
  switch (node->child(0)->type()->getKind()) {
    case TYPE_INT:
      // Performs the assignment and sets up the return
      ret.setData(paste<int>(node->child(0), node->child(1), context));
      break;
    case TYPE_REAL:
      ret.setData(paste<double>(node->child(0), node->child(1), context));
      break;
    case TYPE_BOOL:
      ret.setData(paste<bool>(node->child(0), node->child(1), context));
      break;
    case TYPE_STRING:
      ret.setData(paste<tstring>(node->child(0), node->child(1), context));
      break;
    case TYPE_VECTOR:
      ret.setData(paste<TArray>(node->child(0), node->child(1), context));
      break;
    default:
      tstring message = "Attempting to assign value of unknown DataType.";
      SystemError e(message, node->getLine(), node);
      throw e;
  }
}

// evaluates conditional statements
template <typename T>
void evaluateCondition(const Node* node, TData<T>& ret, TetraContext& context) {
  // Static tables containing what type of comparison to perform given which
  // operation node
  static ComparisonList<int> compInt;
  static ComparisonList<double> compReal;
  static ComparisonList<bool> compBool;
  static ComparisonList<tstring> compString;
  static ComparisonList<TArray> compList;

  assert(node->child(0)->type() != NULL);

  // fetch operands, must perform type checking here, as the parent node has
  // absolutely no idea what is being compared
  switch (node->child(0)->type()->getKind()) {
    case TYPE_INT: {
      /*TData<int> op1;
        evaluateNode<int>(node->child(0),op1,context);
        TData<int> op2;
        evaluateNode<int>(node->child(1),op2,context);*/
      // actually perform the comparison by calling the appropriate function
      // from the appropriate comparison list
      ret.setData(compInt.execute(node->kind(), node->child(0), node->child(1),
                                  context));
    } break;
    case TYPE_REAL: { /*
                         TData<double> op1;
                         evaluateNode<double>(node->child(0),op1,context);
                         TData<double> op2;
                         evaluateNode<double>(node->child(1),op2,context);
                         ret.setData(compReal.execute(node->kind(),op1,op2));*/
      ret.setData(compReal.execute(node->kind(), node->child(0), node->child(1),
                                   context));
    } break;
    case TYPE_BOOL: {
      /*TData<bool> op1;
        evaluateNode<bool>(node->child(0),op1,context);

        TData<bool> op2;
      //A boolean condiitonal might have only one operand, as in !x where x is a
      bool.
      //Tetra at present forbids using ! for a non-bool type in conditions
      if(node->child(1) != NULL) {
      evaluateNode<bool>(node->child(1),op2,context);
      }
      ret.setData(compBool.execute(node->kind(),op1,op2));*/
      ret.setData(compBool.execute(node->kind(), node->child(0), node->child(1),
                                   context));
    } break;
    case TYPE_STRING: {
      /*TData<tstring> op1;
        evaluateNode<tstring>(node->child(0),op1,context);
        TData<tstring> op2;
        evaluateNode<tstring>(node->child(1),op2,context);
        ret.setData(compString.execute(node->kind(),op1,op2));*/
      ret.setData(compString.execute(node->kind(), node->child(0),
                                     node->child(1), context));
    } break;
    case TYPE_VECTOR: {
      /*
         TData<TArray> op1;
         evaluateNode<TArray>(node->child(0),op1,context);
         TData<TArray> op2;
         evaluateNode<TArray>(node->child(1),op2,context);
         ret.setData(compList.execute(node->kind(),op1,op2));*/
      ret.setData(compList.execute(node->kind(), node->child(0), node->child(1),
                                   context));
    } break;
    default:
      tstring message =  "Attempting to compare values with unknown DataType.";
      Error e(message, node->getLine());
  }
}

// Takes a reference to a NODE_VECVAL, and adds it to the vector represented by
// TArray retVal, then recursively adds further NODE_VECVAL nodes if there are
// any
// From "evaluate immediate" T should be the subtype of the vector (i.e. the
// type of data this vector will hold)
// To summarize, this function constructs a new vector
template <typename T>
void evaluateVectorComponent(const Node* node, TetraContext& context,
                             TArray& arrayVal) {
  // evaluate the first child node, and store its value in this array
  TData<T> x;
  evaluateNode<T>(node->child(0), x, context);

  // Used to insert the data into the array
  TData<void*> insertValue;

  // Used as a placeholder address to the actual value ret->getData so it can be
  // deep copied
  // At the moment, this is criminally inefficient
  // TODO: make this possible without having to copy the data
  T placeHolder = x.getData();

  // Use of the explicit void* is required to actually paste in the value of the
  // pointer
  insertValue.setData<void*>(&placeHolder);
  // Set the deletable type so that it is copied properly when we insert it into
  // the array
  insertValue.setDeletableType<T>();
  arrayVal.addElement(insertValue);

  // because insertValue points to local data, we must re-declare that it must
  // not delete anythiung upon destruction before the object gets destroyed
  insertValue.setDeletableType<void>();

  // Check for additional elements, and add them if they exist
  if (node->numChildren() == 2) {
    evaluateVectorComponent<T>(node->child(1), context, arrayVal);
  }
}

// Evaluates immediate expressions (i.e. '5' in x = 5)
template <typename T>
void evaluateImmediate(const Node* node, TData<T>& ret, TetraContext& context) {
  // if we need to lookup a variable, do so, Note that "lookupVar" handles if we
  // have not seen the tstring before
  // However, we must check the type so that we know what to allocate if needed
  // (optimizaiton point for later?)
  if (node->kind() == NODE_IDENTIFIER) {
    ret.setData(*(context.lookupVar<T>(node /*->getString()*/)));
  }
  // Check the type we are getting, and get thaat value from the node
  else {
    switch (node->kind()) {
      case NODE_INTVAL:
        ret.setData(node->getInt());
        break;
      case NODE_REALVAL:
        ret.setData(node->getReal());
        break;
      case NODE_BOOLVAL:
        ret.setData(node->getBool());
        break;
      case NODE_STRINGVAL:
        ret.setData(node->getString());
        break;
      case NODE_VECRANGE: {
        TArray returnArray;

        // Evaluate the bounds of the range
        TData<int> lowerBound;
        evaluateNode<int>(node->child(0), lowerBound, context);
        TData<int> upperBound;
        evaluateNode<int>(node->child(1), upperBound, context);

        // Assembles the array
        for (int num = lowerBound.getData(); num <= upperBound.getData();
             num++) {
          // Sets up the TData to be properly copied into the array, so that it
          // will exist beyond this scope
          TData<void*> inserter;
          inserter.setData<void*>(&num);
          inserter.setDeletableType<int>();

          // Now, addElement inserts the element as a deep copy
          returnArray.addElement(inserter);

          // because inserter's data was not dynamically allocated, we must
          // prevent it from being freed on destruction
          inserter.setDeletableType<void>();
        }

        ret.setData(returnArray);
      } break;
      case NODE_VECVAL: {
        // If we have a vector array literal, we must deduce the type it is
        // holding and initialize its elements
        TArray returnArray;
        // Check that array is nonempty
        if (node->numChildren() > 0) {
          switch (node->child(0)->type()->getKind()) {
            case TYPE_INT:
              evaluateVectorComponent<int>(node, context, returnArray);
              // returnArray.outputElements<int>();
              break;
            case TYPE_REAL:
              evaluateVectorComponent<double>(node, context, returnArray);
              // returnArray.outputElements<double>();
              break;
            case TYPE_BOOL:
              evaluateVectorComponent<bool>(node, context, returnArray);
              // returnArray.outputElements<bool>();
              break;
            case TYPE_STRING:
              evaluateVectorComponent<tstring>(node, context, returnArray);
              // returnArray.outputElements<tstring>();
              break;
            case TYPE_VECTOR:
              evaluateVectorComponent<TArray>(node, context, returnArray);
              break;
            default:
              tstring message = "Attempted to initialize array with unknown subtype.";
              SystemError e(message, node->getLine(), node);
              throw e;
          }
        }
        // Set the return value
        ret.setData(returnArray);
      } break;
      default:
        tstring message = "Attempting to evaluate unknown immediate NodeType.";
        SystemError e(message, node->getLine(), node);
        throw e;
    }
  }
}

// When the program encounters a flag statement (i.e. break, continue, return),
// notify the interpreter that such a flag was encountered
template <typename T>
void evaluateFlag(const Node* node, TData<T>& ret, TetraContext& context) {
  switch (node->kind()) {
    case NODE_BREAK:
      context.notifyBreak();
      break;
    case NODE_CONTINUE:
      context.notifyContinue();
      break;
    case NODE_RETURN:
      // Check if returning a value, or nothing
      if (node->child(0) != NULL) {
        evaluateNode<T>(node->child(0), ret, context);
      }
      context.notifyReturn();
      break;
    default:
      tstring message = "Unexpected execution flag type encountered.";
      SystemError e(message, node->getLine(), node);
      throw e;
      break;
  }
}

// evaluates generic nodes
template <typename T>
void evaluateNode(const Node* node, TData<T>& ret, TetraContext& context) {
    // check if we should end the program
    if (!TetraEnvironment::isRunning()) {
        throw InterruptError();
    }

  if (TetraEnvironment::isDebugMode()) {
    // If we have encountered a new variable or new scope, context should be
    // expolicitely notified here
    context.updateVarReferenceTable(node);
    // Notify the observer that we are about to execute a new node
    TetraEnvironment::getObserver().notify_E(node, context);
    //#endif
  }

  // Call the appropriate function based on the NodeKind of the node
  switch (node->kind()) {
    case NODE_LT:
    case NODE_LTE:
    case NODE_GT:
    case NODE_GTE:
    case NODE_EQ:
    case NODE_NEQ:
    case NODE_NOT:
      evaluateCondition<T>(node, ret, context);
      break;
    case NODE_FUNCALL:
      evaluateFunction<T>(node, ret, context);
      break;
    case NODE_DECLARATION:
    case NODE_INTVAL:
    case NODE_REALVAL:
    case NODE_BOOLVAL:
    case NODE_STRINGVAL:
    case NODE_VECRANGE:
    case NODE_VECVAL:
    case NODE_IDENTIFIER:
      evaluateImmediate<T>(node, ret, context);
      break;
    case NODE_OR:
    case NODE_AND:
    case NODE_BITAND:
    case NODE_BITOR:
    case NODE_BITXOR:
    case NODE_SHIFTL:
    case NODE_SHIFTR:
    case NODE_PLUS:
    case NODE_MINUS:
    case NODE_TIMES:
    case NODE_DIVIDE:
    case NODE_MODULUS:
    case NODE_EXP:
    case NODE_BITNOT:
      evaluateExpression<T>(node, ret, context);
      break;
    // case NODE_FUNCTION_LIST:
    case NODE_TOPLEVEL_LIST:
    case NODE_FUNCTION:
    case NODE_FORMAL_PARAM_LIST:
    case NODE_STATEMENT:
    case NODE_PASS:
    case NODE_IF:
    case NODE_ELIF:
    case NODE_ELIF_CHAIN:
    case NODE_ELIF_CLAUSE:
    case NODE_WHILE:
    case NODE_FOR:
    case NODE_INDEX:
    case NODE_ACTUAL_PARAM_LIST:
      evaluateStatement<T>(node, ret, context);
      break;
    case NODE_ASSIGN:
    case NODE_GLOBAL:
    case NODE_CONST:
      performAssignment<T>(node, ret, context);
      break;
    case NODE_BREAK:
    case NODE_CONTINUE:
    case NODE_RETURN:
      evaluateFlag<T>(node, ret, context);
      break;
    case NODE_PARFOR:
    case NODE_PARALLEL:
    case NODE_BACKGROUND:
    case NODE_LOCK:
      evaluateParallel<T>(node, ret, context);
      break;
    default:
      tstring message = "Warning: unexpected node kind encountered when attempting to classify node.";
      SystemError e(message, node->getLine(), node);
      throw e;
  }

  if (TetraEnvironment::isDebugMode()) {
    //#ifdef USE_OBSERVER
    // If we are exiting a scope, (i.e. just completed execution of a
    // NODE_FUNCTION
    // notify the context so it can pop its current symbol lookup table
    if (node->kind() == NODE_FUNCTION) {
      TetraEnvironment::getObserver().leftScope_E(context);
      context.popReferenceTable();
    }
    //#endif
  }
}

// Equivilant of main for the interpreter module
int interpret(Node* tree, int debug, int threads) {
  // set environment settings
  TetraEnvironment::setDebug(debug);
  TetraEnvironment::setMaxThreads(threads);
  TetraEnvironment::setRunning();

  // Construct a TetraContext (this also initializes the global scope)
  TetraContext tContext(TetraEnvironment::obtainNewThreadID());

  // find address of main method
  const Node* start = functions.getFunctionNode("main()");

  // If main was not found, print an error
  if (start == NULL) {
    tstring message = "Attempted to call undefined function: main()";
    Error e(message, 0);
    throw e;
  }

  // initialize global vars
  tContext.initializeGlobalVars(tree);

  // will hold value returned to OS. 0 if main does not return an int
  TData<int> retVal(0);

  // Initialize a scope fpr the main method, and run!
  tContext.initializeNewScope(start);

  // If debugging is on, set the breakpoint for the start. Don;t do it any
  // earlier, because the preprocessor needs to 'evaluate nodes' which can
  // improperly trigger the debugger
  if (TetraEnvironment::isDebugMode()) {
    // TetraEnvironment::getObserver().step_E();
    tContext.setStopAtNext(true);
    // Register the main thread
    TetraEnvironment::getObserver().threadCreated_E(tContext.getThreadID(),
                                                    tContext);
  }

  evaluateNode<int>(start, retVal, tContext);
  ThreadEnvironment::joinDetachedThreads();

  tContext.exitScope();

  // Return the value from main
  return retVal.getData();
}
