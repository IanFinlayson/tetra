/*
 * This class represents the variables present at various scopes within a tetra program
 */

#include<iostream>
#include<map>
#include<string>
#include"frontend.h"
#include"backend.h"
#include<pthread.h>
#include<list>
using std::string;

VarTable::VarTable() : varMap(30),  parForVars(3){
  //pthread_mutex_init(&table_mutex,NULL);
  pthread_rwlock_init(&table_mutex,NULL);
}

//Release allocated data should be handled by each TData as it is destructed
//This assumes each TData in the table had its setDeletableType method called
VarTable::~VarTable() {
  pthread_rwlock_destroy(&table_mutex);
}

//Declares a variable name that can hold different values across threads
//Numthreads needed so the container does not attempt to resize itself during
std::list<std::pair<pthread_t,TData<void*> > >& VarTable::declareParForVar(const string& varName) {

  //We will check if the variable already exists. To do so, we must obtain a read lock
  pthread_rwlock_rdlock(&table_mutex);

  //Check to make sure we don;t already have a value declared
  //This could happen if, say, someone declares a parfor within a loop, in which case we should append to that array
  if(std::find_if(parForVars.begin(),parForVars.end(),CheckName(varName)) != parForVars.end()) {
    //return std::find_if(parForVars.begin(),parForVars.end(),CheckName(varName))->second;
    //Assemble the return value, so we can release the lock before returning

    std::list<std::pair<pthread_t,TData<void*> > >& ret = std::find_if(parForVars.begin(),parForVars.end(),CheckName(varName))->second;

    pthread_rwlock_unlock(&table_mutex);

    return ret;
  }

  //If not found, we will have to release the mutex anyways before trying to obtain write privelages
  pthread_rwlock_unlock(&table_mutex);

  std::list<std::pair<pthread_t,TData<void*> > > array;
  //Append to the end of the array, so we can return the end

  //This must be done under the lock of a mutex, in case another thread starts a ParFor in this scope
  //pthread_mutex_lock(&table_mutex);
  pthread_rwlock_wrlock(&table_mutex);

  //While we were waiting for the lock, another thread may have pushed this variable into the list!
  if(std::find_if(parForVars.begin(),parForVars.end(),CheckName(varName)) != parForVars.end()) {
    //Release the mutex before returning
    pthread_rwlock_unlock(&table_mutex);
    return std::find_if(parForVars.begin(),parForVars.end(),CheckName(varName))->second;
  }

  parForVars.push_back(std::pair<string, std::list<std::pair<pthread_t,TData<void*> > > >(varName, array));
  //Note that with this syntax, it is illegal to modify the array so as to invalidate this pointer
  std::list<std::pair<pthread_t,TData<void*> > >* ret_ptr = &parForVars.rbegin()->second;
  //pthread_mutex_unlock(&table_mutex);
  pthread_rwlock_unlock(&table_mutex);

  return *ret_ptr;

}

//Checks to see if varName is in this scope, but does not add it if it does not find it
bool VarTable::containsVar(std::string varName) const{
  //return varMap.find(varName) != varMap.end();
  return varMap.exists(varName);
}

bool VarTable::containsVar(const Node* varNode) const{
  return varMap.exists(varNode);
}


//Adds a reference to the table
//i.e. Adding in the varName does not cause any new objects to be created (unlike lookupVar which creates one if it does not yet exist)
//Also, the data is not marked as deletable, so the referenced data will not be destroyed when this data goes out of scope
//Used primarily for passing vectors to functions/for-loops
//Returns the reference once it creates it, so it can be set to reference the proper thing
/*
   TData<void*>& VarTable::declareReference(const string varName) {

//Sets the data for what we will insert as the location of the actual data being referenced
TData<void*> inserter;
//Note that setDeletableType is not called, because this object going out of scope should not result in the data getting deleted
//Container copy-constructs the TData
pthread_mutex_lock(&table_mutex);
varMap[varName] = inserter;
pthread_mutex_unlock(&table_mutex);
//This returns the copy in the map, as opposed to the local var inserter
return varMap[varName];
}
*/
