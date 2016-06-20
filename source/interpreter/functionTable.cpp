/*
 * This file builds a function lookup table so that when the interpreter
 * encounters a function call, it can easily find the address of the
 * appropriate node where the called function code resides
 *Since there is only one funciton table per program (even if using multiple
 files, the further functions should be addable by calling buildTree for each
 file's syntax tree) This uses a single object design.
 */

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "backend.h"
#include "frontend.h"

using std::string;

// Given a function signature, returns the adress of a node containing the
// function definition for that signature
const Node* FunctionMap::getFunctionNode(const string functionSignature) {
  // if function is not there, will return default Node* (i.e. NULL)
  return lookup[functionSignature];
}

const Node* FunctionMap::getFunctionNode(DataType* params, std::string name) {
  return lookup[name + typeToString(params)];
}

// Calls the appropriate function based on the number identifier attached to the
// node
const Node* FunctionMap::getFunctionNode(const Node* callNode) {
  // TSL functions are given the low numbers, but functionLookup only contains
  // user defined functions
  // Thus we must subtract the offset TSL_FUNCS (number of TSL functions) to get
  // the proper index in this array
  return functionLookup[callNode->getInt() - TSL_FUNCS];
}

FunctionMap::FunctionMap() {
  this->functionLookup = NULL;
}

// Fills the function map given the specified base node TODO: this function is
// comprised of old functionality followed by new changes to accomadate constant
// time lookup. This function can potentially bwe rewritten to both search for
// functions and assign them values at the same time
void FunctionMap::build(Node* tree) {
  if (tree->kind() == NODE_TOPLEVEL_LIST 
      || tree->kind() == NODE_CLASS_PART) {
    
    // by frontend specifications, there MUST be a child to add
    Node* candidate = tree->child(0);
    if (candidate->kind() == NODE_FUNCTION) {
      
      //add the params to the sym table
      inferParams(candidate); 

      //update the DataType to a function
      DataType* type = new DataType(TYPE_FUNCTION);

      //create the paramtype
      DataType* paramTypes = new DataType(TYPE_TUPLE);

      //if it has params...
      if (candidate->symtable) {
        //add the types of the formal params
        for (std::map<std::string, Symbol>::reverse_iterator rit 
            = candidate->symtable->rbegin(); 
            rit != candidate->symtable->rend(); ++rit ) {
          
          /* add them to the tuple */
          paramTypes->subtypes->push_back(*(rit->second.getType()));
        } 
      }

      //add the param tuple type as a subtype */
      type->subtypes->push_back(*paramTypes);

      //add the return val subtype
      type->subtypes->push_back(*candidate->type());

      //update the candidates datatype
      candidate->setDataType(type);

      //if this function is already in the table
      if (lookup.count(getFunctionSignature(candidate)) > 0) {
        throw Error("Duplicate function. ", candidate->getLine());
      }

      //otherwise, it's not in the table, so add it!
      lookup[getFunctionSignature(candidate)] = candidate;
    }
    // checks if there are further functions to add
    if (tree->child(1) != NULL) {
      build(tree->child(1));
    }
  }
}

/* wrapper around std::map's insert function */
void FunctionMap::insert(std::pair<string,Node*> pair) {

  lookup.insert(pair);
}

std::map<std::string, Node*> FunctionMap::remove(std::string name) {

  //make a map to store the pairs to return
  std::map<string,Node*> pairs;

  //make a vector to store keys to remove
  std::vector<std::string> keys;

  // find the functions
  for (std::map<std::string, Node*>::iterator it = lookup.begin(); 
      it != lookup.end(); it ++){

    //check for a name match
    if (name == it->first.substr(0, 
          (it->first).find_first_of("("))) { 
      //add it to the return list
      pairs.insert(*it);

      //add the old key to a list for removal
      keys.push_back(it->first);
    }
  }

  //remove all the old keys
  for (unsigned long i = 0; i<keys.size(); i++){
    lookup.erase(keys[i]);
  }

  return pairs;
}

bool FunctionMap::hasFuncNamed(std::string name) {
  //loop through all elements in the map
  for (std::map<std::string, Node*>::iterator it = lookup.begin(); 
      it != lookup.end(); it ++){

    //check for a name match
    if (name == it->first.substr(0, 
          (it->first).find_first_of("("))) { 

      return true;
    }
  }
  //if we got through the list and didn't find a name match
  return false;
}

