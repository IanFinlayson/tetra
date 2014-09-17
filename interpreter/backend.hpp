#ifndef BACKEND_H
#define BACKEND_H
/*
 * This class wraps all data types used in a tetra program.
 * A note about the comments, an "ordinary case" for using a TData object involves all situations EXCEPT when:
 * The template for the type is void* and...
 * the void* is pointing at a dynamically allocated piece of memory for which the TData has ownership
 */ 

#include <iostream>
#include <string>
#include "frontend.hpp"
#include <stack>
#include <pthread.h>
#include <time.h>
#include <list>
//#define NDEBUG
#include <assert.h>
#include <map>
//#include<cstdlib>
#include<algorithm> //for find_if
#include<sstream>


//Interpret function from main

int interpret(Node*);

template<typename T>
class TData{
	public:
		TData();

		TData(const T& pData);

		//Copy Constructor
		TData(const TData<T>& other);

		//default destructor
		~TData();

	
		//If this TData object is storing a pointer to a dynamically allocated variable, it MUST USE THIS METHOD TO INFORM THE OBJECT THAT IT SHOULD DELETE SOMETHING WHEN IT IS DONE
		//In essence, this method notifies the object that it is no longer being used in an ordinary case, but has ownership of some dynamically allocated memory		
		template<typename R>
		void setDeletableType();

		const DataType getPointedTo() const;

		template<typename R>
		bool setData(const R& pData);

		TData<T>& operator=(const TData<T>&);

		const T& getData() const;
		T& getData();
	private:
		T data;
		//used for deleting dynamically allocated data if this object utilizes it;
		DataType pointedTo;
};


template<typename T>
TData<T>::TData() : data(), pointedTo(TYPE_VOID) {
}

template<typename T>
TData<T>::TData(const T& pData) : data(pData),  pointedTo(TYPE_VOID) {
}

//Default copy constructor is fine for ordinary case
template<typename T>
TData<T>::TData(const TData<T>& other) : data(other.data), pointedTo(other.pointedTo.getKind()) {
}

//default destructor, in ordinary cases no cleanup is needed
template<typename T>
TData<T>::~TData() {
}

//Naive copt is adequate under ordinary circumstances
template <typename T>
TData<T>& TData<T>::operator=(const TData<T>& other) {
	//unles we are  working with variables (see TDataspecializations), a naive copy shouild be fine
	pointedTo = other.pointedTo;
	data = other.data;
	return *this;
}

//Used to denote that the TData should treat itself as if it had a dynamically allocated member
//When set, TData will perform deep copies, and delete what it is pointing to when it is done
//If this TData object is storing a pointer to a dynamically allocated variable, it MUST USE THIS METHOD TO INFORM THE OBJECT THAT IT SHOULD DELETE SOMETHING WHEN IT IS DONE		
//'Default' behavior occurs when user calls this method with an unexpected variable type. This should bever happen, hence the assertion
template<typename T> template<typename R>
void TData<T>::setDeletableType(){
	//A non-specialized version of this method should never be called
	assert(false);
	//cout << "Error, attmepting to set unsupported type as deletable type.\nThis may result in a memory leak." << endl;
}

template<typename T>
const DataType TData<T>::getPointedTo() const {
	return pointedTo.getKind();
}

template<typename T> template<typename R>
bool TData<T>::setData(const R& pData) {
	//By default, this should not do anything, but we will specialize the tamplate for the few scenarios where this actually works
	//Note that this is occasionbally called on mismatched types. This is expected behavior for the interpreter.
	//As an exampole, a statement node may be expecting an int return type, because it was called within a function returning an int,
	//it then sees s = "abc" which returns a string. The interpreter will call TData<int>::setData<string>(...), and do nothing.
	return false;
}   

//Get a reference to what is stored within the object
template<typename T>
const T& TData<T>::getData() const {
	return data;
}

template<typename T>
T& TData<T>::getData() {
	return data;
}


/*
 * This class wraps an array of tData<void*> used to make arrays and multi-dimensional arrays
 * 
 */

class TArray {

public:

	TArray();
	TArray(const TArray&);
	~TArray();


	//gets the element pointed to by
	//(const and non-const versions)
	TData<void*>& elementAt(unsigned int);
	const TData<void*>& elementAt(unsigned int) const;

	//Add an element using copy constructor
	void addElement(const TData<void*>&);
	
