/*
 * This file builds a function lookup table so that when the interpreter encounters a function call, it can easily find the address of the appropriate node where the called function code resides
 *Since there is only one funciton table per program (even if using multiple files, the further functions should be addable by calling buildTree for each file's syntax tree) This uses a single object design.
 */

//#include "functionTable.h"
#include "backend.hpp"
#include <map>
#include <iostream>
#include <frontend.hpp>
#include <string>
#include <cstdlib>
#include <sstream>
//#define NDEBUG
#include <assert.h>

using std::string;

FunctionMap::FunctionMap() {
}

//Given a function signature, returns the adress of a node containing the function definition for that signature
const Node* FunctionMap::getFunctionNode(const string functionSignature) {
	//if function is not there, will return default Node* (i.e. NULL)
	return instance.lookup[functionSignature];
}


//Fills the function map given the specified base node
void FunctionMap::build(const Node* tree) {
	
	if(tree->kind() == NODE_TOPLEVEL_LIST) {

		//by frontend specifications, there MUST be a child to add
		Node* candidate = tree->child(0);
		if(candidate->kind() == NODE_FUNCTION) {
			instance.lookup[getFunctionSignature(candidate)] = candidate;
		}
		//checks if there are further functions to add
		if(tree->child(1) != NULL) {
			build(tree->child(1));
		}   
	}
}

//Fills the numerical field of each variable node to a value referencing where it will be held in the variable scope table
void optimize(Node* node, std::map<std::string,int>& refer, int& nextNum) {

	//If the node is a variable identifier, give it a numerical value
	if(node->kind() == NODE_IDENTIFIER || node->kind() == NODE_FORMAL_PARAM) {
		int possibleRef = refer[node->getString()];
		//If no numerical value exists, assign it one.
		//Note that this system wastes the 0th position,
		//because there is no easy way to differentiate between "0 is the identifier" and "0 was returned when the element was default constructed"
		if(possibleRef == 0) {
			possibleRef = nextNum;
			refer[node->getString()] = possibleRef;
			nextNum++;
		}
		node->setIntval(possibleRef);
		//cout << node->getString() << ": "<<possibleRef<<endl;
	}

	for(int index = 0; index < node->numChildren(); index++) {
		optimize(node->child(index), refer, nextNum);
	}
}

//Edits the base tree such that variable ID nodes will have their integer field set
//This field will allow for immediate lookup of the variable, as opposed to 
//having to perform string comparison
void FunctionMap::optimizeLookup() {
	//typedef std::pair<const std::string, Node*> mapElem;
	for(std::map<const string, Node*>::iterator searcher = instance.lookup.begin(); searcher != instance.lookup.end(); searcher++) {
		std::map<std::string,int> refer;
		int nextNum = 1;
		optimize(searcher->second, refer, nextNum);
	}
}

//Given a NODE_ACTUAL_PARAM or NODE_FORMAL_PARAM, adds the signature of a single argument to the string
//Then recursively calls this function until it has assembled the entire signature
void FunctionMap::concatSignature(const Node* node, string& signature) {
 
	if(node->kind() != NODE_ACTUAL_PARAM_LIST && node->kind() != NODE_FORMAL_PARAM_LIST) {

		assert(node->type() != NULL);

		//Given the type of the next argument, append the appropriate value to the signature
		DataType* argType = node->type();
		switch(argType->getKind()) {
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
				//Must also fill in subtype information
				//While loop allows for accounting for vectors containing vectors (containing vectors...)
				while(argType->getSub() != NULL) {
					argType = argType->getSub();
					switch(argType->getKind()) {
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
						default:
							std::stringstream message;
							message << "Error, unknown nodekind encountered in function signature." << std::endl;
							SystemError e(message.str(),node->getLine(),node);
							throw e;
					}
				}
			break;
			default:
				std::stringstream message;
				message << "Error, unknown nodekind encountered in function signature." << std::endl;
				SystemError e(message.str(),node->getLine(),node);
				throw e;
		}
	}
	else {
		concatSignature(node->child(0), signature);
		if(node->numChildren() == 2) {
			concatSignature(node->child(1), signature);
		}
	}

}

//Given a NODE_FUNTION (seen by the build method) or NODE_FUNCALL (seen at runtime)
//Assembles the function signature for the function
const string FunctionMap::getFunctionSignature(const Node* node) {
	string ret = node->getString();
	
	assert (node->kind() == NODE_FUNCTION || node->kind() == NODE_FUNCALL);

	ret += "#";
	// This symbol cannot be in a funciton name (since it denotes a comment
	//Hence, we use it so that a user-defined function name does not accidentally align with a signature of a function

	//If there are arguments, add them to the signature	
	//Reminder that a NODE_FUNCTION should only have arguments if there are 2  children
	//A NODE_FUNCALL should have arguments if it has any children
	if((node->kind() == NODE_FUNCTION && node->numChildren() == 2) || (node->kind() == NODE_FUNCALL && node->child(0) != NULL)) {
		concatSignature(node->child(0), ret);
	}

	return ret;
}

	

//initializes single static function map instance
FunctionMap FunctionMap::instance;
