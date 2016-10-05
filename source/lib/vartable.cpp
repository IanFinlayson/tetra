/* vartable.cpp
 * implements the variable table class */

#include "tetra.h"

Data* VarTable::lookupVar(const String& name, DataType* type) {
    /* if not here, create it */
    if (variables.find(name) == variables.end()) {
        variables[name] = Data::create(type, NULL);
    }

    return variables[name];
}

bool VarTable::containsVar(const String& name) const {
    return variables.find(name) != variables.end();
}
