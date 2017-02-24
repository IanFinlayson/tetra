/* scope.h
 * the Scope class keeps track of variables which belong to a given function
 * (or globals in case of the global scope object) */

#ifndef SCOPE_H
#define SCOPE_H

#include <list>
#include <map>

#include "vartable.h"
#include "strings.h"

class Node;
class ParallelWorker;

enum ThreadStatus { RUNNING, STOPPED, DESTROYED, WAITING };

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
 * function call. This takes precedence over breaks and continues
 *
 * PARALLEL: each statement node encountered will result in a new spwned thread.
 * Note that for conditionals and loops, only one thread will be spawned to
 * evaluate the bodies of the conditionals/loops */
enum ExecutionStatus { NORMAL, ELIF, CONTINUE, BREAK, RETURN, PARALLEL };

/* a scope contains local variables of a function */
class Scope {
   public:
    Scope() {
        executionStatus = NORMAL;
        numThreads = 0;
    }

    /* look a variable up in this scope by name */
    Data* lookupVar(String name, DataType* type) {
        Data* var;
        if (numThreads >= 1) {
            varMutex.lock();
        }

        var = varScope.lookupVar(name, type);
        
        if (numThreads >= 1) {
            varMutex.unlock();
        }
        return var;
    }

    /* Used for aliasing an array
     * Returns an uninitialized pointer that will be associated with varName
     * The calling program can set this pointer to point to whatever varname
     * should alias. */
    Data& declareReference(const String varName);

    /* Used by loops and constrol statements to determine if they can proceed, or
     * if they should return */
    ExecutionStatus queryExecutionStatus() {
        return executionStatus;
    }

    /* sets the execution status to the specified value */
    void setExecutionStatus(ExecutionStatus status) {
        executionStatus = status;
    }

    /* add a background thread into this scope */
    void incrementBackgroundThreads() {
        numThreads++;
    }
    void decrementBackgroundThreads() {
        numThreads--;
    }

    /* return number of threads in the scope */
    int getNumThreads() const {
        return numThreads;
    }

    bool containsVar(const String& varName) const {
        return varScope.containsVar(varName);
    }

    /* Used by the TetraContext to obtain a stack trace */
    void setCallNode(const Node* node) {
        callNode = node;
    }

    const Node* getCallNode() const;

   private:
    VarTable varScope;
    ExecutionStatus executionStatus;

    /* the number of threads active in this scope */
    int numThreads;

    /* mutex for protecting variables when in parallel mode */
    QMutex varMutex;

    /* by storing the address of the call node, we can print back a call stack to
     * the user if the program terminates unexpectedly */
    const Node* callNode;
};

#endif
