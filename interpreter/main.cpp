/*
 * This program interprets a tetra file by getting its tree representation
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include "frontend.hpp"
#include "functionTable.cpp"

using namespace std;
/*
string translateType(Datatype* type) {
	x = type->getKind
	if(*x == TYPE_INT) 
		return "INT";
	if(*x == TYPE_REAL)
		return "REAL";
	if(*x == TYPE_STRING)
		return "STRING";
	if(*x == TYPE_BOOL)
		return "BOOL";
	if(*x == TYPE_VECTOR)
		return "VEC<"+x->getSub()+">";
	if(*x == TYPE_VOID)
		return "VOID"
	return "UNKNOWN";

}*/

void descNode(Node* n) {
	cout << "--------" << endl;
	cout << n->getLine() << endl;
	cout << n->getString() << endl;
	cout << n->getInt() << endl;
	cout << n->getReal() << endl;
	cout << n->getBool() << endl;
	cout << n->kind() << endl;
	//cout << typeToString(n->type()) << endl;
	cout << n->numChildren() << endl;
	//cout << "x: " << n->hasSymbol("x") <<endl;
}

//executes generic statements
Node* evaluateStatement(Node* node) {
	return NULL;
}

//calls a function, returns the return value
Node* evaluateFunction(Node* node) {
	return NULL;
}

//evaluates operations on data types
Node* evaluateExpression(Node* node) {
	return NULL;
}

//evaluates boolean operations
Node* evaluateCondition(Node* node) {
	return NULL;
}

//used to accomodate unsupported features during development. Evaluates children while ignoring the node itself
Node* skip(Node* node) {
	return NULL;
}
/*
void buildFunctionTable(Node* tree) {
	//buildFunctionTable(tree->child(x));

	//finds all the funcitons
	if(tree->kind() == NODE_FUNCTION_LIST) {
		descNode(tree->child(0));
		if(tree->child(1) != NULL) {
			buildFunctionTable(tree->child(1));
		}
	}
	
}
*/
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

	functionMap funcTable(tree);

	descNode(funcTable.getFunctionNode("for_test")->child(0));

	cout << "Running " << argv[1] << "..." << endl;

	

	return 0;
}

