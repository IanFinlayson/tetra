/*
 * The classes contained in this file are used to keep track of certain properties of a program as it is being executed
 * These include the function call stack, the variables currently in scope, and where the programshould jump to should there be a break or continue statement
 * Although your standard running program utilizes one stack, this stack is represented via an object because if the eventual need for multiple call stacks through multithreading
 */


#include <stack>
#include <iostream>
#include <string>
#include "frontend.hpp"
#include "backend.hpp"
//#include "progContext.h"
//#include "functionTable.h"
//#include "tArray.h"
#include <list>

//#define NDEBUG
#include <assert.h>

TetraScope::TetraScope() {
	callNode = NULL;
	executionStatus = NORMAL;
}
//This embedded class represents the details of the present runtime environment, including the current VariableContext and loop depth
TetraScope::TetraScope(const Node * pCallNode)/* : executionStatus(NORMAL), callNode(pCallNode)*/ {
	callNode = pCallNode;
	executionStatus = NORMAL;
}
/*
//Wraps the varTable::declareReference
TData<void*>& TetraScope::declareReference(const string varName) {
	return varScope.declareReference(varName);
}
*/

std::list<std::pair<pthread_t,TData<void*> > >& TetraScope::declareThreadSpecificVariable(const std::string& name) {
	return varScope.declareParForVar(name);
}

//Used by loops and constrol statements to determine if they can proceed, or if they should return
ExecutionStatus TetraScope::queryExecutionStatus() {
	return executionStatus;
}

//Sets the execution status to the appropriate value
/*void TetraScope::setExecutionStatus(ExecutionStatus status) {
	executionStatus = status;
}*/

bool TetraScope::containsVar(std::string varName) const{
	return varScope.containsVar(varName);
}

bool TetraScope::containsVar(const Node* varNode) const{
	return varScope.containsVar(varNode);
}

void TetraScope::setCallNode(const Node* node) {
	callNode = node;
}

const Node* TetraScope::getCallNode() const {
	return callNode;
}


//-------------------------------------------------------------
//This class wraps a std stack of TetraScopes

//Forward declare evaluateNode for the initialization of global variables
template <typename T>
void evaluateNode(const Node*,TData<T>&,TetraContext&);

TetraContext::TetraContext() {
	//Initialize the global scope
	initializeNewScope(NULL);
	globalScope = &(progStack.top());
	threadID = -1;
}

TetraContext::TetraContext(long tID) {
	//initialize the global scope
	initializeNewScope(NULL);
	globalScope = &(progStack.top());
	threadID = tID;

	//Debug variables
	stopAtNext = false;
	stepping = false;
	resume = false;
	lastLineNo = -1;
}

void TetraContext::initializeGlobalVars(const Node * tree) {

	//Traverse the tree
	if(tree->kind() == NODE_TOPLEVEL_LIST) {

		Node* candidate = tree->child(0);
		if(candidate->kind() == NODE_GLOBAL || candidate->kind() == NODE_CONST) {
			//If debugging is enabled, register the name of the global in the global reference lookup table
			if(TetraEnvironment::isDebugMode()) {
				Node* id = candidate->child(0);
				globRefTable[id->getString()] = id->getInt();
			}
			
			//perform assignment at this global scope
			TData<int> dummy;
			evaluateNode<int>(candidate, dummy, *this);
		}

		if(tree->child(1) != NULL) {
			initializeGlobalVars(tree->child(1));
		}
	}
}

//Initializes an empty scope and sets that as the current scope
void TetraContext::initializeNewScope(const Node * callNode) {
	scope_ptr newScope(callNode);
	progStack.push(newScope);
}

//Takes the given scope and sets it as the current scope
//This allows local data to get passed in
void TetraContext::initializeNewScope(TetraScope& newScope) {
	//progStack.push(newScope);
	//cout << "Here we are!" << endl;
	//cout << "z: " << newScope.lookupVar<TArray>("z")->size() << endl;
	scope_ptr newScopePtr(newScope);
	progStack.push(newScopePtr); 
}