	//Utility methods used for iterating over the vector in for (-each) loops, and vector cleanup	
	const std::vector< TData<void*> >::const_iterator begin() const;
	const std::vector< TData<void*> >::const_iterator end() const;

	int size() const;

	//Copy assignment operator
	TArray& operator=(const TArray& other);

	//All operations must be defined (or at least stubbed) to work with our operators!
	TArray operator||(TArray& other) {
		return *this;
	}
	TArray operator&&(TArray& other) {
		return *this;
	}
	bool operator<(TArray& other) {
		return false;
	}
	bool operator<=(TArray& other) {
		return false;
	}
	bool operator>(TArray& other) {
		return false;
	}
	bool operator>=(TArray& other) {
		return false;
	}
	bool operator==(TArray& other) {
		return false;
	}
	bool operator!=(TArray& other) {
		return false;
	}
	TArray operator^(TArray& other) {
		return *this;
	}
	TArray operator|(TArray& other) {
		return *this;
	}
	TArray operator&(TArray& other) {
		return *this;
	}
	TArray operator<<(TArray& other) {
		return *this;
	}
	TArray operator>>(TArray& other) {
		return *this;
	}
	TArray operator+(TArray& other) {
		return *this;
	}
	TArray operator-(TArray& other) {
		return *this;
	}
	TArray operator*(TArray& other) {
		return *this;
	}
	TArray operator/(TArray& other) {
		return *this;
	}
	TArray operator%(TArray& other) {
		return *this;
	}
	TArray operator!() {
		return *this;
	}
	//The specialization for TArray EXP TArray is presently in the operationMap class
	
private:

	//Implementation of smart pointer for vector that uses simple reference counting
	class vec_ptr {
	public:
		vec_ptr() {
			ptr = new std::vector< TData<void*> >;
			refCount = new int();//Zero initialized
			*refCount = 0;
			array_mutex_ptr = new pthread_mutex_t();
			pthread_mutex_init(array_mutex_ptr, NULL);
			addReference();
		}

		//Copy constructor aliases this TArray to the other, rather than performing a deep copy
		//Note that this is largely desired behavior
		//there is an error here if the array gets deleted whiole it is passed, and before it locks
		vec_ptr(const vec_ptr& other) {
			other.lockArray();
			ptr = other.ptr;
			refCount = other.refCount;
			array_mutex_ptr = other.array_mutex_ptr;
			//Note that we already have the mutex needed to change the increment value
			(*refCount)++;

			other.unlockArray();
			//addReference();	
		}

		~vec_ptr() {
			removeReference();
		}

		//Assignment operator aliases the pointer
		//Note that this means that the copy assignment operator/copy constructor will perform a SHALLOW copy
		//For the purposes of the interpreter, however, this is the desired behavior
		vec_ptr& operator=(const vec_ptr& other) {

			if(&other != this) {

				removeReference();

				//This may contain a race condition
				//other may be getting copy assigned as well, leading to corrupted values
				//TODO: check this out on paper
				other.lockArray();
				ptr = other.ptr;
				refCount = other.refCount;
				array_mutex_ptr = other.array_mutex_ptr;

				(*refCount)++;//We already hold the mutex for incrementing this!

				other.unlockArray();

				//addReference();
			}
			return *this;
		}

		//Methods to simulate pointer functionality
		std::vector< TData<void*> >& operator*() const {
			return *ptr;
		} 
		std::vector< TData<void*> >* operator->() const {
			return ptr;
		}
		
	private:
		void addReference() {
			pthread_mutex_lock(array_mutex_ptr);
			(*refCount)++;
			pthread_mutex_unlock(array_mutex_ptr);
		}
		void removeReference() {

			pthread_mutex_lock(array_mutex_ptr);
			
			//Bool used to flag if the mutex can be destroyed
			//(since we cannot destroy it while holding it)
			bool destroyable = false;

			(*refCount)--;
			if(*refCount == 0) {
				delete refCount;
				delete ptr;
				//Used temporarily to insure that if a race condition occurs, a stale pointer does not still coincidentally point at an ok value
				ptr = NULL;
				destroyable = true;
			}

			pthread_mutex_unlock(array_mutex_ptr);

			if(destroyable) {
				delete array_mutex_ptr;
			}
		}
		
		//For the copy constructor, we must insure that the source does not get corrupted as we are writing to ourselves
		void lockArray() const{
			pthread_mutex_lock(array_mutex_ptr);
		}