DataType* FunctionMap::getFunctionsNamed(std::string name) {

  //create a dataType to return
  DataType* retType = new DataType(TYPE_OVERLOAD);  

  //loop through all elements in the map
  for (std::map<std::string, Node*>::iterator it = lookup.begin(); 
      it != lookup.end(); it ++) {

    //check for a name match
    if (name == it->first.substr(0, 
          (it->first).find_first_of("("))) { 

      //if one was found, add it to the subtypes
      retType->subtypes->push_back(*(it->second->type()));
    }
  }
  //If there are no subtypes, return null
  if (retType->subtypes->size() == 0) {
    retType = NULL;
  //If there is one, just return that one
  } else if (retType->subtypes->size() == 1) {
    retType = &((*(retType->subtypes))[0]);
  }
  return retType;
}

bool FunctionMap::hasFunction(Node* node) {
  return lookup.count(getFunctionSignature(node));
}

bool FunctionMap::hasFunction(DataType* type, std::string name) {
  return lookup.count(name+typeToString(type));
}

void optimizeFunction(Node* base, Node** funcs,
                      std::map<std::string, Node*>& lookup) {
  if (base->kind() == NODE_FUNCALL) {
    for (unsigned int index = 0; index < lookup.size(); index++) {
      std::string name = base->getString();
      // These if statements provide optimization for TSL built in functions
      // These assigned values are matched in the evaluateFunction method of the
      // interpreter
      if (name == "print") {
        base->setIntval(0);
      } else if (name == "read_int") {
        base->setIntval(1);
      } else if (name == "read_real") {
        base->setIntval(2);
      } else if (name == "read_string") {
        base->setIntval(3);
      } else if (name == "read_bool") {
        base->setIntval(4);
      } else if (name == "len") {
        base->setIntval(5);
      } else if (lookup[FunctionMap::getFunctionSignature(base)] ==
                 funcs[index]) {
        // The earliest numbers are reserved for Tetra Standard Library
        // Functions
        // Integers for user-defined functions will start with the number after
        // the last TSL number
        base->setIntval(index + TSL_FUNCS);
      }
    }
  }

  for (int index = 0; index < base->numChildren(); index++) {
    optimizeFunction(base->child(index), funcs, lookup);
  }
}

void FunctionMap::optimizeFunctionLookup(Node* start) {
  // Now that all of the functions have been found, we can assign them numbers
  int numFuncs = lookup.size();
  functionLookup = new Node*[numFuncs];
  int count = 0;
  for (std::map<std::string, Node*>::iterator iter = lookup.begin();
       iter != lookup.end(); iter++) {
    functionLookup[count] = iter->second;
    count++;
  }

  optimizeFunction(start, functionLookup, lookup);
  // Then we will go through again and assign numbers to all the function call
  // nodes
}

// Fills the numerical field of each variable node to a value referencing where
// it will be held in the variable scope table
void optimize(Node* node, std::map<std::string, int>& refer, int& nextNum,
              std::map<std::string, int>& globRefer) {
  // If the node is a variable identifier, give it a numerical value
  if (node->kind() == NODE_IDENTIFIER || node->kind() == NODE_DECLARATION) {
    std::string name = node->getString();

    // Check if it is a global variable
    if (globRefer.find(name) != globRefer.end()) {
      // negative numbers denote that the variable is in the global scope
      // The negative will of course be removed when actual lookup occurs
      node->setIntval(-1 * globRefer[name]);
      // cout <<name<<": "<<globRefer[name]<<endl;
    } else {
      int possibleRef = refer[name];
      // If no numerical value exists, assign it one.
      // Note that this system wastes the 0th position,
      // because there is no easy way to differentiate between "0 is the
      // identifier" and "0 was returned when the element was default
      // constructed"
      if (possibleRef == 0) {
        possibleRef = nextNum;
        refer[name] = possibleRef;
        nextNum++;
      }
      node->setIntval(possibleRef);
      // cout << node->getString() << ": "<<possibleRef<<endl;
    }
  }

  for (int index = 0; index < node->numChildren(); index++) {
    optimize(node->child(index), refer, nextNum, globRefer);
  }
}

