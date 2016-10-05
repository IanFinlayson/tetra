/* vartable.h
 * the variable table just stores a map of variables accessed by name */

#ifndef VARTABLE_H
#define VARTABLE_H

#include <map>

class Data;
class DataType;
class String;

class VarTable {
   public:
    /* returns a pointer to this variable, creating it if needed */
    Data* lookupVar(const String& name, DataType* type);

    /* checks if a variable exists in the table */
    bool containsVar(const String& name) const;

   private:
    std::map<String, Data*> variables;
};

#endif