		void unlockArray() const{
			pthread_mutex_unlock(array_mutex_ptr);
		}

		std::vector< TData<void*> >* ptr;
		int* refCount;

		//Because threads can be copied in different threads (see, pass by reference to functions)
		//we must thread-safe thread assignment
		pthread_mutex_t* array_mutex_ptr;
		//pthread_mutex_t* copy_mutex_ptr;
	};
	
	//A smart pointer to a vector of void*
	vec_ptr elements;

	//Implementation of stream insertion allows for printing of array
	friend std::ostream& operator<<(std::ostream& outStream, TArray obj);
};


template<> TData<void*>::TData(const TData<void*>& other);
template<> TData<void*>::~TData();
template<> TData<void*>& TData<void*>::operator=(const TData<void*>&);
template<> template<> void TData<void*>::setDeletableType<TArray>();

//Template specializations for setData and setDeletable type
//Declared here because TData<TArray> must be defined before all declarations may occur
template<> template<> bool TData<int>::setData<int>(const int&);
template<> template<> bool TData<double>::setData<double>(const double&);
template<> template<> bool TData<bool>::setData<bool>(const bool&);
template<> template<> bool TData<std::string>::setData<std::string>(const std::string&);
template<> template<> bool TData<int*>::setData<int*>(int* const &);
template<> template<> bool TData<double*>::setData<double*>(double* const &);
template<> template<> bool TData<std::string*>::setData<std::string*>(std::string* const &);
template<> template<> bool TData<bool*>::setData<bool*>(bool* const &);
template<> template<> bool TData<TArray*>::setData<TArray*>(TArray*const &);
template<> template<> bool TData<void*>::setData<void*>(void* const &);
template<> template<> void TData<void*>::setDeletableType<int>();
template<> template<> void TData<void*>::setDeletableType<double>();
template<> template<> void TData<void*>::setDeletableType<bool>();
template<> template<> void TData<void*>::setDeletableType<std::string>();
template<> template<> void TData<void*>::setDeletableType<void>();
template<> template<> bool TData<TArray>::setData<TArray>(const TArray&);

/*
 * This class represents the variables present at various scopes within a tetra program
 */
class VarTable {

public:
	
	VarTable();

	//Release allocated data
	~VarTable();

	VarTable& operator=(const VarTable& other) {
		cout << "Please don;t use this! (VarTable::operator=)" << endl;
		return *this;
	}

	//returns a reference to the storage location of the variable. The interpreter supplies the expected type.
	template<typename T>
	T* lookupVar(const std::string varName);

	//Checks whether a named variable is in the scope, without actually adding it if it does not exist
	bool containsVar(const std::string varName) const;

	//Returns a TData from within varMap containing an invalid reference. The reference can be set to something valid, and that thing will not be deleted when this variableContext goes out of scope
	TData<void*>& declareReference(const std::string varName);

	std::list<std::pair<pthread_t,TData<void*> > >& declareParForVar(const std::string&);
private:
	
	std::map<std::string, TData<void*> > varMap;
	//pthread_mutex_t table_mutex;
	pthread_rwlock_t table_mutex;

	//This is a std::vector containing pairs associating strings with another vector containing pairs that associate a thread with a TData value
	//This is used in the context of parallel for loops, where the lookup function can:
	//1) check if the outer vector contains a string matching the given lookup value
	//2) if so, obtain the vector for that value
	//3) search the pairs of the inner vector to see if the calling thread has an entry for that variable
	//4) return the correct variable for that thread
	std::list< std::pair<std::string, std::list<std::pair<pthread_t,TData<void*> > > > > parForVars;

	//Predicate functions for the lookupVar function
	//This code used from stack overflow question 12008059
	struct CheckName {
		CheckName(std::string pVal) : searchVal(pVal) {
		}
		bool operator()(std::pair<std::string, std::list<std::pair<pthread_t,TData<void*> > > > val) {
			return searchVal == val.first;
		}
		private:
		std::string searchVal;
	};

