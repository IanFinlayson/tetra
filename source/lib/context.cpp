/* context.cpp
 * this file implements the Context class which keeps track of variables in the
 * scopes of the running program */

#include "tetra.h"

Context::Context() {
    /* initialize the global scope */
    globalScope = new Scope();
    programStack.push(globalScope);

    /* debug variables */
    stopAtNext = false;
    stepping = false;
    resume = false;
    lastLineNo = -1;

    /* Only need to instantiate debug info if in debug mode */
    if (Environment::isDebugMode()) {
        parForVars = new std::vector<String>();
        scopes = new std::stack<const Node*>();
        globalReferenceTable = new std::map<String, int>();
        referenceTables = new std::stack<std::map<String, int> >();
    } else {
        parForVars = NULL;
        scopes = NULL;
        referenceTables = NULL;
        globalReferenceTable = NULL;
    }
}

/* set up the global variables from the parse tree */
void Context::initializeGlobalVars(const Node* tree) {
    /* Traverse the tree */
    if (tree->kind() == NODE_TOPLEVEL_LIST) {
        Node* candidate = tree->child(0);
        if (candidate->kind() == NODE_GLOBAL || candidate->kind() == NODE_CONST) {
            /* If debugging is enabled, register the name of the global in the global */
            /* reference lookup table */
            if (Environment::isDebugMode()) {
                Node* id = candidate->child(0);
                (*globalReferenceTable)[id->getStringvalue()] = id->getIntvalue().toInt();
            }

            /* perform assignment at this global scope */
            evaluateStatement(candidate, this);
        }

        if (tree->child(1) != NULL) {
            initializeGlobalVars(tree->child(1));
        }
    }
}

/* initializes an empty scope and sets that as the current scope */
void Context::initializeNewScope(const Node* callNode) {
    Scope* newScope = new Scope(callNode);
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
