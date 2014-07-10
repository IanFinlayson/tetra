#ifndef VARIABLE_CONTEXT_H
#define VARIABLE_CONTEXT_H
/*
 * This class represents the variables present at various scopes within a tetra program
 */

#include<iostream>
#include<map>
#include<string>
#include"tData.h"

#include<pthread.h>

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

private:
	
	std::map<string, TData<void*> > varMap;
	pthread_mutex_t table_mutex;
};
//Returns a reference to the storage location of the variable
////defined in the header because it is a template
template<typename T>
T* VarTable::lookupVar(const string varName) {
	//pthread_mutex_lock(&table_mutex);
	//check whether an entry exists for this variable
	if(varMap.find(varName) == varMap.end()) {
		//If the variable does not yet exist, we need to allocate memory for the TData to point to!
		T* newData_ptr = new T(); //() should zero the memory, even for primitive types
		TData<void*> insertable(newData_ptr);
		//Must notify TData that it is pointing at dynamically allocated memory
		insertable.setDeletableType<T>();

		varMap[varName] = insertable;
	}
	   
	//T* ret = static_cast<T*>(varMap[varName].getData());

	//pthread_mutex_unlock(&table_mutex);

	return static_cast<T*>(varMap[varName].getData());
	
	//return ret;
}

#endif
