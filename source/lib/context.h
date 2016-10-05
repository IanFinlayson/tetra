/* context.h
 * this file declares the Context class which keeps track of variables in scope(s)
 * as the program runs */

#ifndef CONTEXT_H
#define CONTEXT_H

#include <cassert>
#include <stack>

#include "values.h"
#include "scope.h"

class Node;

/* this class contains a stack of scopes, as well as information on the currently
 * executing program */
class Context {
   public:
    /* note that this constructor starts with a GLOBAL scope. One must be */
    /* initialized through initializeNewScope to have it represent local data */
    Context();
    Context(long);

    /*	Context(const Context&); */

    ~Context();

    void initializeGlobalVars(const Node*);

    /* lookup a variable  in the present context */
    Data* lookupVar(String name, DataType* type) {
        if (getGlobalScopeRef().containsVar(name)) {
            return (getGlobalScopeRef().lookupVar(name, type));
        } else {
            return programStack.top().lookupVar(name, type);
        }
    }

    /* overloaded function call, one when there is no initial setup for a scope
       (i.e. a function call with no formal parameters that must be initialized)
       The second is for adding a scope which had to have some data preloaded into
       it, as is the case when calling a function with arguments */
    void initializeNewScope(const Node* callNode);
    void initializeNewScope(Scope& newScope);

    /* pushes an alias to the given scope onto the stack used for multithreading */
    void branchOff(const Scope& baseScope, Scope* globScope);

    /* pops the current scope off the stack. Has the effect of destroying all variables of the
     * present scope */
    void exitScope();

    /* returns a reference to the current scope */
    Scope& getCurrentScope() {
        return programStack.top();
    }

    Scope& getScopeRef();

    Scope& getGlobalScopeRef() {
        return *globalScope;
    }

    /* wraps a call to the current scope's queryExecutionStatus */
    ExecutionStatus queryExecutionStatus() {
        assert(programStack.empty() == false);
        return programStack.top().queryExecutionStatus();
    }

    /* sets the current scope's ExecutionStatus to the appropriate value */
    void notifyBreak() {
        programStack.top().setExecutionStatus(BREAK);
    }
    void notifyContinue() {
        programStack.top().setExecutionStatus(CONTINUE);
    }
    void notifyReturn() {
        programStack.top().setExecutionStatus(RETURN);
    }
    void notifyElif() {
        programStack.top().setExecutionStatus(ELIF);
    }

    void notifyParallel();

    /* sets the current scope's executionStatus to NORMAL */
    void normalizeStatus() {
        programStack.top().setExecutionStatus(NORMAL);
    }

    /* declares  variable that can have different values across different threads */
    std::list<std::pair<pthread_t, Data> >& declareThreadSpecificVariable(const String&);

    /* performs a deep copy of the current context */
    Context& operator=(const Context&);

    /* methods dealing with parallelism at a contextual level */
    void addThread(pthread_t);
    void setupParallel();
    void endParallel();

    long getThreadID();

    /* for use when debugging */
    int getLastLineNum();
    void* fetchVariable(String s);
    std::map<String, int>& getRefTable() {
        return referenceTables->top();
    }
    std::map<String, int>& getGlobRefTable() {
        return *globalReferenceTable;
    }
    void updateVarReferenceTable(const Node* node);
    void popReferenceTable();

    /* get and set methods for debug flags (inline) */
    int getLastLineNo() {
        return lastLineNo;
    }
    bool getStepping() {
        return stepping;
    }
    bool getStopAtNext() {
        return stopAtNext;
    }
    bool getResume() {
        return resume;
    }
    ThreadStatus getRunStatus() {
        return runStatus;
    }
    bool isParallelForVariable(String);

    void setLastLineNo(int pLast) {
        lastLineNo = pLast;
    }
    void setStepping(bool steppingArg) {
        stepping = steppingArg;
    }
    void setStopAtNext(bool pStopAtNext) {
        stopAtNext = pStopAtNext;
    }
    void setResume(bool pResume) {
        resume = pResume;
    }
    void setRunStatus(ThreadStatus pStatus) {
        runStatus = pStatus;
    }
    void registerParallelForVariable(String);

    /* used to give debug info to newly created threads */
    void copyDebugInfo(Context* baseContext) {
        *parForVars = *(baseContext->parForVars);
        *referenceTables = *(baseContext->referenceTables);
        *globalReferenceTable = *(baseContext->globalReferenceTable);
        scopes->push(baseContext->scopes->top());
    }

    std::stack<const Node*>& getScopes() {
        return *scopes;
    }

    /* prints a stack trace */
    void printStackTrace() const;

   private:
    std::stack<Scope> programStack;
    Scope* globalScope;

    long threadID;
    int lastLineNo;
    bool stepping;
    bool stopAtNext;
    bool resume;

    std::stack<const Node*>* scopes;

    std::stack<std::map<String, int> >* referenceTables;

    std::map<String, int>* globalReferenceTable;

    ThreadStatus runStatus;

    pthread_mutex_t parallelList_mutex;
    std::vector<String>* parForVars;
};

#endif