	struct CheckThread {
		CheckThread(pthread_t pVal) : searchVal(pVal) {
		}
		bool operator()(std::pair<pthread_t,TData<void*> > val) {
			return searchVal == val.first;
		}
		private:
		pthread_t searchVal;
	};


};
//Returns a reference to the storage location of the variable
////defined in the header because it is a template
//TODO: set a single return value for single exit point, and simplify lock/unlock semantics
template<typename T>
T* VarTable::lookupVar(const std::string varName) {
	//pthread_mutex_lock(&table_mutex);
	
	//Check whether this variable is a parallel for variable. Ideally there won;t be many of these floating around, but we can implement a non-linear algorithm later if needed
	std::list< std::pair<std::string, std::list<std::pair<pthread_t,TData<void*> > > > >::iterator loc;
	
	pthread_rwlock_rdlock(&table_mutex);

	//Check if variable is a thread-specific variable (e.g. parallel for loop variable)	
	loc = std::find_if(parForVars.begin(), parForVars.end(), CheckName(varName));
	if(loc != parForVars.end()) {
		std::list< std::pair< pthread_t, TData<void*> > >& varList = (*loc).second;
		std::list< std::pair< pthread_t, TData<void*> > >::iterator value;
		value = std::find_if(varList.begin(), varList.end(), CheckThread(pthread_self()));
		if(value != varList.end()) {

			T* ret = static_cast<T*>(value->second.getData());
			pthread_rwlock_unlock(&table_mutex);
			return ret;
		}
		else {	//If this thread has not yet registered a value, create it
			//Unlock the read mutex while we assmeble the data to be inserted, will eventually obtain write privelages
			pthread_rwlock_unlock(&table_mutex);

			T* newData_ptr = new T();
			//cout << "New Data: " << newData_ptr << endl;
			TData<void*> insertable(newData_ptr);
			//Ensure that insertable gets copied correctly, and deleted correctly when we are done
			insertable.setDeletableType<T>();
			//Obtain write privelages
			pthread_rwlock_wrlock(&table_mutex);

			//Note that since THIS thread can push in a pair with pthread_t equal to pthread_self()
			//Hence there is no need to check whether the condition is still true or not

			varList.push_back(std::pair<pthread_t,TData<void*> >(pthread_self(), insertable));
			(varList.rbegin())->second.setDeletableType<T>();
			T* ret = static_cast<T*>((varList.rbegin())->second.getData());
			pthread_rwlock_unlock(&table_mutex);
			//cout << "Ret: " << ret << endl;
			return ret;
		}
	}
	//check whether a normal entry exists for this variable
	//The find and [] operators are redundant. we can combine them into one search
	else if(!containsVar(varName)) {

		//This is a legacy portion of code. The check for if something is a global variable is presently in the TetraContext's wrapper call of lookupVar
		//If the variable is not in the current scope, it might be in the global scope
/*		scope_ptr& globalScopeRef = context.getGlobalScopeRef();

		if(globalScopeRef->containsVar(varname)) {

			T* ret = globalScopeRef->lookupVar<T>(varName);

			//release read mutex
			pthread_rwlock_unlock(&table_mutex);
			return ret;
		}
*/
		//Otherwise, we must insert a new variable into the current scope

		//Release read privelages while we assemble what we need to insert
		pthread_rwlock_unlock(&table_mutex);
		//If the variable does not yet exist, we need to allocate memory for the TData to point to!
		T* newData_ptr = new T(); //() should zero the memory for primitive types, so that all types have a default value
		TData<void*> insertable(newData_ptr);
		//Must notify TData that it is pointing at dynamically allocated memory
		insertable.setDeletableType<T>();

		//Obtain write privelages before we insert
		pthread_rwlock_wrlock(&table_mutex);

		//Note that another thread may have inserted the value while we were waitring for write privelages!
		//Hence, we must again check that the variable is stillnot in the table

		if(varMap.find(varName) == varMap.end()) {
			varMap[varName] = insertable;
		}
		//else do nothing, as the statement after this block will retrieve the correct value
	}
	   
	T* ret = static_cast<T*>(varMap[varName].getData());

	//Release whatever lock we happened to have
	pthread_rwlock_unlock(&table_mutex);

	//return static_cast<T*>(varMap[varName].getData());
	
	return ret;
}



/*
 * ThreqdEnvironment wraps single global object storing infomration about the states of all threads in the program
 * This class is responsible for ensuring that all threads exit in an orderly fashion
 */


//This class contains everything needed for a thread of execution to keep track of and join back with all threads that it spawns

class ThreadPool {

private:

	pthread_mutex_t threadCount_mutex;
	std::vector<pthread_t> currentThreads;

public:

	ThreadPool();

        //Since the threadCount is a less-visited area, we will use a coarse mutex for reading and writing
        //Atomically queries the number of registered active threads
        int queryThreads();

