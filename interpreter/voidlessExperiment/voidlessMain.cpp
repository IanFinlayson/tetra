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
#include "voidlessOperationMap.cpp"
#include "voidlessComparisonMap.cpp"
#include "variableContext.cpp"
#include "voidlessTData.cpp"

//temporary global varTable for testing purposes
VarTable vars;

using namespace std;
//data wrapper class
/*
template<typename T>
class TData{
public:
	TData() {}

	TData(T pData) {
		data = pData;
	}

	void setData(T pData) {
		data = pData;
	}

	T getData() {
		return data;
	}
private:
	T data;
	
		
};*/

template <typename T>
T paste(Node*,Node*);
template <typename T>
void evaluateNode(Node*,TData<T>*);
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
//executes structural statementsi
template<typename T>
void evaluateStatement(Node* node, TData<T>* ret) {
	switch(node->kind()) {
		case NODE_FUNCTION:
			cout <<"here?!?!?" << endl;
			//If there were arguments, meaning that child(0) contains the body of code and child 1 is empty
			if(node->child(1) != NULL) {
				cout << "goin arglist" << endl;
				evaluateNode<T>(node->child(1),ret);
			} else {
				cout << "goin noargs" << endl;
				evaluateNode<T>(node->child(0),ret);
			}
			cout << "after statement" << endl;
		break;
		default:
			cout << "Unsupported nodekind encountered in statement" << endl;
	}
}

void pasteArgList(Node* node1, Node* node2) {
	//If there are further parameters to pass
	cout << "pastin arg list" << endl;
	if(node1->kind() == NODE_FORMAL_PARAM_LIST) { //must have 2 children, as does the corresponding tree of NODE_ACTUAL_PARAM_LIST
		pasteArgList(node1->child(0),node2->child(0));
		pasteArgList(node1->child(1),node2->child(1));
	}
	else {
		cout << "Pasting stuff!" << endl;
		switch(node1->type()->getKind()) {
			case TYPE_INT:
				cout << "ints specifically " << endl;
				 paste<int>(node1,node2);
			break;
			case TYPE_REAL:
				paste<double>(node1,node2);
			break;
			case TYPE_BOOL:
				paste<bool>(node1,node2);
			break;
			case TYPE_STRING:
				paste<string>(node1,node2);
			break;
			default:
				cout << "Unknown NodeDataType: " << node1->type()->getKind() << " encountered when attempting to paste arguments.\nAborting...";
				exit(EXIT_FAILURE);
			}
	}
}

//calls a function, returns the return value
template<typename T>
void evaluateFunction(Node* node, TData<T>* ret) {
	//gets the node where the body of the called funciton begins
	Node* callNode = FunctionMap::getFunctionNode(node->getString());

	//check if there are parameters to be passed, and do so if needed
	//This call will have arguments if and only if the calling node has children
cout << "___________________________________________"<< node->numChildren() << endl;	
	if(node->child(0) != NULL) {
		cout << "herely herely" << endl;
		pasteArgList(callNode->child(0),node->child(0));
	}

	//transfer control to the function
	evaluateNode<T>(callNode,ret);
}

//evaluates operations on data types
template <typename T>
void evaluateExpression(Node* node, TData<T>* ret) {

//	void* ret = NULL;//this is what we will return

	//Static tables containing what actions to perform given which operation node	
	static OperationList<T> execData;

//	DataTypeKind retType = (node->type()->getKind());

	TData<T> op1;
	evaluateNode<T>(node->child(0),&op1);
	TData<T> op2;
	evaluateNode<T>(node->child(1),&op2);
	ret->setData(execData.execute(node->kind(),&op1,&op2));

}

//used to get references to variables (for assignment, and possibly other features in the future)
template<typename T>
void evaluateAddress(Node* node, TData<T>* ret) {
	//Right now, we are assuming that the only type of node for which this will occur is a NODE_IDENTIFIER and NODE_FORMAL_PARAM
	
	//Must check type so we can cast the info from lookupVar to the appropriate type
	if(node->kind() == NODE_IDENTIFIER || node->kind() == NODE_FORMAL_PARAM) {
		switch(/*node->type()->getKind()*/TYPE_INT) {
			case TYPE_INT:
				 ret->setData(vars.lookupVar<int>(node->getString()));
			break;
			case TYPE_REAL:
				ret->setData(vars.lookupVar<double>(node->getString()));
			break;
			case TYPE_BOOL:
				ret->setData(vars.lookupVar<bool>(node->getString()));
			break;
			case TYPE_STRING:
				ret->setData(vars.lookupVar<string>(node->getString()));
			break;
			default:
				cout << "Unknown NodeDataType: " << node->type()->getKind() << " encountered.\nAborting...";
				exit(EXIT_FAILURE);
		}
	}

}

