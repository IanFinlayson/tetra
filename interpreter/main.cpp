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
#include "comparisonMap.cpp"
#include "variableContext.cpp"

//temporary global varTable for testing purposes
VarTable vars;

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
	
}

//evaluates operations on data types
void evaluateExpression(Node* node, void* ret) {

//	void* ret = NULL;//this is what we will return

	//Static tables containing what actions to perform given which operation node	
	static OperationList<int> execInt;
	static OperationList<double> execReal;
	static OperationList<bool> execBool;
	static OperationList<string> execString;

//	DataTypeKind retType = (node->type()->getKind());

	switch(node->kind()) {
		default:

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
					*static_cast<double*>(ret) = execReal.execute(node->kind(),op1,op2);
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

//used to get references to variables (for assignment, and possibly other features in the future)
void evaluateAddress(Node* node, void* ret) {
	//Right now, we are assuming that the only type of node for which this will occur is a NODE_IDENTIFIER
	
	if(node->kind() == NODE_IDENTIFIER) {
		switch(node->type()->getKind()) {
			case TYPE_INT:
				*static_cast<int**>(ret) = vars.lookupVar<int>(node->getString());
			break;
			case TYPE_REAL:
				*static_cast<double**>(ret) = vars.lookupVar<double>(node->getString());
			break;
			case TYPE_BOOL:
				*static_cast<bool**>(ret) = vars.lookupVar<bool>(node->getString());
			break;
			case TYPE_STRING:
				*static_cast<string**>(ret) = vars.lookupVar<string>(node->getString());
			break;
			default:
				cout << "Unknown NodeDataType: " << node->type()->getKind() << " encountered.\nAborting...";
				exit(EXIT_FAILURE);
		}
	}

}

//At the moment, ret will point to whatever value was copied for the assignment
void performAssignment(Node* node, void* ret) {
	switch(node->type()->getKind()) {
		case TYPE_INT:
		{
			//fetch operands
			int* op1_ptr = NULL;//This SHOULD no longer be 0 once the lhs of evaluateNode.
			evaluateAddress(node->child(0),&op1_ptr);
			int op2;
			evaluateNode(node->child(1),&op2);
			*op1_ptr = op2;
			*(static_cast<int*>(ret)) = op2;
		}
		break;
		case TYPE_REAL:
		{
			//fetch operands
			double* op1_ptr = NULL;
			evaluateAddress(node->child(0),&op1_ptr);
			double op2;
			evaluateNode(node->child(1),&op2);
			*op1_ptr = op2;
			*(static_cast<double*>(ret)) = op2;
		}
		break;
		case TYPE_BOOL:
		{
			//fetch operands
			bool* op1_ptr = NULL;
			evaluateAddress(node->child(0),&op1_ptr);
			bool op2;
			evaluateNode(node->child(1),&op2);
			*op1_ptr = op2;
			*(static_cast<bool*>(ret)) = op2;
		}
		break;
		case TYPE_STRING:
		{
			//fetch operands
			string* op1_ptr = NULL;
			evaluateAddress(node->child(0),op1_ptr);
			string op2;
			evaluateNode(node->child(1),&op2);
			*op1_ptr = op2;
			*(static_cast<string*>(ret)) = op2;
		}
		break;
		default:
			cout << "Unknown assignment NodeType: " << node->kind() << " encountered.\nAborting...";
			exit(EXIT_FAILURE);
		
	}
}
//evaluates boolean operations
void evaluateCondition(Node* node, void* ret) {

	//Static tables containing what type of comparison to perform given which operation node	
	static ComparisonList<int> compInt;
	static ComparisonList<double> compReal;
	static ComparisonList<bool> compBool;
	static ComparisonList<string> compString;

	switch(node->kind()) {
		default:
			//Must check the type of the CHILD because this node itself has type bool (since its comparison)
			switch(node->child(0)->type()->getKind())
			{
				case TYPE_INT:
				{
					//fetch operands
					int op1;
					evaluateNode(node->child(0),&op1);
					int op2;
					evaluateNode(node->child(1),&op2);
					*static_cast<bool*>(ret) = compInt.execute(node->kind(),op1,op2);
				}
				break;
				case TYPE_REAL:
				{
					//fetch operands
					double op1;
					evaluateNode(node->child(0),&op1);
					double op2;
					evaluateNode(node->child(1),&op2);
					*static_cast<bool*>(ret) = compReal.execute(node->kind(),op1,op2);
				}
				break;
				case TYPE_BOOL:
				{
					//fetch operands
					bool op1;
					evaluateNode(node->child(0),&op1);
					bool op2;
					evaluateNode(node->child(1),&op2);
					*static_cast<bool*>(ret) = compBool.execute(node->kind(),op1,op2);
				}
				break;
				case TYPE_STRING:
				{
					//fetch operands
					string op1;
					evaluateNode(node->child(0),&op1);
					string op2;
					evaluateNode(node->child(1),&op2);
					*static_cast<bool*>(ret) = compString.execute(node->kind(),op1,op2);
				}
				break;
				default:
					cout << "Unknown NodeDataType: " << node->type()->getKind() << " encountered when attempting comparison.\nAborting...";
					exit(EXIT_FAILURE);
			}
	}

}

//Evaluates immediate expressions
//At present, the pointers created by this method need to be deleted by the caller
void evaluateImmediate(Node* node, void* ret) {


	//if we need to lookup a variable, do so, Note that "lookupVar" handles if we have not seen the string before
	//However, we must check the type so that we know what to allocate if needed (optimizaiton point for later?)
	if(node->kind() == NODE_IDENTIFIER) {
		switch(node->type()->getKind()) {
			case TYPE_INT:
				*static_cast<int*>(ret) = *(vars.lookupVar<int>(node->getString()));
			break;
			case TYPE_REAL:
				*static_cast<double*>(ret) = *(vars.lookupVar<double>(node->getString()));
			break;
			case TYPE_BOOL:
				*static_cast<bool*>(ret) = *(vars.lookupVar<bool>(node->getString()));
			break;
			case TYPE_STRING:
				*static_cast<string*>(ret) = *(vars.lookupVar<string>(node->getString()));
			break;
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
		case ASSIGNMENT:
			performAssignment(node,ret);
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
	cout << "super fin" << endl;
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
	testAdd2.setDataType(realVal);
	testAdd2.addChild(&testReal);
	testAdd2.addChild(&testReal);

	double* a = new double;
	evaluateNode(&testAdd2,a);

	cout << "double + double: " << *a << endl;

	Node testAddS(NODE_EQ);
	testAddS.setDataType(intVal);
	testAddS.addChild(&testInt);
	testAddS.addChild(&testInt2);

	bool* s2 = new bool;
	evaluateNode(&testAddS,s2);

	cout << "27 cmp 39: " << *s2 << endl;

	Node varX(NODE_IDENTIFIER);
	varX.setStringval("X");
	varX.setDataType(realVal);
	*(vars.lookupVar<double>("X")) = 15;

	Node assignX(NODE_ASSIGN);
	assignX.setDataType(realVal);
	assignX.addChild(&varX);
	assignX.addChild(&testAdd2);

	evaluateNode(&assignX,a);
	evaluateNode(&varX,a);
	cout << "X(?): " << *a << endl;
	cout << "fin" << endl;
}
