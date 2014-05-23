/*
 * This file builds a function lookup table so that when the interpreter encounters a function call, it can easily find the address of the appropriate node where the called function code resides
 */

#include<map>
#include<iostream>
#include<frontend.hpp>
#include<string>

using std::string;

class functionMap {

public:
	functionMap(Node* tree) {
		buildFunctionMap(tree);
	}

	Node* getFunctionNode(string functionSignature) {
		return lookup[functionSignature];
	}

private:

	std::map<string, Node*> lookup;

	//Fills the function map given the specified base node
	void buildFunctionMap(Node* tree) {

		if(tree->kind() == NODE_FUNCTION_LIST) {
			//by specifications, there MUST be a child
			Node* candidate = tree->child(0);
			lookup[candidate->getString()] = candidate;

			//checks if there are further functions to add
			if(tree->child(1) != NULL) {
				buildFunctionMap(tree->child(1));
			}   
		} 
	}
};