//puts the value of op2 into the address denoted by op1
//used for performing assignment and copying argument lists
//Returns the value of the assignment, with an eye towards chaining assignments (x = y = 2)
//Assumes that node1 is a NODE_IDENTIFIER, and node2 is an appropriate value
template<typename T>
T paste(Node* node1, Node* node2) {
	cout << "Pastin" << endl;
	TData<T*> op1;
	TData<T> op2;
	evaluateAddress<T*>(node1,&op1);
	evaluateNode<T>(node2,&op2);
	cout << op1.getData() << " -after pastin- " << op2.getData() << endl;	
	T ret = op2.getData();
	*(op1.getData()) = ret;
	return ret;
}


//At the moment, ret will point to whatever value was copied for the assignment
template<typename T>
void performAssignment(Node* node, TData<T>* ret) {

	cout << "Assigning!" << endl;
	//We must typecheck here because parent node does not know what we are trying to assign
	switch(node->type()->getKind()) {
		case TYPE_INT:
			//Performs the assignment and sets up the return
			cout << "inting" << endl;
			ret->setData(paste<int>(node->child(0), node->child(1)));
			cout << "afterint" << endl;
		break;
		case TYPE_REAL:
			ret->setData(paste<double>(node->child(0), node->child(1)));
		break;
		case TYPE_BOOL:
			ret->setData(paste<bool>(node->child(0), node->child(1)));
		break;
		case TYPE_STRING:
			ret->setData(paste<string>(node->child(0), node->child(1)));
		break;
		default:
			cout << "Unknown NodeDataType: " << node->type()->getKind() << " encountered.\nAborting...";
			exit(EXIT_FAILURE);
	}
	cout << "After switch" << endl;
	/*
	switch(node->type()->getKind()) {
		case TYPE_INT:
		{
			//fetch operands
			TData<int*> op1;//This SHOULD no longer be 0 once the lhs of evaluateNode.
			evaluateAddress<int*>(node->child(0),&op1);
			TData<int> op2;
			evaluateNode<int>(node->child(1),&op2);
			*(op1.getData()) = op2.getData();
			ret->setData(op2);
		}
		break;
		case TYPE_REAL:
		{
			//fetch operands
			TData<double*> op1;
			evaluateAddress<double*>(node->child(0),&op1);
			TData<double> op2;
			evaluateNode<double>(node->child(1),&op2);
			*(op1.getData()) = op2.getData();
			ret->setData(op2);
		}
		break;
		case TYPE_BOOL:
		{
			//fetch operands
			TData<bool*> op1;
			evaluateAddress<bool*>(node->child(0),&op1);
			TData<bool> op2;
			evaluateNode<bool>(node->child(1),&op2);
			*(op1.getData()) = op2.getData();
			ret->setData(op2);
		}
		break;
		case TYPE_STRING:
		{
			//fetch operands
			TData<string*> op1;
			evaluateAddress<string*>(node->child(0),&op1);
			TData<string> op2;
			evaluateNode<string>(node->child(1),&op2);
			*op1.getData() = op2.getData();
			ret->setData(op2);
		}
		break;
		default:
			cout << "Unknown assignment NodeType: " << node->kind() << " encountered.\nAborting...";
			exit(EXIT_FAILURE);
		
	}*/
}
//evaluates boolean operations
template<typename T>
void evaluateCondition(Node* node, TData<T>* ret) {

	//Static tables containing what type of comparison to perform given which operation node	
	static ComparisonList<int> compInt;
	static ComparisonList<double> compReal;
	static ComparisonList<bool> compBool;
	static ComparisonList<string> compString;


		//fetch operands, must perform type checking here, as the parent node has absolutely no idea what is being compared
	switch(node->child(0)->type()->getKind()) {
		case TYPE_INT:
		{
			TData<int> op1;
			evaluateNode<int>(node->child(0),&op1);
			TData<int> op2;
			evaluateNode<int>(node->child(1),&op2);
			ret->setData(compInt.execute(node->kind(),&op1,&op2));
		}
		break;
		case TYPE_REAL:
		{
			TData<double> op1;
			evaluateNode<double>(node->child(0),&op1);
			TData<double> op2;
			evaluateNode<double>(node->child(1),&op2);
			ret->setData(compReal.execute(node->kind(),&op1,&op2));
		}
		break;
		case TYPE_BOOL:
		{
			TData<bool> op1;
			evaluateNode<bool>(node->child(0),&op1);
			TData<bool> op2;
			evaluateNode<bool>(node->child(1),&op2);
			ret->setData(compBool.execute(node->kind(),&op1,&op2));
		}
		break;
		case TYPE_STRING:
		{
			TData<string> op1;
			evaluateNode<string>(node->child(0),&op1);
			TData<string> op2;
			evaluateNode<string>(node->child(1),&op2);
			ret->setData(compString.execute(node->kind(),&op1,&op2));
		}
		break;
		default:
			std::cout << "Error: unexpected NODE_DATA_TYPES begin compared" << std::endl;	
	}


}

