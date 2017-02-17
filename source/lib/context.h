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

    /* wraps a call to the current scope's queryExecutionStatus */
    ExecutionStatus queryExecutionStatus() {
        assert(programStack.empty() == false);
        return programStack.top()->queryExecutionStatus();
    }

    /* sets the current scope's ExecutionStatus to the appropriate value */
    void notifyBreak() {
        programStack.top()->setExecutionStatus(BREAK);
    }
    void notifyContinue() {
        programStack.top()->setExecutionStatus(CONTINUE);
    }
    void notifyReturn() {
        programStack.top()->setExecutionStatus(RETURN);
    }
    void notifyElif() {
        programStack.top()->setExecutionStatus(ELIF);
    }

    void notifyParallel() {
        globalScope->setExecutionStatus(PARALLEL);
        programStack.top()->setExecutionStatus(PARALLEL);
    }

    /* add a background thread to each of the scopes we are in */
    void incrementBackgroundThreads() {
        globalScope->incrementBackgroundThreads();
        programStack.top()->incrementBackgroundThreads();
    }

    void decrementBackgroundThreads() {
        globalScope->decrementBackgroundThreads();
        programStack.top()->decrementBackgroundThreads();
    }

    /* sets the current scope's executionStatus to NORMAL */
    void normalizeStatus() {
        programStack.top()->setExecutionStatus(NORMAL);
    }

    /* performs a deep copy of the current context */
    Context& operator=(const Context&);

   private:
    /* the scopes avaiable at this time */
    std::stack<Scope*> programStack;
    Scope* globalScope;
};

#endif
