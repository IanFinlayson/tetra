/*
 * This file is used for analyzing nodes within a program
 */

#include <string>
#include <iostream>
#include "frontend.hpp"
#include <cstdlib>

using std::cout;
using std::endl;
using std::string;

void descNode(Node* n) {
	cout << "--------" << endl;
	cout << "Line: " << n->getLine() << endl;
	cout << "Stringval: " << n->getString() << endl;
	cout << "Intval: " << n->getInt() << endl;
	cout << "Realval: " << n->getReal() << endl;
	cout << "Boolval: " << n->getBool() << endl;
	cout << "NodeKind: " << n->kind() << endl;
	cout << "DataType: " << ((n->type() == NULL)?"No type":typeToString(n->type())) << endl;
	cout << "Children: " << n->numChildren() << endl;
	//cout << "x: " << n->hasSymbol("x") <<endl;
}
											

void outputTree(Node* node) {
	descNode(node);
	int numChildren = node->numChildren();
	for(int index = 0; index < numChildren; index++) {
		outputTree(node->child(index));
	}
}


int main(int argc, char** argv) {

	//check that the proper commands were passed
	if(argc < 2) {
		cout << "Please pass a file name!" << endl;
		return 0;
	}   

	Node* tree;

	try {
		tree = parseFile(argv[1]);
	} catch(Error e) {
		cout << "The following error was detected in your program:\n" << e << "\nExecution aborted" <<endl; 
		exit(EXIT_FAILURE);
	}   

	outputTree(tree);
}

