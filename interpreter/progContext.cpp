/*
 * The classes contained in this file are used to keep track of certain properties of a program as it is being executed
 * These include the function call stack, the variables currently in scope, and where the programshould jump to should there be a break or continue statement
 * Although your standard running program utilizes one stack, this stack is represented via an object because if the eventual need for multiple call stacks through multithreading
 */


#include <stack>
#include <iostream>
#include <string>
#include "variableContext.h"
#include "frontend.hpp"
#include "progContext.h"
using std::string;


//This embedded class represents the details of the present runtime environment, including the current VariableContext and loop depth
TetraScope::TetraScope() {
	executionStatus = NORMAL;
}

//Used by loops and constrol statements to determine if they can proceed, or if they should return
ExecutionStatus TetraScope::queryExecutionStatus() {
	return executionStatus;
}

void TetraScope::setExecutionStatus(ExecutionStatus status) {
	executionStatus = status;
}



//This class wraps a std stack of TetraScopes

TetraContext::TetraContext() {
	//nothing at the moment
}

void TetraContext::initializeNewScope() {
	TetraScope newScope;
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

TetraScope* TetraContext::getCurrentScope() {
	 return &progStack.top();
}

TetraContext& TetraContext::operator=(const TetraContext& other){
	progStack = other.progStack;
	return *this;
}

ExecutionStatus TetraContext::queryExecutionStatus() {
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

void TetraContext::notifyElif() {
	progStack.top().setExecutionStatus(ELIF);
}

void TetraContext::normalizeStatus() {
	progStack.top().setExecutionStatus(NORMAL);
}

