/*
 * This program merely prints out node information about a tetra program tree in a depth-first search pattern
 */


#include <iostream>
#include "frontend.hpp"


void descNode(Node* n) {
	cout << "--------" << endl;
	cout << n->getLine() << endl;
	cout << n->getString() << endl;
	cout << n->getInt() << endl;
	cout << n->getReal() << endl;
	cout << n->getBool() << endl;
	cout << n->kind() << endl;
	cout << ((n->type() == NULL)?"No type":typeToString(n->type())) << endl;
        cout << n->numChildren() << endl;
        //cout << "x: " << n->hasSymbol("x") <<endl;
 }

void traverseTree(Node* node) {
	descNode(node);

	for(int x = 0; x < node->numChildren(); x++) {
		traverseTree(node->child(x));
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
	}

	
	traverseTree(tree);

	cout << "Running " << argv[1] << "..." << endl;



	return 0;
}