        //Atomically increments the number of active threads
        void addThread(pthread_t aThread);

        //Atomically decrements the number of active threads
        void removeThread(pthread_t rThread);

        pthread_t getNextJoin();
	
	void waitTillEmpty();
};


class ThreadEnvironment {

private:

	//This holds the count of ADDITIONAL running p-threads (not including the main thread)
	//This allows the main thread to not execute until every thread has finished, so that resources don;t get unexpectedly destroyed while other threads are running
	//(e.g. the Node tree doesn;t get destroyed while another thread is exeuting instructions)
	//Mutex insures that threads don;t simultaneously try to destroy themselves/add new threads
	std::vector< pthread_t > currentThreads;
	pthread_mutex_t threadCount_mutex;
	ThreadPool backgroundThreads;

	//This vector holds each MUTEX lock created by the program
	//By putting them all in one global location, we allow threads to query whether a particular mutex has been created or not
	std::map<std::string, pthread_mutex_t*> mutexes;
	pthread_mutex_t map_mutex;

	//We may want to change the constructor to an initializer, as pthread_create may return errors
	ThreadEnvironment();
	~ThreadEnvironment();

	static ThreadEnvironment instance;

public:

	//Since the threadCount is a less-visited area, we will use a coarse mutex for reading and writing
	//Atomically queries the number of registered active threads
	static int queryThreads();

	//Atomically increments the number of active threads
	static void addThread(pthread_t aThread);

	//Atomically decrements the number of active threads
	static void removeThread(pthread_t rThread);

	static pthread_t getNextJoin();

	//This could use a better name, as the name implies an oxymoron
	static void joinDetachedThreads();

	//This method returns the mutex associated with a string, or creates a new mutex associated with the string and returns that
	static pthread_mutex_t* identifyMutex(std::string mutexName);
};

/*
 * This file builds a function lookup table so that when the interpreter encounters a function     call, it can easily find the address of the appropriate node where the called function code res    ides
 *Since there is only one funciton table per program (even if using multiple files, the further     functions should be addable by calling buildTree for each file's syntax tree) There can only b    e one instance.
 * TODO: For a long time, this class has been a convoluted mix between a singleton and static m    ember class. Must query the design plans of the debugger to see whether this will ever be neede    d as an object, so we can stick with one or the other
 */
class FunctionMap {

private:
	FunctionMap();
	std::map<std::string, Node*> lookup;
	static FunctionMap instance;
	static void concatSignature(const Node*,std::string&);

public:
	//Returns the address of a node containing the function body of the function denoted by functionSignature
	static const Node* getFunctionNode(const std::string functionSignature);

	//Generates a unique function signature based on the name AND the arguments
	static const std::string getFunctionSignature(const Node* node);

	//Fills the function map given the specified base node
	static void build(const Node* tree);
};

