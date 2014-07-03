#ifndef PROG_CONTEXT_H
#define PROG_CONTEXT_H

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

using std::string;

//Each context will have a flag as to what action should be taken when control reaches a structure Node
//NORMAL: continue as usual
//ELIF: Denotes that program control is in an elif chain. Keep evaluating condiitons while this is true. Break, continue, and return take precendece
//CONTINUIE: Keep returning until a loop is hit, then reevaluate the loop node
//BREAK: keep returning until you hit a loop node, then return from that node
//RETURN: keep returning until you hit a function call, then return from the function call. This takes precedence over breaks and continues
enum ExecutionStatus {

	NORMAL,
	ELIF,
	CONTINUE,
	BREAK,
	RETURN
};

//This embedded class represents the details of the present runtime environment, including the current VariableContext and loop depth
class TetraScope {

	public:
		TetraScope(const Node*);

		//Returns a pointer to the data referenced by the given variable name 'name', or creates a place for it if it does not yet exist
		template <typename T>
		T* lookupVar(string name) {
			return varScope.lookupVar<T>(name);
		}

		//Used for aliasing an array
		//Returns an unitialized pointer that will be associated with varName
		//The calling program can set this pointer to point to whatever varname should alias.
		TData<void*>& declareReference(const string varName);

		//Used by loops and constrol statements to determine if they can proceed, or if they should return
		ExecutionStatus queryExecutionStatus();

		//sets the execution status to the specified value
		void setExecutionStatus(ExecutionStatus status);

		//Used by the TetraContext to obtain a stack trace
		void setCallNode(const Node*);
		const Node* getCallNode() const;
	private:
		VarTable varScope;
		ExecutionStatus executionStatus;

		//By storing the address of the call node, we can print back a call stack to the user if the program terminates unexpectedly
		const Node* callNode;
};



//This class wraps a std stack of TetraScopes

class TetraContext {

public:
	//Note that this constructor does NOT start with a default scope. One must be initialized through initializeNewScope before this can be used	
	TetraContext();

	//Wraps a call to lookupVar of the current scope
	template<typename T>
	T* lookupVar(string name) {
		return progStack.top().lookupVar<T>(name);
	}

	//Wraps a call of declareReference for the current scope
	TData<void*>& declareReference(const string varName);

	//Overloaded function call, one when there is no initial setup for a scope (i.e. a function call with no formal parameters that must be initialized)
	//The second is for adding a scope which had to have some data preloaded into it, as is the case when calling a function with arguments
	void initializeNewScope(const Node* callNode);
	void initializeNewScope(TetraScope& newScope);

	//Pops the current scope off the stack. Has the effect of destroying al variables of the present scope
	void exitScope();

	//If, for some reason the tetra program crashes inadvertantly, we may as well clean up the TetraContext stack
	~TetraContext();

	//Returns a reference to the current scope
	TetraScope& getCurrentScope();

	//wraps a call to the current scope's queryExecutionStatus
	ExecutionStatus queryExecutionStatus();

	//Sets the current scope's ExecutionStatus to the appropriate value
	void notifyBreak();
	void notifyContinue();
	void notifyReturn();
	void notifyElif();

	//Sets the current scope's executionStatus to NORMAL
	void normalizeStatus();

	//Performs a deep copy of the current context
	TetraContext& operator=(const TetraContext&);

	//Prints a stack trace
	void printStackTrace();

private:
	std::stack<TetraScope> progStack;
};

#endif
