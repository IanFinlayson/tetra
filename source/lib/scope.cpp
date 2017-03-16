/* scope.cpp
 * the Scope class keeps track of variables which belong to a given function
 * (or globals in case of the global scope object) */

#include "tetra.h"

Scope::Scope() {
    executionStatus = NORMAL;
    isParallel = false;
}

/* look a variable up in this scope by name */
Data* Scope::lookupVar(const String& name, DataType* type, Context* context) {
    Data* var = NULL;
    if (isParallel) {
        varMutex.lock();
    }

    /* first check if this is a parallel for loop variable */
    auto search = parallelForVariables.find(name);
    if (search != parallelForVariables.end()) {
        /* it is a parfor variable, we need to look it up by thread id in this
         * context through parent and grand-parent contexts until it's found */
        Context* current = context;

        while (current != NULL) {
            /* check if this context has a handle to it */
            auto search2 = search->second.find(current->getThreadId());
            if (search2 != search->second.end()) {
                /* if so, look it up and break out */
                var = search->second[current->getThreadId()];
                break;
            }

            /* it must not be here, go on to the next one */
            current = current->getParent();
        }

        /* if var is NULL, we somehow did not find it */
        if (var == NULL) {
            throw Error("Was not able to look up parallel for variable " + name, 0);
        }

    } else {
        /* it's not a parallel for variable, just look it up like normal */
        var = varScope.lookupVar(name, type);
    }

    if (isParallel) {
        varMutex.unlock();
    }
    return var;
}

/* Used by loops and constrol statements to determine if they can proceed, or
 * if they should return */
ExecutionStatus Scope::queryExecutionStatus() {
    return executionStatus;
}

/* mark the scope as being parallel */
void Scope::markParallel() {
    isParallel = true;
}

/* sets the execution status to the specified value */
void Scope::setExecutionStatus(ExecutionStatus status) {
    executionStatus = status;
}

bool Scope::containsVar(const String& varName) const {
    return varScope.containsVar(varName);
}

/* Used by the TetraContext to obtain a stack trace */
void Scope::setCallNode(const Node* node) {
    callNode = node;
}

/* called when we are starting a new parallel for loop on some variable in this scope */
void Scope::setupParallelFor(const String& variable) {
    varMutex.lock();

    /* see if it's not already here */
    auto search = parallelForVariables.find(variable);
    if (search == parallelForVariables.end()) {
        /* insert it */
        std::map<unsigned int, Data*> newOne;
        parallelForVariables.insert(std::make_pair(variable, newOne));
    }

    varMutex.unlock();
}

/* called when we are assigning the parallel for variable for a thread */
void Scope::assignParallelFor(const String& variable, unsigned int threadid, Data* value) {
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

// TODO FIXME for debugging
void pmap(std::map<String, std::map<unsigned int, Data*> > parallelForVariables) {
    for (auto it : parallelForVariables) {
        std::cout << it.first << ":\n";
        for (auto it2 : it.second) {
            std::cout << "\t" << it2.first << " : " << it2.second->getValue()->toString() << "\n";
        }
    }
}
