#ifndef VARTABLE_H
#define VARTABLE_H

#include <map>

class VarTable {
   public:
    /* returns a pointer to this variable, creating it if needed */
    Tdata* lookupVar(const Tstring& name) {
        return variables[name];
    }

   private:
    std::map<Tstring, Tdata*> variables;
};

#endif
