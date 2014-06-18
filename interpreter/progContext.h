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
	RETURN,
	REF_RETURN
};

//This embedded class represents the details of the present runtime environment, including the current VariableContext and loop depth
class TetraScope {

	public:
		TetraScope();

		template <typename T>
		T* lookupVar(string name) {
			return varScope.lookupVar<T>(name);
		}

		TData<void*>& declareReference(const string varName);

		//Used by loops and constrol statements to determine if they can proceed, or if they should return
		ExecutionStatus queryExecutionStatus();

		void setExecutionStatus(ExecutionStatus status);

		void setReturnedRef(void* const);

		void* getReturnedRef() const;

	private:
		VarTable varScope;
		ExecutionStatus executionStatus;
		void* returnedRef;
};



//This class wraps a std stack of TetraScopes

class TetraContext {

public:
	
	TetraContext();

	template<typename T>
	T* lookupVar(string name) {
		return progStack.top().lookupVar<T>(name);
	}

	TData<void*>& declareReference(const string varName);

	//Overloaded function call, one when there is no initial setup for a scope (i.e. a function call with no formal parameters that must be initialized)
	//The second is for adding a scope which had to have some data preloaded into it, as is the case when calling a function with arguments
	void initializeNewScope();
	void initializeNewScope(TetraScope& newScope);

	void exitScope();

	//If, for some reason the tetra program crashes inadvertantly, we may as well clean up the TetraContext stack
	~TetraContext();

	TetraScope& getCurrentScope();

	ExecutionStatus queryExecutionStatus();

	void notifyBreak();

	void notifyContinue();

	void notifyReturn();

	void notifyRefReturn();

	void notifyElif();

	void normalizeStatus();

	TetraContext& operator=(const TetraContext&);

	void setReturnedRef(void* const);

	void* getReturnedRef() const;

private:
	std::stack<TetraScope> progStack;
};

#endif
