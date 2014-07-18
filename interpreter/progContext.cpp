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

//This embedded class represents the details of the present runtime environment, including the current VariableContext and loop depth
TetraScope::TetraScope(const Node* pCallNode) : executionStatus(NORMAL), callNode(pCallNode) {

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

void TetraScope::setCallNode(const Node* node) {
	callNode = node;
}

const Node* TetraScope::getCallNode() const {
	return callNode;
}


//-------------------------------------------------------------
//This class wraps a std stack of TetraScopes

TetraContext::TetraContext() {
	//nothing at the moment
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
	assert(progStack.size() == 0);
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

//Prints a list of all function calls
void TetraContext::printStackTrace() const {

	TetraContext dummy = *this;

	using namespace std;
	//Check that callStack currently has something in it
	if(dummy.progStack.size() == 0) {
		TetraEnvironment::getOutputStream() << "(The interpreter was unable to recover a stack trace)" << endl;
		return;
	}

	//Print the primary stack frame where the error occurred
	const Node* stackElement = dummy.progStack.top()->getCallNode();
	TetraEnvironment::getOutputStream() << FunctionMap::getFunctionSignature(stackElement) << " (line " << stackElement->getLine() << ")" << endl;
	dummy.exitScope();

	//Print further stack frames if there are any
	while(dummy.progStack.size() > 0) {
		const Node* element = dummy.progStack.top()->getCallNode();
		TetraEnvironment::getOutputStream() << "Called from " << FunctionMap::getFunctionSignature(element) << " (line " << element->getLine() << ")" << endl;
		dummy.exitScope();	
	}

}
