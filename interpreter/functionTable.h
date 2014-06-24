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
	static void concatSignature(const Node*,string&);

public:
	//Returns the address of a node containing the function body of the function denoted by functionSignature
	static const Node* getFunctionNode(const string functionSignature);

	//Generates a unique function signature based on the name AND the arguments
	static const string getFunctionSignature(const Node* node);

	//Fills the function map given the specified base node
	static void build(const Node* tree);
};

#endif
