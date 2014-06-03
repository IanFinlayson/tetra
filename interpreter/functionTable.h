/*
 * This file builds a function lookup table so that when the interpreter encounters a function call, it can easily find the address of the appropriate node where the called function code resides
 *Since there is only one funciton table per program (even if using multiple files, the further functions should be addable by calling buildTree for each file's syntax tree) This uses a single object design.
 */

#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include <map>
#include <iostream>
#include <frontend.hpp>
#include <string>

using std::string;

class FunctionMap {

private:
	FunctionMap();
	std::map<string, Node*> lookup;
	static FunctionMap instance;	

public:
	static Node* getFunctionNode(string functionSignature);

	//Fills the function map given the specified base node
	static void build(Node* tree);
};

#endif
