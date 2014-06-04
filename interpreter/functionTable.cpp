/*
 * This file builds a function lookup table so that when the interpreter encounters a function call, it can easily find the address of the appropriate node where the called function code resides
 *Since there is only one funciton table per program (even if using multiple files, the further functions should be addable by calling buildTree for each file's syntax tree) This uses a single object design.
 */

#include "functionTable.h"
#include <map>
#include <iostream>
#include <frontend.hpp>
#include <string>
#include <cstdlib>

//#define NDEBUG
#include <assert.h>

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
		instance.lookup[getFunctionSignature(candidate)] = candidate;
		cout << "Added: " << getFunctionSignature(candidate) << endl;	
		//checks if there are further functions to add
		if(tree->child(1) != NULL) {
			build(tree->child(1));
		}   
	} 
}

//Adds the signature of a single argument to the string
void FunctionMap::concatSignature(Node* node, string& signature) {
	 
	if(node->kind() != NODE_ACTUAL_PARAM_LIST && node->kind() != NODE_FORMAL_PARAM_LIST) {

		//cout << node->getInt() << endl;

		//assert(node->type() != NULL);

		switch(/*node->type()->getKind()*/TYPE_INT) {
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
			default:
				std::cout << "Error, unknown nodekind encountered in function signature. Aborting..." << std::endl;
				exit(EXIT_FAILURE);
		}
	}
	else {
		concatSignature(node->child(0), signature);
		if(node->numChildren() == 2) {
			concatSignature(node->child(1), signature);
		}
	}

}

string FunctionMap::getFunctionSignature(Node* node) {
	string ret = node->getString();
	
	assert (node->kind() == NODE_FUNCTION || node->kind() == NODE_FUNCALL);

	ret += "#";
	// This symbol cannot be in a funciton name (since it denotes a comment
	//Hence, we use it so that a user-defined function name does not accidentally align with a signature of a function

	//If there are arguments, addd them to the signature	
	if((node->kind() == NODE_FUNCTION && node->numChildren() == 2) || (node->kind() == NODE_FUNCALL && node->child(0) != NULL)) {
		concatSignature(node->child(0), ret);
	}

	return ret;
}

	

//initializes static function map
FunctionMap FunctionMap::instance;
