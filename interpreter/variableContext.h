#ifndef VARIABLE_CONTEXT_H
#define VARIABLE_CONTEXT_H
/*
 * This class represents the variables present at various scopes within a tetra program
 */

#include<iostream>
#include<map>
#include<string>
#include"tData.h"
#include<algorithm> //for find_if
#include<list>

#include<pthread.h>

#include<sstream>

using std::string;

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
	T* lookupVar(const string varName);

	//Returns a TData from within varMap containing an invalid reference. The reference can be set to something valid, and that thing will not be deleted when this variableContext goes out of scope
	TData<void*>& declareReference(const string varName);

	std::list<std::pair<pthread_t,TData<void*> > >& declareParForVar(const string&);
private:
	
	std::map<string, TData<void*> > varMap;
	pthread_mutex_t table_mutex;

	//This is a std::vector containing pairs associating strings with another vector containing pairs that associate a thread with a TData value
	//This is used in the context of parallel for loops, where the lookup function can:
	//1) check if the outer vector contains a string matching the given lookup value
	//2) if so, obtain the vector for that value
	//3) search the pairs of the inner vector to see if the calling thread has an entry for that variable
	//4) return the correct variable for that thread
	std::list< std::pair<string, std::list<std::pair<pthread_t,TData<void*> > > > > parForVars;

	//Predicate functions for the lookupVar function
	//This code used from stack overflow question 12008059
	struct CheckName {
		CheckName(string pVal) : searchVal(pVal) {
		}
		bool operator()(std::pair<string, std::list<std::pair<pthread_t,TData<void*> > > > val) {
			return searchVal == val.first;
		}
		private:
		string searchVal;
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
T* VarTable::lookupVar(const string varName) {
	pthread_mutex_lock(&table_mutex);
	
	//Check whether this variable is a parallel for variable. Ideally there won;t be many of these floating around, but we can implement a non-linear algorithm later if needed
	std::list< std::pair<string, std::list<std::pair<pthread_t,TData<void*> > > > >::iterator loc;
	loc = std::find_if(parForVars.begin(), parForVars.end(), CheckName(varName));
	if(loc != parForVars.end()) {
		std::list< std::pair< pthread_t, TData<void*> > >& varList = (*loc).second;
		std::list< std::pair< pthread_t, TData<void*> > >::iterator value;
		value = std::find_if(varList.begin(), varList.end(), CheckThread(pthread_self()));
		if(value != varList.end()) {

			T* ret = static_cast<T*>(value->second.getData());
			pthread_mutex_unlock(&table_mutex);
			return ret;
		}
		else {	//If this thread has not yet registered a value, create it
			T* newData_ptr = new T();
			//cout << "New Data: " << newData_ptr << endl;
			TData<void*> insertable(newData_ptr);
			//Ensure that insertable gets copied correctly, and deleted correctly when we are done
			insertable.setDeletableType<T>();
			varList.push_back(std::pair<pthread_t,TData<void*> >(pthread_self(), insertable));
			(varList.rbegin())->second.setDeletableType<T>();
			T* ret = static_cast<T*>((varList.rbegin())->second.getData());
			pthread_mutex_unlock(&table_mutex);
			//cout << "Ret: " << ret << endl;
			return ret;
		}
	}
	//check whether a normal entry exists for this variable
	//The find and [] operators are redundant. we can combine them into one search
	else if(varMap.find(varName) == varMap.end()) {
		//If the variable does not yet exist, we need to allocate memory for the TData to point to!
		T* newData_ptr = new T(); //() should zero the memory, even for primitive types
		TData<void*> insertable(newData_ptr);
		//Must notify TData that it is pointing at dynamically allocated memory
		insertable.setDeletableType<T>();

		varMap[varName] = insertable;
	}
	   
	T* ret = static_cast<T*>(varMap[varName].getData());

	pthread_mutex_unlock(&table_mutex);

	//return static_cast<T*>(varMap[varName].getData());
	
	return ret;
}

#endif