/*
 * The classes contained in this file are used to keep track of certain properties of a program as it is being executed
 * These include the function call stack, the variables currently in scope, and where the programshould jump to should there be a break or continue statement
 * Although your standard running program utilizes one stack, this stack is represented via an object because if the eventual need for multiple call stacks through multithreading
 */


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
		T* lookupVar(std::string name) {
			return varScope.lookupVar<T>(name);
		}

		//Used for aliasing an array
		//Returns an unitialized pointer that will be associated with varName
		//The calling program can set this pointer to point to whatever varname should alias.
		TData<void*>& declareReference(const std::string varName);

		//declare a variable that can hold different values across different threads
		std::list<std::pair<pthread_t,TData<void*> > >& declareThreadSpecificVariable(const std::string&);

		//Used by loops and constrol statements to determine if they can proceed, or if they should return
		ExecutionStatus queryExecutionStatus();

		//sets the execution status to the specified value
		void setExecutionStatus(ExecutionStatus status);

		bool containsVar(std::string varName) const;

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
			std::stack<ThreadPool*> spawnedThreads;
			refCount_mutex_ptr = new pthread_mutex_t();
			addReference();
		}

		//Creates a pointer that points to a COPY of the given tetrascope. Used for initializing funciton parameters. Note that scope does not include spawned threads.
		scope_ptr(const TetraScope& newScope) {
			ptr = new TetraScope(newScope);
			refCount = new int();//Zero initialized
			*refCount = 0;
			status = NORMAL;
			std::stack<ThreadPool*> spawnedThreads;
			refCount_mutex_ptr = new pthread_mutex_t();
			addReference();
		}
		//Copy constructor aliases this Scope to the other, rather than performing a deep copy
		//Note that this is largely desired behavior
		//Note also that each Scope_ptr must have its own threadpool stack!
		scope_ptr(const scope_ptr& other) {
			//cout << "This gets waited at a few times" << endl;
			other.lockScope();
			//cout << "Access gained" << endl;
			ptr = other.ptr;
			status = other.status;
			std::stack<ThreadPool*> spawnedThreads;
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
			lockScope();
			spawnedThreads.top()->addThread(thread);
			unlockScope();
		}

		void setupParallel() {
			//Since each scope_ptr gets its own copy of spawnedThreads (even if everything else is an alias), this is threadsafe
			ThreadPool* newPool = new ThreadPool();
			
			lockScope();
			spawnedThreads.push(newPool);
			unlockScope();
		}

		void endParallel() {
			//cout << "Main finished: " << time(0) << endl;
			spawnedThreads.top()->waitTillEmpty();
			//cout << "All joined: " << time(0) << endl;
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
	//Note that this constructor starts with a GLOBAL scope. One must be initialized through initializeNewScope to have it represent local data	
	TetraContext();

	void initializeGlobalVars(const Node *);

	//This initializer function initializes a new TetraContext object so that its base scope is aliased to the current scope of the currentContext.
	//This allows threads to share the same scope while not caring where they branch
/*	void initializeContextBranch(TetraContext* newContext, TetraContext& currentContext) {
		newContext->progStack.push( currentContext.progStack.top() );
	}
*/
	//Wraps a call to lookupVar of the current scope after checking that there are no globals
	template<typename T>
	T* lookupVar(std::string name) {
		if(getGlobalScopeRef()->containsVar(name)) {
			return (getGlobalScopeRef()->lookupVar<T>(name));
		}
		else {
			return progStack.top()->lookupVar<T>(name);
		}
	}

	//Wraps a call of declareReference for the current scope
	TData<void*>& declareReference(const std::string varName);

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
	//~TetraContext();

	//Returns a reference to the current scope
	//This may be outdated
	TetraScope& getCurrentScope();

	scope_ptr& getScopeRef();

	scope_ptr& getGlobalScopeRef() {
		return *globalScope;
	}

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

	//Declares  variable that can have different values across different threads
	std::list<std::pair<pthread_t,TData<void*> > >& declareThreadSpecificVariable(const std::string&);

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
	scope_ptr* globalScope;	
};


//Header for Tetra Standard Library
void print(const Node*,TetraContext&);
int readInt();
double readReal();
bool readBool();
std::string readString();
int len(TArray&);
int len(std::string&);

/*
 * This class is used in reporting runtime errors
 */

class RuntimeError: public Error {
public:
	RuntimeError(const std::string& pMessage, int pLine, TetraContext& pContext);
	TetraContext& getContext();
private:
	TetraContext context;
};

/*
 * These types of errors are thrown when the interpreter enters an invalid state
 * e.g. the interpreter fails to find information about the types of certain variables
 */

class SystemError: public Error {
public:
	SystemError(const std::string& pMessage, int pLine, const Node* pNode);
	const Node* getNode();
private:
	const Node* node;
};

//This class allows other libraries to define how the interpreter should handle I/O
class VirtualConsole {

public:
	//Used to input standard input. Implementation should return the user input as a string
	virtual std::string receiveStandardInput() const = 0;
	//Used for standard output. Argument is a string containing what the Tetra Program is requesting to output.
	virtual void processStandardOutput(const std::string) const = 0;
};

/*
 * This class stores constants related to the environment where the program is run, including:
 * -number of allowed threads
 * -where to print for print statements
 */ 

#include"commandObserver.h"

class TetraEnvironment {
public:
	static void initialize();
	static void initialize(const VirtualConsole&);
	static void setConsole(const VirtualConsole&);
	static const VirtualConsole& getConsole();
	static int getMaxThreads();
	static void setMaxThreads(int);
	static ostream& getOutputStream();
	static void setOutputStream(ostream&);
	static CommandObserver& getObserver();
	static void setObserver(CommandObserver&);
	
private:
	static int maxThreads;
	static ostream* outputStream;
	static VirtualConsole const * console_ptr;
	static CommandObserver observer;
};

#endif
