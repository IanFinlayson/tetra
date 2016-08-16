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

#include "tetra.h"

// Given a function signature, returns the adress of a node containing the
// function definition for that signature
const Node* FunctionMap::getFunctionNode(const tstring functionSignature) {
  // if function is not there, will return default Node* (i.e. NULL)
  return lookup[functionSignature];
}

const Node* FunctionMap::getFunctionNode(DataType* params, tstring name) {
  return lookup[name + typeToString(params)];
}

//lookup function node from funcall node
const Node* FunctionMap::getFunctionNode(const Node* callNode) {
  return lookup[getFunctionSignature(callNode)];
}

FunctionMap::FunctionMap() {
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
      //populate the symtable and update the datatype
      inferParams(candidate);
      
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
void FunctionMap::insert(std::pair<tstring,Node*> pair) {

  lookup.insert(pair);
}

/* clear all functions from the map */
void FunctionMap::clearAll() {
    lookup.clear();
}

std::map<tstring, Node*> FunctionMap::remove(tstring name) {

  //make a map to store the pairs to return
  std::map<tstring,Node*> pairs;

  //make a vector to store keys to remove
  std::vector<tstring> keys;

  // find the functions
  for (std::map<tstring, Node*>::iterator it = lookup.begin(); 
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

bool FunctionMap::hasFuncNamed(tstring name) {
  //loop through all elements in the map
  for (std::map<tstring, Node*>::iterator it = lookup.begin(); 
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

DataType* FunctionMap::getFunctionsNamed(tstring name) {

  //create a dataType to return
  DataType* retType = new DataType(TYPE_OVERLOAD);  

  //loop through all elements in the map
  for (std::map<tstring, Node*>::iterator it = lookup.begin(); 
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

bool FunctionMap::hasFunction(DataType* type, tstring name) {
  return lookup.count(name+typeToString(type));
}

// Given a NODE_FUNCTION (seen by the build method) or NODE_FUNCALL (seen at
// runtime)
// Assembles the function signature for the function
tstring FunctionMap::getFunctionSignature(const Node* node) {
  if (node->kind() == NODE_FUNCTION) {
    return node->getValue()->toString() + typeToString(&((*(node->type()->subtypes))[0]));
  } else if (node->kind() == NODE_FUNCALL ) {
    //get the params
    DataType* params = new DataType(TYPE_TUPLE); 
    if (node->numChildren() > 1) {
      buildParamTupleType(params,node->child(1));
    }

    return node->child(0)->getValue()->toString() + typeToString(params); 
  }
  throw Error("Cannot create Function signature.");
}
