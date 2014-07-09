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
#include <pthread.h>
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


//Class defines a smart pointer to a TetraScope
class scope_ptr {

	public:
		scope_ptr(const Node * callNode) {
			ptr = new TetraScope(callNode);
			refCount = new int();//Zero initialized
			*refCount = 0;
			status = NORMAL;
			//spawnedThreads = NULL;
			refCount_mutex_ptr = new pthread_mutex_t();
			addReference();
		}

		//Creates a pointer that points to a COPY of the given tetrascope. Used for initializing funciton parameters. Note that scope does not include spawned threads.
		scope_ptr(const TetraScope& newScope) {
			ptr = new TetraScope(newScope);
			refCount = new int();//Zero initialized
			*refCount = 0;
			status = NORMAL;
			//spawnedThreads = NULL;
			refCount_mutex_ptr = new pthread_mutex_t();
			addReference();
		}
		//Copy constructor aliases this Scope to the other, rather than performing a deep copy
		//Note that this is largely desired behavior
		scope_ptr(const scope_ptr& other) {
			other.lockScope();
			ptr = other.ptr;
			status = other.status;
			//spawnedThreads = NULL;
			refCount_mutex_ptr = other.refCount_mutex_ptr;

			//Note that we are copying the pointer, not the value!
			//Since the pointer does not change, this assignment is threadsafe
			refCount = other.refCount;

			(*refCount)++;
			other.unlockScope();
			//addReference();
		}

		~scope_ptr() {
			removeReference();
			//Note that if spawnedThreads is null, delete is still valid
			//delete spawnedThreads;
			//Check to see if we must delete the underlying object

		}

		//Assignment operator aliases the pointer to the scope, but stillm ust have its own thread call stack
		//Note that this means that the copy assignment operator/copy constructor will perform a SHALLOW copy
		//For the purposes of the interpreter, however, this is the desired behavior
		//This does not appear to ever get called. If that is the case, our thread-safetyness burden eases considerably, as we should only have to lock the reference incrementing/decrememnting
		scope_ptr& operator=(const scope_ptr& other) {
			cout << "THIS GETS CALLED\n\n\n\n\n" << endl;
			if(&other != this) {
				//release old data
				removeReference();

				//Copy new data
				ptr = other.ptr;
				refCount = other.refCount;
				status = other.status;
				//spawnedThreads = NULL;
				addReference();
			}
			return *this;
		}

		void addThread(pthread_t thread) {
			//Note that the threadpool implementation of this method is threadsafe
			spawnedThreads.top()->addThread(thread);
		}

		void setupParallel() {
			//Since each scope_ptr gets its own copy of spawnedThreads (even if everything else is an alias), this is threadsafe
			ThreadPool* newPool = new ThreadPool();
			spawnedThreads.push(newPool);
		}

		void endParallel() {
			spawnedThreads.top()->waitTillEmpty();
			delete spawnedThreads.top();
			spawnedThreads.pop();
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
		//add/removeReference are put under a mutex for thread safety
		void addReference() {
			pthread_mutex_lock(refCount_mutex_ptr);
			(*refCount)++;
			pthread_mutex_unlock(refCount_mutex_ptr);
		}
		//When a reference is removed, check if we must delete it
		//Note that when the last reference is removed, there are physically NO MORE REFERENCES to the data, so there is nothing ot fear from a threadsafety perspective
		//This includes the possibility of copy constructing, since if a thread is copy constructing, it is a)holding a reference and b) not deleting that reference
		void removeReference() {

			pthread_mutex_lock(refCount_mutex_ptr);

			//Bool is used to check if we need to destroy the mutex at the end
			bool destroyable = false;

			(*refCount)--;
			if(*refCount == 0) {
				delete refCount;
				delete ptr;
				destroyable = true;
			}

			pthread_mutex_unlock(refCount_mutex_ptr);
			if(destroyable) {
				delete refCount_mutex_ptr;
			}
		}

		void lockScope() const {
			pthread_mutex_lock(refCount_mutex_ptr);
		}

		void unlockScope() const {
			pthread_mutex_unlock(refCount_mutex_ptr);
		}

		TetraScope* ptr;
		int* refCount;
		//Because different threads my have their own execution statuses, this variable is stored here
		ExecutionStatus status;

		//Because each context may spawn its own threads, we need to keep track of spawned threads on a thread-by-thread basis
		//Becausehe main thread in parallel blocks. we actually have to keep track of a stack of threadpools, in case the main thread encounters another parallel statement in the same scope
		std::stack<ThreadPool*> spawnedThreads;

		//Because different threads may destroy themselves at arbitrary times, we must lock uses of refCount
		pthread_mutex_t* refCount_mutex_ptr;
};


//This class wraps a std stack of TetraScopes

class TetraContext {

public:
	//Note that this constructor does NOT start with a default scope. One must be initialized through initializeNewScope before this can be used	
	TetraContext();

	//This initializer function initializes a new TetraContext object so that its base scope is aliased to the current scope of the currentContext.
	//This allows threads to share the same scope while not caring where they branch
/*	void initializeContextBranch(TetraContext* newContext, TetraContext& currentContext) {
		newContext->progStack.push( currentContext.progStack.top() );
	}
*/
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
	void branchOff(const scope_ptr baseScope);

	//Pops the current scope off the stack. Has the effect of destroying al variables of the present scope
	void exitScope();

	//If, for some reason the tetra program crashes inadvertantly, we may as well clean up the TetraContext stack
	~TetraContext();

	//Returns a reference to the current scope
	//This may be outdated
	TetraScope& getCurrentScope();

	scope_ptr& getScopeRef();

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

	std::stack<scope_ptr> progStack;
	
};


#endif
