/*
 * This class represents the variables present at various scopes within a tetra program
 */

#include<iostream>
#include<map>
#include<string>
#include"frontend.hpp"
#include"tArray.h"
#include"tData.h"
#include"variableContext.h"

using std::string;

VarTable::VarTable() {
	//nothing needed at the moment
}

//Release allocated data should be handled by each TData as it is destructed
//This assumes each TData in the table had its setDeletableType method called
VarTable::~VarTable() {
}



//Adds a reference to the table
//i.e. Adding in the varName does not cause any new objects to be created (unlike lookupVar which creates one if it does not yet exist)
//Also, the data is not marked as deletable, so the referenced data will not be destroyed when this data goes out of scope
//Used primarily for passing vectors to functions/for-loops
//Returns the reference once it creates it, so it can be set to reference the proper thing

TData<void*>& VarTable::declareReference(const string varName) {

        //Sets the data for what we will insert as the location of the actual data being referenced
        TData<void*> inserter;
        //Note that setDeletableType is not called, because this object going out of scope should not result in the data getting deleted
        //Container copy-constructs the TData
        varMap[varName] = inserter;
        //This returns the copy in the map, as opposed to the local var inserter
        return varMap[varName];
}

