
#ifndef SCOPE_H
#define SCOPE_H

#include <list>
#include "vartable.h"

enum ThreadStatus { RUNNING, STOPPED, DESTROYED, WAITING };

/* macro to squelch unused variable warnings */
#define UNUSED(x) (void) x;

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
    Scope() {}

    Scope(const Node* node) {
        UNUSED(node);
    }

    /* look a variable up in this scope by name */
    Tdata* lookupVar(Tstring name) {
        return varScope.lookupVar(name);
    }

    /* Used for aliasing an array
     * Returns an uninitialized pointer that will be associated with varName
     * The calling program can set this pointer to point to whatever varname
     * should alias. */
    Tdata& declareReference(const Tstring varName);

    /* declare a variable that can hold different values across different threads */
    std::list<std::pair<pthread_t, Tdata> >& declareThreadSpecificVariable(const Tstring&);

    /* Used by loops and constrol statements to determine if they can proceed, or
     * if they should return */
    ExecutionStatus queryExecutionStatus() {
        return executionStatus;
    }

    /* sets the execution status to the specified value */
    void setExecutionStatus(ExecutionStatus status);

    bool containsVar(Tstring varName) const;
    bool containsVar(const Node* varNode) const;

    /* Used by the TetraContext to obtain a stack trace */
    void setCallNode(const Node*);
    const Node* getCallNode() const;

   private:
    VarTable varScope;
    ExecutionStatus executionStatus;

    /* This boolean denotes that there are multiple threads working in the current
     * scope.
     * While that is the case, Insertions into the scope's VarTable must be
     * performed in a threadsafe manner */
    bool multiThreaded;

    /* By storing the address of the call node, we can print back a call stack to
     * the user if the program terminates unexpectedly */
    const Node* callNode;
};

#endif