// Edits the base tree such that variable ID nodes will have their integer field
// set
// This field will allow for immediate lookup of the variable, as opposed to
// having to perform string comparison
void FunctionMap::optimizeLookup(const Node* start) {
  std::map<std::string, int> globRef;
  int nextGlob = 1;

  // Look through the NODE_TOPLEVEL_LIST nodes to register globals and gobal
  // consts
  while (start != NULL) {
    if (start->child(0)->kind() == NODE_CONST ||
        start->child(0)->kind() == NODE_GLOBAL) {
      // Get the identifier, which is to the first child of a CONST/GLOBAL node
      Node* cand = start->child(0)->child(0);
      // cout << cand->getString() << ":: " << nextGlob << endl;
      cand->setIntval(nextGlob);
      globRef[cand->getString()] = nextGlob;
      // If debugging is on, register the global variable for the debugger
      // EDIT: added to TetraContext::initializeGlobalVars
      /*if(TetraEnvironment::isDebugMode()) {
      //This function is a friend function of the TetraEnvironment
      cout << "Registered global: " << cand->getString() <<" at " <<nextGlob << endl;
      TetraEnvironment::getObserver().globRefTable[cand->getString()] = nextGlob;

      }*******/  //////
      nextGlob++;
      // We must check the whole tree now for references to other global vars
      optimize(start->child(0), globRef, nextGlob, globRef);
      // optimize(start->child(0),globRef,nextGlob,globRef);
    }
    start = start->child(1);
  }

  // typedef std::pair<const std::string, Node*> mapElem;
  for (std::map<const string, Node*>::iterator searcher =
           lookup.begin();
       searcher != lookup.end(); searcher++) {
    std::map<std::string, int> refer;
    int nextNum = 1;
    optimize(searcher->second, refer, nextNum, globRef);
  }
}

// Given a NODE_ACTUAL_PARAM or NODE_FORMAL_PARAM, adds the signature of a
// single argument to the string
// Then recursively calls this function until it has assembled the entire
// signature
void FunctionMap::concatSignature(const Node* node, string& signature) {
  if (node->kind() != NODE_ACTUAL_PARAM_LIST &&
      node->kind() != NODE_FORMAL_PARAM_LIST) {
    assert(node->type() != NULL);

    // Given the type of the next argument, append the appropriate value to the
    // signature
    DataType* argType = node->type();
    switch (argType->getKind()) {
      case TYPE_INT:
        signature += "_I";
        break;
      case TYPE_REAL:
        signature += "_R";
        break;
      case TYPE_BOOL:
        signature += "_B";
        break;
      case TYPE_STRING:
        signature += "_S";
        break;
      case TYPE_VECTOR:
        signature += "_V";
      case TYPE_MUTEX:
        signature += "_M";
        break;
      case TYPE_TASK:
        signature += "_T";
        break;

        // Must also fill in subtype information
        // While loop allows for accounting for vectors containing vectors
        // (containing vectors...)
        while (argType->subtypes->size()) {
          argType = &((*(argType->subtypes))[0]);
          switch (argType->getKind()) {
            case TYPE_INT:
              signature += "I";
              break;
            case TYPE_REAL:
              signature += "R";
              break;
            case TYPE_BOOL:
              signature += "B";
              break;
            case TYPE_STRING:
              signature += "S";
              break;
            case TYPE_VECTOR:
              signature += "V";
              break;
            case TYPE_MUTEX:
              signature += "M";
              break;
            case TYPE_TASK:
              signature += "T";
              break;
            default:
              std::stringstream message;
              message << "Error, unknown nodekind encountered in function "
                         "signature."
                      << std::endl;
              SystemError e(message.str(), node->getLine(), node);
              throw e;
          }
        }
        break;
      default:
        std::stringstream message;
        message << "Error, unknown nodekind encountered in function signature."
                << std::endl;
        SystemError e(message.str(), node->getLine(), node);
        throw e;
    }
  } else {
    concatSignature(node->child(0), signature);
    if (node->numChildren() == 2) {
      concatSignature(node->child(1), signature);
    }
  }
}

// Given a NODE_FUNCTION (seen by the build method) or NODE_FUNCALL (seen at
// runtime)
// Assembles the function signature for the function
string FunctionMap::getFunctionSignature(const Node* node) {

  return node->getString() + typeToString(&((*(node->type()->subtypes))[0]));
}

// Delete the functionLookup table
void FunctionMap::cleanup() { delete[] functionLookup; }
