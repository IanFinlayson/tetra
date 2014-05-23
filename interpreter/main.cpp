/*
 * This program interprets a tetra file by getting its tree representation
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include "frontend.hpp"
#include "functionTable.cpp"
#include "nodeTable.cpp"
#include "operationMap.cpp"

using namespace std;

void evaluateNode(Node*,void*);
void runTest();//function that contains test code

//used for debugging purposes
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
//executes structural statements
void evaluateStatement(Node* node, void* ret) {
//	return NULL;
}

//calls a function, returns the return value
void evaluateFunction(Node* node, void* ret) {
//	return NULL;
}

//evaluates operations on data types
void evaluateExpression(Node* node, void* ret) {

//	void* ret = NULL;//this is what we will return

	
	static operationList<int> execInt;
	//static operationList<double> execReal;
	static operationList<bool> execBool;
	static operationList<string> execString;

//	DataTypeKind retType = (node->type()->getKind());

	switch(node->kind()) {
		default:


			/*This whole switch statement is actually very dangerous at the moment (pointers to local vars, oops), will change
			 */

			switch(node->child(0)->type()->getKind())
			{
				case TYPE_INT:
				{
					//fetch operands
					int op1;
					evaluateNode(node->child(0),&op1);
					int op2;
					evaluateNode(node->child(1),&op2);
					*static_cast<int*>(ret) = execInt.execute(node->kind(),op1,op2);
				}
				break;
				case TYPE_REAL:
				{
					//fetch operands
					double op1;
					evaluateNode(node->child(0),&op1);
					double op2;
					evaluateNode(node->child(1),&op2);
					*static_cast<double*>(ret) = execInt.execute(node->kind(),op1,op2);
				}
				break;
				case TYPE_BOOL:
				{
					//fetch operands
					bool op1;
					evaluateNode(node->child(0),&op1);
					bool op2;
					evaluateNode(node->child(1),&op2);
					*static_cast<bool*>(ret) = execBool.execute(node->kind(),op1,op2);
				}
				break;
				case TYPE_STRING:
				{
					//fetch operands
					string op1;
					evaluateNode(node->child(0),&op1);
					string op2;
					evaluateNode(node->child(1),&op2);
					*static_cast<string*>(ret) = execString.execute(node->kind(),op1,op2);
				}
				break;
				default:
					;//do zilch
			}
	}


	//return NULL;

}

//evaluates boolean operations
void evaluateCondition(Node* node, void* ret) {
//	return NULL;
}

//Evaluates immediate expressions
//At present, the pointers created by this method need to be deleted by the caller
void evaluateImmediate(Node* node, void* ret) {


	if(node->kind() == NODE_IDENTIFIER) { //if we need to lookup a variable
		switch(node->type()->getKind()) {
		/*	case TYPE_INT:
				int* ret = new int;
				*ret = *();
			break;*/
			default:
				cout << "Unknown NodeDataType: " << node->type()->getKind() << " encountered.\nAborting...";
				exit(EXIT_FAILURE);
		}
	}

	else {
		switch(node->kind()) {
			case NODE_INTVAL:
				*static_cast<int*>(ret) = node->getInt();
			break;
			case NODE_REALVAL:
				*static_cast<double*>(ret) = node->getReal();
			break;
			case NODE_BOOLVAL:
				*static_cast<bool*>(ret) = node->getBool();
			break;
			case NODE_STRINGVAL:
				*static_cast<string*>(ret) = node->getString();
			break;
			default:
				cout << "Unknown Immediate NodeType: " << node->kind() << " encountered.\nAborting...";
				exit(EXIT_FAILURE);
		}
			
	}
}

//evaluates generic nodes
void evaluateNode(Node* node, void* ret) {
	switch(NodeTable::classifyNode(node)) {
		case CONDITION:
			evaluateCondition(node,ret);
		break;
		case CONTROL:
			evaluateFunction(node,ret);
		break;
		case IMMEDIATE:
			evaluateImmediate(node,ret);
		break;
		case OPERATION:
			evaluateExpression(node,ret);
		break;
		case STRUCTURE:
			evaluateStatement(node,ret);
		break;
		default:
			cout << "Warnming: unexpected node kind encountered: " << node->kind() << "\n Aborting..." << endl;
		exit(EXIT_FAILURE);
	}
}

//used to accomodate unsupported features during development. Evaluates children while ignoring the node itself
void skip(Node* node, void* ret) {
	//return ret;
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


	//NodeTable.initialize();
	functionMap funcTable(tree);

	//descNode(funcTable.getFunctionNode("for_test")->child(0));

	cout << "Running " << argv[1] << "..." << endl;

	runTest();

	return 0;
}

void runTest() {
/* PLEASE DON"T DELETE THIS TEST CODE!
 * it will be modified for further test cases
 */
	DataType* intVal;
	DataType intV = TYPE_INT;
	intVal = &intV;

	DataType* realVal;
	DataType realV = TYPE_REAL; 
	realVal = &realV;

	DataType* boolVal;
	DataType boolV = TYPE_BOOL; 
	boolVal = &boolV;

	DataType* stringVal;
	DataType stringV = TYPE_STRING; 
	stringVal = &stringV;

	Node testInt(NODE_INTVAL);
	testInt.setDataType(intVal);
	testInt.setIntval(27);

	Node testInt2(NODE_INTVAL);
	testInt2.setDataType(intVal);
	testInt2.setIntval(39);

	Node testBool(NODE_BOOLVAL);
	testBool.setDataType(boolVal);
	testBool.setBoolval(true);

	Node testReal(NODE_REALVAL);
	testReal.setDataType(realVal);
	testReal.setRealval(-13.6);

	Node testString(NODE_STRINGVAL);
	testString.setDataType(stringVal);	
	testString.setStringval("It works maybe!");

	int* i = new int;
	evaluateNode(&testInt,i);

	string* s  = new string;
	evaluateNode(&testString,s);

	double* d = new double;
	evaluateNode(&testReal,d);

	bool* b = new bool;
	evaluateNode(&testBool,b);
	
	cout << "Bool: " << *b << endl;
	cout << "Int: " << *i << endl;
	cout << "Real: " << *d << endl;
	cout << "String: " << *s << endl;

	Node testAdd(NODE_PLUS);
	testAdd.setDataType(intVal);
	testAdd.addChild(&testInt);
	testAdd.addChild(&testInt);

	Node testAdd2(NODE_PLUS);
	testAdd2.setDataType(intVal);
	testAdd2.addChild(&testAdd);
	testAdd2.addChild(&testInt2);

	int* a = new int;
	evaluateNode(&testAdd2,a);

	cout << "Int + Int: " << *a << endl;

	Node testAddS(NODE_PLUS);
	testAddS.setDataType(stringVal);
	testAddS.addChild(&testString);
	testAddS.addChild(&testString);

	string* s2 = new string;
	evaluateNode(&testAddS,s2);

	cout << "String + String: " << *s2 << endl;


}
