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
#include "threadEnvironment.h"

using std::string;

//Each context will have a flag as to what action should be taken when control reaches a structure Node
//NORMAL: continue as usual
//ELIF: Denotes that program control is in an elif chain. Keep evaluating condiitons while this is true. Break, continue, and return take precendece
//CONTINUIE: Keep returning until a loop is hit, then reevaluate the loop node
//BREAK: keep returning until you hit a loop node, then return from that node
//RETURN: keep returning until you hit a function call, then return from the function call. This takes precedence over breaks and continues
//PARALLEL: each statement node encountered will result in a new spwned thread. Note that for conditionals and loops, only one thread will be spawned to evaluate the bodies of the conditionals/loops
enum ExecutionStatus {

	NORMAL,
	ELIF,
	CONTINUE,
	BREAK,
	RETURN,
	PARALLEL
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

		//This boolean denotes that there are multiple threads working in the current scope.
		//While that is the case, Insertions into the scope's VarTable must be performed in a threadsafe manner
		bool multiThreaded;

		//By storing the address of the call node, we can print back a call stack to the user if the program terminates unexpectedly
		const Node* callNode;
};



//This class wraps a std stack of TetraScopes

class TetraContext {

public:
	//Note that this constructor does NOT start with a default scope. One must be initialized through initializeNewScope before this can be used	
	TetraContext();

	//This initializer function initializes a new TetraContext object so that its base scope is aliased to the current scope of the currentContext.
	//This allows threads to share the same scope while not caring where they branch
	void initializeContextBranch(TetraContext* newContext, TetraContext& currentContext) {
		newContext->progStack.push( currentContext.progStack.top() );
	}

	//Wraps a call to lookupVar of the current scope
	template<typename T>
	T* lookupVar(string name) {
		return progStack.top()->lookupVar<T>(name);
	}

	//Wraps a call of declareReference for the current scope
	TData<void*>& declareReference(const string varName);

	//Overloaded function call, one when there is no initial setup for a scope (i.e. a function call with no formal parameters that must be initialized)
	//The second is for adding a scope which had to have some data preloaded into it, as is the case when calling a function with arguments
	void initializeNewScope(const Node* callNode);
	void initializeNewScope(TetraScope& newScope);

	//Pushes an alias to the given scope onto the stack
	//Used for multithreading
	void branchOff(const TetraContext& baseScope);

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
	void notifyParallel();

	//Sets the current scope's executionStatus to NORMAL
	void normalizeStatus();

	//Performs a deep copy of the current context
	TetraContext& operator=(const TetraContext&);

	//Methods dealing with parallelism at a contextual level
	void addThread(pthread_t);
	void setupParallel();
	void endParallel();

	//Prints a stack trace
	void printStackTrace() const;

private:
	//Class defines a smart pointer to a TetraScope
	class scope_ptr {

	public:
                scope_ptr(const Node * callNode) {
                        ptr = new TetraScope(callNode);
                        refCount = new int();//Zero initialized
                        *refCount = 0;
			status = NORMAL;
			spawnedThreads = NULL;
                        addReference();
                }

		//Creates a pointer that points to a COPY of the given tetrascope. Used for initializing funciton parameters. Note that scope does not include spawned threads.
                scope_ptr(TetraScope& newScope) {
                        ptr = new TetraScope(newScope);
                        refCount = new int();//Zero initialized
                        *refCount = 0;
			status = NORMAL;
			spawnedThreads = NULL;
                        addReference();
                }
		//Copy constructor aliases this Scope to the other, rather than performing a deep copy
                //Note that this is largely desired behavior
                scope_ptr(const scope_ptr& other) {
                        ptr = other.ptr;
                        refCount = other.refCount;
			status = other.status;
			spawnedThreads = NULL;
                        addReference();
                }

                ~scope_ptr() {
                        removeReference();
			//Note that if spawnedThreads is null, delete is still valid
			delete spawnedThreads;
                        //Check to see if we must delete the underlying object
                        if(*refCount == 0) {
                                delete refCount;
                                delete ptr;
                        }
                }

                //Assignment operator aliases the pointer to the scope, but stillm ust have its own thread call stack
                //Note that this means that the copy assignment operator/copy constructor will perform a SHALLOW copy
                //For the purposes of the interpreter, however, this is the desired behavior
                scope_ptr& operator=(const scope_ptr& other) {
                        if(&other != this) {
                                removeReference();
                                //Check to see if we must delete the vector that this used to point to
                                if(*refCount == 0) {
                                        delete refCount;
                                        delete ptr;
                                }
                                ptr = other.ptr;
                                refCount = other.refCount;
				status = other.status;
				spawnedThreads = NULL;
                                addReference();
                        }
                        return *this;
                }

		void addThread(pthread_t thread) {
			spawnedThreads->addThread(thread);
		}

		void setupParallel() {
			if(spawnedThreads == NULL) {
				spawnedThreads = new ThreadPool();
			}
			else {
				//TODO: This should be an error
				std::cout << "ERROR" << endl;
			}
		}

		void endParallel() {
			spawnedThreads->waitTillEmpty();
			delete spawnedThreads;
			spawnedThreads = NULL;
		}

		

                //Methods to simulate pointer functionality
                TetraScope& operator*() const {
                        return *ptr;
                }
                TetraScope* operator->() const {
                        return ptr;
                }

		//void notifyBreak() {status = BREAK; }
		//void notifyContinue() {status = CONTINUE; }
		//void notifyReturn() {status = RETURN; }
		//void notifyElif() {status = ELIF; }
		//void normalizeStatus() {status = NORMAL; }
		void setExecutionStatus(ExecutionStatus pStatus){status = pStatus; }
		ExecutionStatus queryExecutionStatus() {return status; }

        private:
                void addReference() {
                        (*refCount)++;
                }
                void removeReference() {
                        (*refCount)--;
                }

                TetraScope* ptr;
                int* refCount;
		//Because different threads my have their own execution statuses, this variable is stored here
		ExecutionStatus status;
		ThreadPool* spawnedThreads;
	};
	std::stack<scope_ptr> progStack;
	
};


#endif
