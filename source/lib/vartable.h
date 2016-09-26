#ifndef VARTABLE_H
#define VARTABLE_H

#include <map>

class VarTable {
   public:
    /* returns a pointer to this variable, creating it if needed */
    Tdata* lookupVar(const Tstring& name, DataType* type) {
        /* if not here, create it */
        if (variables.find(name) == variables.end()) {
            variables[name] = Tdata::create(type, NULL);
        }

        return variables[name];
    }

    bool containsVar(const Tstring& name) const {
        return variables.find(name) != variables.end();
    }

   private:
    std::map<Tstring, Tdata*> variables;
};

#endif