//This function takes the given TetraScope, and pushes an alias to that scope into this context
//Used for multithreading, so threads in the same scope can share the base scope,
//while also being able to branch off into their own call stacks
void TetraContext::branchOff(const scope_ptr baseScope) {
	//cout << "X from base context: " << *(const_cast<scope_ptr&>(baseScope)->lookupVar<TArray>("x")) << endl;
	scope_ptr newScopePtr(baseScope);
	//Note that the "branch off" should be the base of as new TetraContext (for a new thread)
	//assert(progStack.size() == 0);
	progStack.push(newScopePtr);
	//cout << "X after branch: " <<  *(lookupVar<TArray>("x")) << endl;;
}

//destroys the current scope, returning to the previously initialized scope
void TetraContext::exitScope() {
	progStack.pop();
}

//If, for some reason the tetra program crashes inadvertantly, we may as well clean up the TetraContext stack
/*TetraContext::~TetraContext() {
	while(!progStack.empty()) {
		progStack.pop();
	}
}

TData<void*>& TetraContext::declareReference(const string varName) {
	return progStack.top()->declareReference(varName);
}
*/
TetraScope& TetraContext::getCurrentScope() {
	return *(progStack.top());
}

scope_ptr& TetraContext::getScopeRef() {
	return progStack.top();
}

TetraContext& TetraContext::operator=(const TetraContext& other){
	progStack = other.progStack;
	return *this;
}

//Wraps a call to hte current scope's queryExecutionStatus()
ExecutionStatus TetraContext::queryExecutionStatus() {
	//cout << "Size: " << progStack.size() << endl;
	assert (progStack.empty() == false);
	//return progStack.top()->queryExecutionStatus();
	return progStack.top().queryExecutionStatus();
}


//Notify thew program of the given special occurances
void TetraContext::notifyBreak() {
	progStack.top().setExecutionStatus(BREAK);
}

void TetraContext::notifyContinue() {
	progStack.top().setExecutionStatus(CONTINUE);
}

void TetraContext::notifyReturn() {
	progStack.top().setExecutionStatus(RETURN);
}

void TetraContext::notifyElif() {
	progStack.top().setExecutionStatus(ELIF);
}

void TetraContext::notifyParallel() {
	progStack.top().setExecutionStatus(PARALLEL);
}

void TetraContext::normalizeStatus() {
	progStack.top().setExecutionStatus(NORMAL);
}

//Parallel setup/end calls
void TetraContext::addThread(pthread_t val) {
	progStack.top().addThread(val);
}

void TetraContext::setupParallel() {
	progStack.top().setupParallel();
}

void TetraContext::endParallel() {
	progStack.top().endParallel();
}

std::list<std::pair<pthread_t,TData<void*> > >& TetraContext::declareThreadSpecificVariable(const std::string& name) {
	return progStack.top()->declareThreadSpecificVariable(name);
}

long TetraContext::getThreadID() {
	return threadID;
}

//Prints a list of all function calls
void TetraContext::printStackTrace() const {

	TetraContext dummy = *this;

	using namespace std;
	//Check that callStack currently has something in it
	if(dummy.progStack.size() <= 1) {
		TetraEnvironment::getOutputStream() << "(The interpreter was unable to recover a stack trace)" << endl;
		return;
	}

	//Print the primary stack frame where the error occurred
	const Node* stackElement = dummy.progStack.top()->getCallNode();
	TetraEnvironment::getOutputStream() << FunctionMap::getFunctionSignature(stackElement) << " (line " << stackElement->getLine() << ")" << endl;
	dummy.exitScope();

	//Print further stack frames if there are any
	//Note that the first scope on the stack is the global vars
	while(dummy.progStack.size() > 1) {
		const Node* element = dummy.progStack.top()->getCallNode();
		TetraEnvironment::getOutputStream() << "Called from " << FunctionMap::getFunctionSignature(element) << " (line " << element->getLine() << ")" << endl;
		dummy.exitScope();	
	}

}
////////////////////////////////////////////////////////////////////////////////////////
//Debugger specific methods

