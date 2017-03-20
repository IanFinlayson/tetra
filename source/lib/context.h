/* context.h
 * this file declares the Context class which keeps track of variables in scope(s)
 * as the program runs */

#ifndef CONTEXT_H
#define CONTEXT_H

#include <cassert>
#include <stack>
#include <QMutex>

#include "values.h"
#include "scope.h"
#include "symbol.h"
#include "function.h"
#include "data.h"

class Node;

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
enum ExecutionStatus { NORMAL, ELIF, CONTINUE, BREAK, RETURN };

/* this class contains a stack of scopes, as well as information on the currently
 * executing program */
class Context {
   public:
    /* note that this constructor starts with a GLOBAL scope. One must be */
    /* initialized through initializeNewScope to have it represent local data */
    Context();

    ~Context();

    void initializeGlobalVars(const Node*);

    /* find an overloaded function by name and arguments */
    Data* findOverload(Node* functionCall);

    /* lookup a variable  in the present context */
    Data* lookupVar(String name, DataType* type);

    /* the first is for when there is no initial setup for a scope
       (i.e. a function call with no formal parameters that must be
       initialized) the second is for adding a scope which had to have some
       data preloaded into it, as is the case when calling a function with
       arguments */
    void initializeNewScope();
    void initializeNewScope(Scope* newScope);

    /* pops the current scope off the stack. Has the effect of destroying all variables of the
     * present scope */
    void exitScope();

    /* returns a reference to the current scope */
    Scope* getCurrentScope() {
        return programStack.top();
    }

    Scope* getScopeRef();

    Scope* getGlobalScopeRef() {
        return globalScope;
    }

    ExecutionStatus queryExecutionStatus() {
        return executionStatus;
    }

    /* sets the ExecutionStatus to the appropriate value */
    void notifyBreak() {
        executionStatus = BREAK;
    }
    void notifyContinue() {
        executionStatus = CONTINUE;
    }
    void notifyReturn() {
        executionStatus = RETURN;
    }
    void notifyElif() {
        executionStatus = ELIF;
    }
    void normalizeStatus() {
        executionStatus = NORMAL;
    }
    void setStatus(ExecutionStatus s) {
        executionStatus = s;
    }

    /* set this context's current scopes as being parallel */
    void markParallel() {
        globalScope->markParallel();
        programStack.top()->markParallel();
    }

    /* performs a deep copy of the current context */
    Context& operator=(const Context&);

    /* get and set the thread id */
    void setThreadId(int threadId);
    int getThreadId() const;

    /* get and set the parent context */
    void setParent(Context* parent);
    Context* getParent() const;

   private:
    /* the scopes avaiable at this time */
    std::stack<Scope*> programStack;
    Scope* globalScope;

    /* the thread id of the thread in this context */
    int threadId;

    /* the context which spawned this one */
    Context* parent;

    /* the execution status of this context */
    ExecutionStatus executionStatus;
};

#endif