//Evaluates immediate expressions
//At present, the pointers created by this method need to be deleted by the caller
template<typename T>
void evaluateImmediate(Node* node, TData<T>* ret) {


	//if we need to lookup a variable, do so, Note that "lookupVar" handles if we have not seen the string before
	//However, we must check the type so that we know what to allocate if needed (optimizaiton point for later?)
	if(node->kind() == NODE_IDENTIFIER) {
		ret->setData(*(vars.lookupVar<T>(node->getString())));
	}

	else {
		switch(node->kind()) {
			case NODE_INTVAL:
				ret->setData(node->getInt());
			break;
			case NODE_REALVAL:
				ret->setData(node->getReal());
			break;
			case NODE_BOOLVAL:
				ret->setData(node->getBool());
			break;
			case NODE_STRINGVAL:
				ret->setData(node->getString());
			break;
			default:
				cout << "Unknown Immediate NodeType: " << node->kind() << " encountered.\nAborting...";
				exit(EXIT_FAILURE);
		}
			
	}
}

//evaluates generic nodes
template<typename T>
void evaluateNode(Node* node, TData<T>* ret) {
	cout << "_-_-_-_-_-_-_-_-_-\n";
	descNode(node);
	cout << "_-_-_-_-_-_-_-_-_-_-" << endl;
	switch(NodeTable::classifyNode(node)) {
		case CONDITION:
			evaluateCondition<T>(node,ret);
		break;
		case CONTROL:
			evaluateFunction<T>(node,ret);
		break;
		case IMMEDIATE:
			evaluateImmediate<T>(node,ret);
		break;
		case OPERATION:
			evaluateExpression<T>(node,ret);
		break;
		case STRUCTURE:
			evaluateStatement<T>(node,ret);
		break;
		case ASSIGNMENT:
			performAssignment<T>(node,ret);
			cout << "after assignment evaluation" << endl;
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
	FunctionMap::build(tree);

	Node* start = FunctionMap::getFunctionNode("main");
	descNode(start);
	descNode(start->child(0));
	TData<int> retVal(0);

	cout << "Running " << argv[1] << "..." << endl;

	evaluateNode<int>(start,&retVal);
	
	cout << "hereMain" << endl;

	//runTest();
	cout << "super fin" << endl;
	return 0;
}

void runTest() {
/* PLEASE DON"T DELETE THIS TEST CODE!
 * it will be modified for further test cases
 */

	cout << "\n Running tests..." << endl;

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

	TData<int> i(0);
	evaluateNode<int>(&testInt,&i);

	TData<string> s("");
	evaluateNode<string>(&testString,&s);

	TData<double> d(0);
	evaluateNode<double>(&testReal,&d);

	TData<bool> b(false);
	evaluateNode<bool>(&testBool,&b);
	
	cout << "Bool: " << b.getData() << endl;
	cout << "Int: " << i.getData() << endl;
	cout << "Real: " << d.getData() << endl;
	cout << "String: " << s.getData() << endl;

	Node testAdd(NODE_PLUS);
	testAdd.setDataType(intVal);
	testAdd.addChild(&testInt);
	testAdd.addChild(&testInt);

	Node testAdd2(NODE_PLUS);
	testAdd2.setDataType(realVal);
	testAdd2.addChild(&testReal);
	testAdd2.addChild(&testReal);

	TData<double> a;
	evaluateNode<double>(&testAdd2,&a);

	cout << "double + double: " << a.getData() << endl;

	Node testAddS(NODE_EQ);
	testAddS.setDataType(boolVal);
	testAddS.addChild(&testInt);
	testAddS.addChild(&testInt2);

	TData<bool> s2(false);
	evaluateNode<bool>(&testAddS,&s2);

	cout << "27 cmp 39: " << s2.getData() << endl;

	Node varX(NODE_IDENTIFIER);
	varX.setStringval("GHRUAAAHHH");
	varX.setDataType(stringVal);
	*(vars.lookupVar<string>("GHRUAAAHHH")) = "oink";

	Node assignX(NODE_ASSIGN);
	assignX.setDataType(stringVal);
	assignX.addChild(&varX);
	assignX.addChild(&testString);

	evaluateNode<string>(&assignX,&s);
	evaluateNode<string>(&varX,&s);
	cout << "X(?): " << s.getData() << endl;
	cout << "fin" << endl;
}
