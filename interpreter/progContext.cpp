/*
 * The classes contained in this file are used to keep track of certain properties of a program as it is being executed
 * These include the function call stack, the variables currently in scope, and where the programshould jump to should there be a break or continue statement
 * Although your standard running program utilizes one stack, this stack is represented via an object because if the eventual need for multiple call stacks through multithreading
 */


#include <stack>
#include <iostream>
#include <string>
#include "variableContext.cpp"
#include "libfrontend.a"

using std string;

//Each context will have a flag as to what action should be taken when control is switched to a looping statement
enum LoopStatus {

	NORMAL,
	CONTINUE,
	BREAK,
	RETURN

}

//This class represents the details of the present runtime environment, including the current VariableContext and loop depth
class TetraScope {

public:
	TetraScope() {
		loopStatus = NORMAL;
	}

	template <typename T>
	T* lookupVar(String name) {
		return varScope.lookupVar<T>(name);
	}

	//Used by loops and statements to determine if they can proceed, or if they should return
	//At the moment, I am working under the assumption that only "Statement" and loop nodes will need to check for a loop status, as these are the only nodes that allow execution of further children after returning.
	LoopStatus queryLoopFlag() {
		return loopFlag;
	}

private:
	VarTable varScope;
	LoopStatus loopFlag;
}

//This class wraps a std stack of TetraScopes

class TetraContext {

public:
	
	TetraContext() {
		//nothing at the moment
	}

	template<typename T>
	T* lookupVar(String name) {
		return progStack.top().lookupVar<T>(name);
	}

	initializeNewScope() {
		//To be continued!	
	}

	//If, for some reason the tetra program crashes inadvertantly, we may as well clean up the TetraContext stack
	~TetraContext() {
		while(!progStack.empty()) {
			progStack.pop();
		}
	}

private:
	std::stack<TetraScope> progStack;
}


