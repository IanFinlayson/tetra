#ifndef VARIABLE_CONTEXT_H
#define VARIABLE_CONTEXT_H
/*
 * This class represents the variables present at various scopes within a tetra program
 */

#include<iostream>
#include<map>
#include<string>
#include"tData.cpp"

using std::string;

class VarTable {

public:
	
	VarTable();

	//Release allocated data
	~VarTable();

	//returns a reference to the storage location of the variable. The interpreter supplies the expected type. At present, this results in a memory leak until we do a little extra design
	template<typename T>
	T* lookupVar(string varName);

private:

	std::map<string, TData<void*> > varMap;

};
//Returns a reference to the storage location of the variable
////defined in the header because it is a template
template<typename T>
T* VarTable::lookupVar(string varName) {

	//check whether an entry exists for this variable
	if(varMap.find(varName) == varMap.end()) {
		//Althought the container would insert a default TData, we need to allocate memory for the TData to point to!
		//T released manually in destructor
		T* newData_ptr = new T;

		TData<void*> insertable(newData_ptr);
		insertable.setDeletableType<T>();

		varMap[varName] = insertable;
	}   

	return static_cast<T*>(varMap[varName].getData());

}


#endif
