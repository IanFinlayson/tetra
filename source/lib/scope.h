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

/* each context will have a flag as to what action should be taken when control reaches a structure
 * Node
 *
 * NORMAL: continue as usual
 *
 * ELIF: Denotes that program control is in an elif chain. Keep evaluating
 * conditions while this is true. Note that if the program ever needs to change
 * to any other mode, it means that some condition was true (and hence no longer
 * has an ELIF execution status).
 *
 * CONTINUIE: Keep returning until a loop is hit, then reevaluate the loop node
 *
 * BREAK: keep returning until you hit a loop node, then return from that node
 *
 * RETURN: keep returning until you hit a function call, then return from the
 * function call. This takes precedence over breaks and continues */
enum ExecutionStatus {NORMAL, ELIF, CONTINUE, BREAK, RETURN};

/* a scope contains local variables of a function */
class Scope {
   public:
    Scope() {
        executionStatus = NORMAL;
        isParallel = false;
    }

    /* look a variable up in this scope by name */
    Data* lookupVar(const String& name, DataType* type, unsigned int threadid) {
        Data* var;
        if (isParallel) {
            varMutex.lock();
        }

        /* first check if this is a parallel for loop variable */
        auto search = parallelForVariables.find(name);
        if (search != parallelForVariables.end()) {
            /* look it up by thread id */
            var = search->second[threadid];
        } else {
            /* just look it up in the normal place */
            var = varScope.lookupVar(name, type);
        }

        if (isParallel) {
            varMutex.unlock();
        }
        return var;
    }

    /* Used by loops and constrol statements to determine if they can proceed, or
     * if they should return */
    ExecutionStatus queryExecutionStatus() {
        return executionStatus;
    }

    /* mark the scope as being parallel */
    void markParallel() {
        isParallel = true;
    }

    /* sets the execution status to the specified value */
    void setExecutionStatus(ExecutionStatus status) {
        executionStatus = status;
    }

    bool containsVar(const String& varName) const {
        return varScope.containsVar(varName);
    }

    /* Used by the TetraContext to obtain a stack trace */
    void setCallNode(const Node* node) {
        callNode = node;
    }

    const Node* getCallNode() const;

    /* called when we are starting a new parallel for loop on some variable in this scope */
    void setupParallelFor(const String& variable) {
        varMutex.lock();
        std::map<unsigned int, Data*> newOne;
        parallelForVariables.insert(std::make_pair(variable, newOne));
        varMutex.unlock();
    }

    /* called when we are assigning the parallel for variable for a thread */
    void assignParallelFor(const String& variable, unsigned int threadid, Data* value) {
        /* find the sub map for this variable */
        varMutex.lock();
        auto search = parallelForVariables.find(variable);

        if (search == parallelForVariables.end()) {
            throw Error("Could not assign parallel for variable");
        } else {
            /* insert/overwrite this thread/value pairing */
            search->second[threadid] = value;
        }
        varMutex.unlock();
    }

    /* called when we are done with a parallel for variable i.e. the loop is done */
    void clearParallelFor(const String& variable) {
        /* find the sub map for this variable */
        varMutex.lock();
        auto search = parallelForVariables.find(variable);
        if (search == parallelForVariables.end()) {
            throw Error("Could not assign parallel for variable");
        } else {
            /* remove this map */
            parallelForVariables.erase(search);
        }
        varMutex.unlock();
    }

   private:
    VarTable varScope;
    ExecutionStatus executionStatus;

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
