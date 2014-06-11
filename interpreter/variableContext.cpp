/*
 * This class represents the variables present at various scopes within a tetra program
 */

#include<iostream>
#include<map>
#include<string>
#include"frontend.hpp"
#include"tArray.h"
#include"tData.cpp"
#include"variableContext.h"

using std::string;

VarTable::VarTable() {
	//nothing needed at the moment
}

//Release allocated data
VarTable::~VarTable() {
	std::cout << "destructor called" << std::endl;

	std::map<string,TData<void*> >::iterator iter;
	for(iter = varMap.begin(); iter != varMap.end();iter++) {
		std::cout << "deleting: " << iter->first << std::endl;
		//Use a pointer, so we don;t have to copy the opbject to get where it points
		const TData<void*>* next = &(iter->second);
		switch(next->getPointedTo().getKind()) {
			case TYPE_INT:
				std::cout << "Had value: " << *static_cast<int*>(next->getData()) << std::endl;
				delete static_cast<int*>(next->getData());
			break;
			case TYPE_REAL:
				std::cout << "Had value: " << *static_cast<double*>(next->getData()) << std::endl;			
				delete static_cast<double*>(next->getData());
			break;
			case TYPE_BOOL:
				std::cout << "Had value: " << *static_cast<bool*>(next->getData()) << std::endl;			
				delete static_cast<bool*>(next->getData());
			break;
			case TYPE_STRING:
				std::cout << "Had value: " << *static_cast<string*>(next->getData()) << std::endl;			
				delete static_cast<string*>(next->getData());
			break;
			case TYPE_VECTOR:
				std::cout << "Deleting vector...Had values" << endl;
				delete static_cast<TArray*>(next->getData());
			break;
			default:
				std::cout << "nothing deleted, possible mem leak" << std::endl;
				break;

		}
		varMap.erase(iter);
	}
	std::cout << "Destructor Finished" << std::endl;
}

