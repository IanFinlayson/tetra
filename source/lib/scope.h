/* scope.h
 * the Scope class keeps track of variables which belong to a given function
 * (or globals in case of the global scope object) */

#ifndef SCOPE_H
#define SCOPE_H

#include <list>
#include <map>

#include "vartable.h"
#include "strings.h"
#include "error.h"

class Node;
class ParallelWorker;
class Context;

/* a scope contains local variables of a function */
class Scope {
   public:
    Scope();

    /* look a variable up in this scope by name */
    Data* lookupVar(const String& name, DataType* type, Context* context);

    /* mark the scope as being parallel */
    void markParallel();

    bool containsVar(const String& varName) const;

    /* Used by the TetraContext to obtain a stack trace */
    void setCallNode(const Node* node);

    /* called when we are starting a new parallel for loop on some variable in this scope */
    void setupParallelFor(const String& variable);

    /* called when we are assigning the parallel for variable for a thread */
    void assignParallelFor(const String& variable, unsigned int threadid, Data* value);

    /* called when we are done with a parallel for variable i.e. the loop is done */
    void clearParallelFor(const String& variable);

   private:
    VarTable varScope;

    /* whether this is a parallel scope */
    bool isParallel;

    /* mutex for protecting variables when in parallel mode */
    QMutex varMutex;

    /* by storing the address of the call node, we can print back a call stack to
     * the user if the program terminates unexpectedly */
    const Node* callNode;

    /* each scope keeps track of a parallel for variables in it
     * we map them by name and thread id, to the actual data */
    std::map<String, std::map<unsigned int, Data*> > parallelForVariables;
};

#endif
