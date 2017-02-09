/* context.cpp
 * this file implements the Context class which keeps track of variables in the
 * scopes of the running program */

#include "tetra.h"

Context::Context() {
    /* initialize the global scope */
    globalScope = new Scope();
    programStack.push(globalScope);
}

/* set up the global variables from the parse tree */
void Context::initializeGlobalVars(const Node* tree) {
    /* Traverse the tree */
    if (tree->kind() == NODE_TOPLEVEL_LIST) {
        Node* candidate = tree->child(0);
        if (candidate->kind() == NODE_GLOBAL || candidate->kind() == NODE_CONST) {
            /* perform assignment at this global scope */
            evaluateStatement(candidate, this);
        }

        if (tree->child(1) != NULL) {
            initializeGlobalVars(tree->child(1));
        }
    }
}

/* initializes an empty scope and sets that as the current scope */
void Context::initializeNewScope() {
    Scope* newScope = new Scope();
    programStack.push(newScope);
}

/* takes the given scope and sets it as the current scope this allows local
data to get passed in */
void Context::initializeNewScope(Scope* newScope) {
    programStack.push(newScope);
}

/* destroys the current scope, returning to the previously initialized scope */
void Context::exitScope() {
    Scope* scope = programStack.top();
    delete scope;
    programStack.pop();
}

/* If, for some reason the tetra program crashes inadvertantly, we may as well */
/* clean up the Context stack */
Context::~Context() {}