//Returns an untyped pointer to a given variable
//Returns NULL if not found
void* TetraContext::fetchVariable(std::string s) {

        const std::map<std::string,int>& refTable = refTables.top();
        if(globRefTable.find(s) != globRefTable.end()) {
                //Return a pointer to the requested variable
                //Interface requires passing a node pointer
                //Note that although there is a global variable, their was never any mangling of the number (making it negative), so we are fine passing in the normal number without multiplying it by -1
                Node x(NODE_IDENTIFIER);
                x.setIntval(globRefTable.find(s)->second);
                x.setStringval(s);

		//lookupVar requies some type. We will be casting the pointer 'correctly' later
		//Passing void* is not an option since no variable can be of type void*
                void* ret = getGlobalScopeRef()->lookupVar<int*>(&x);
                return ret;
        }
        else if(refTable.find(s) != refTable.end()) {
                //Return a pointer to the requested variable
                //Interface requires passing a node pointer
                Node x(NODE_IDENTIFIER);
                x.setIntval(refTable.find(s)->second);
                x.setStringval(s);

                void* ret = lookupVar<int*>(&x);
                return ret;
        }
        else {
                return NULL;
        }

}


//Since the basic interpreter abstracts away the variable names, while in debug mode,
//the context needs to keep a reference table whenever entering or leaving a scope so that
//it can fulfill fetchVariable requests

//Checks if the current node references a new variable, and if so, adds it to the table.
//If is a function node, creates a new reference table for the new scope
//Note that this list is created dynamically because variables are officially initialized
//when they are encountered in the interpreter. A prebuilt table for the scope cannot
//reflect that different variables may have been initialized at different locations or
//logicl paths


void TetraContext::updateVarReferenceTable(const Node* node) {

        //We might need to push a new table to the stack, or add a new entry to the present table
        if(node->kind() == NODE_FUNCTION) {
                refTables.push(std::map<std::string,int>());
                //Push the formal params on
                Node* paramNode = node->child(0);

                //The variables should not yet exist
                while(paramNode->kind() == NODE_FORMAL_PARAM_LIST) {
                        refTables.top()[paramNode->child(0)->getString()] = paramNode->child(0)->getInt();
                        paramNode = paramNode->child(1);
                }
                //Push on final parameter (if it exists)
                if(paramNode->kind() == NODE_FORMAL_PARAM) {
                        refTables.top()[paramNode->getString()] = paramNode->getInt();
                }

        }
        else if(node->kind() == NODE_IDENTIFIER) {
                //If we have found an identifier, add an entry for it
                //We need this because it is possible that a user will use a variable before putting it in an assignment
                //e.g.
                //if someCondition
                //      x = 1
                //else
                //      squatdiddly
                //print(x)

                //Check if the variable already exists
                if(globRefTable.find(node->getString()) == globRefTable.end() && refTables.top().find(node->getString()) == refTables.top().end()){
                        //std::cout << "Registered: " <<node->getString() << " at " << node->getInt()<<endl;
                        refTables.top()[node->getString()] = node->getInt();
                }
        }

        //ASSUMPTION: These nodes always have an identifier or NODE_VECREF of some sort to their left
        else if(node->kind() == NODE_ASSIGN || node->kind() == NODE_GLOBAL || node->kind() == NODE_CONST) {
                //Check that we are assigning to a variable, rather than say, a vector refernece such as x[0][1]
                if(node->child(0)->kind() == NODE_IDENTIFIER){
                        //Make sure the var has not already been registered
                        //cout << node->child(0)->getString() <<"!!!!!!!" << endl;
                        if(globRefTable.find(node->child(0)->getString()) == globRefTable.end() && refTables.top().find(node->child(0)->getString()) == refTables.top().end()){
                                //std::cout << "Registered: " <<node->child(0)->getString() << " at " << node->child(0)->getInt()<<endl;
                                refTables.top()[node->child(0)->getString()] = node->child(0)->getInt();
                        }
                }
        }
        //else, we don't need to do anything
}
  


//When leaving a scope, pops the table of string references
void TetraContext::popReferenceTable() {
        refTables.pop();
}

