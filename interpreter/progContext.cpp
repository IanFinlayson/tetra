/*
 * The classes contained in this file are used to keep track of certain properties of a program as it is being executed
 * These include the function call stack, the variables currently in scope, and where the programshould jump to should there be a break or continue statement
 * Although your standard running program utilizes one stack, this stack is represented via an object because if the eventual need for multiple call stacks through multithreading
 */


#include <stack>
#include <iostream>
#include <string>
#include "frontend.hpp"
#include "progContext.h"
//#include "variableContext.h"



//#define NDEBUG
#include <assert.h>

using std::string;


//This embedded class represents the details of the present runtime environment, including the current VariableContext and loop depth
TetraScope::TetraScope() {
	executionStatus = NORMAL;
}

//Wraps the varTable::declareReference
TData<void*>& TetraScope::declareReference(const string varName) {
	return varScope.declareReference(varName);
}

//Used by loops and constrol statements to determine if they can proceed, or if they should return
ExecutionStatus TetraScope::queryExecutionStatus() {
	return executionStatus;
}

void TetraScope::setExecutionStatus(ExecutionStatus status) {
	executionStatus = status;
}

void TetraScope::setReturnedRef(void* const ref) {
	returnedRef = ref;
}

void* TetraScope::getReturnedRef() const {
	return returnedRef;
}

//This class wraps a std stack of TetraScopes

TetraContext::TetraContext() {
	//nothing at the moment
}

void TetraContext::initializeNewScope() {
	TetraScope newScope;
	progStack.push(newScope);
}

void TetraContext::initializeNewScope(TetraScope& newScope) {
	progStack.push(newScope);
}

void TetraContext::exitScope() {
	progStack.pop();
}

//If, for some reason the tetra program crashes inadvertantly, we may as well clean up the TetraContext stack
TetraContext::~TetraContext() {
	while(!progStack.empty()) {
		progStack.pop();
	}
}

TData<void*>& TetraContext::declareReference(const string varName) {
	return progStack.top().declareReference(varName);
}


TetraScope& TetraContext::getCurrentScope() {
	 return progStack.top();
}

TetraContext& TetraContext::operator=(const TetraContext& other){
	progStack = other.progStack;
	return *this;
}

ExecutionStatus TetraContext::queryExecutionStatus() {
	//cout << "Size: " << progStack.size() << endl;
	assert (progStack.empty() == false);
	return progStack.top().queryExecutionStatus();
}

void TetraContext::notifyBreak() {
	progStack.top().setExecutionStatus(BREAK);
}

void TetraContext::notifyContinue() {
	progStack.top().setExecutionStatus(CONTINUE);
}

void TetraContext::notifyReturn() {
	progStack.top().setExecutionStatus(RETURN);
}

void TetraContext::notifyRefReturn() {
	progStack.top().setExecutionStatus(REF_RETURN);
}

void TetraContext::notifyElif() {
	progStack.top().setExecutionStatus(ELIF);
}

void TetraContext::normalizeStatus() {
	progStack.top().setExecutionStatus(NORMAL);
}

void TetraContext::setReturnedRef(void* const ref) {
	 progStack.top().setReturnedRef(ref);
}

void* TetraContext::getReturnedRef() const {
	return progStack.top().getReturnedRef();
}


