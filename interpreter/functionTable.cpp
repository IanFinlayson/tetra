/*
 * This file builds a function lookup table so that when the interpreter encounters a function call, it can easily find the address of the appropriate node where the called function code resides
 *Since there is only one funciton table per program (even if using multiple files, the further functions should be addable by calling buildTree for each file's syntax tree) This uses a single object design.
 */

#include "functionTable.h"
#include <map>
#include <iostream>
#include <frontend.hpp>
#include <string>

using std::string;

FunctionMap::FunctionMap() {
}

Node* FunctionMap::getFunctionNode(string functionSignature) {
	return instance.lookup[functionSignature];
}


//Fills the function map given the specified base node
void FunctionMap::build(Node* tree) {

	if(tree->kind() == NODE_FUNCTION_LIST) {
		//by specifications, there MUST be a child
		Node* candidate = tree->child(0);
		instance.lookup[candidate->getString()] = candidate;
	
		//checks if there are further functions to add
		if(tree->child(1) != NULL) {
			build(tree->child(1));
		}   
	} 
}

//initializes static function map
FunctionMap FunctionMap::instance;
