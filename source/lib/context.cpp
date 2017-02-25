/* context.cpp
 * this file implements the Context class which keeps track of variables in the
 * scopes of the running program */

#include "tetra.h"

Context::Context() {
    /* initialize the global scope */
    globalScope = new Scope();
    programStack.push(globalScope);
}

/* find an overloaded function by name and arguments */
Data* Context::findOverload(Node* functionCall) {
    /* build the signature we are looking for from the supplied parameters */
    String signature = functionCall->child(0)->getStringvalue() + "(";

    /* loop over the args if any */
    Node* args = (functionCall->getNumChildren() > 1) ? functionCall->child(1) : NULL;
    bool first = true;
    while (1) {
        if (args == NULL) {
            break;
        }

        /* only put comma in if this isnt' the first time */
        if (!first) {
            signature += ",";
        }
        first = false;

        /* add this one in */
        signature += typeToString(args->child(0)->type());

        /* move to the next one */
        args = (args->getNumChildren() > 1) ? args->child(1) : NULL;
    }
    signature += ")";

    /* now find it in the tree and pack up an object for it */
    Node* funcNode = functions.getFunctionNode(signature);
    Function funcVal(funcNode);
    DataType t(TYPE_FUNCTION);
    return Data::create(&t, &funcVal);
}

/* lookup a variable  in the present context */
Data* Context::lookupVar(String name, DataType* type, unsigned int threadid) {
    Data* value;

    if (getGlobalScopeRef()->containsVar(name)) {
        value = (getGlobalScopeRef()->lookupVar(name, type, threadid));
    } else if (type->getKind() == TYPE_FUNCTION) {
        /* find the function */
        String signature = FunctionMap::getFunctionSignature(name, type);
        Node* funcNode = functions.getFunctionNode(signature);
        Function funcVal(funcNode);
        value = Data::create(type, &funcVal);
    } else {
        /* look up general variables */
        value = programStack.top()->lookupVar(name, type, threadid);
    }

    return value;
}

/* set up the global variables from the parse tree */
void Context::initializeGlobalVars(const Node* tree) {
    /* Traverse the tree */
    if (tree->kind() == NODE_TOPLEVEL_LIST) {
        Node* candidate = tree->child(0);
        if (candidate->kind() == NODE_GLOBAL || candidate->kind() == NODE_CONST) {
            /* perform assignment at this global scope */
            evaluateStatement(candidate, this, 0);
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

    /* if the scope is empty, delete it */
    if (scope->getNumThreads() == 0) {
        delete scope;
    }

    programStack.pop();
}

/* If, for some reason the tetra program crashes inadvertantly, we may as well */
/* clean up the Context stack */
Context::~Context() {}
