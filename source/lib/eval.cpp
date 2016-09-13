/*
 * This program interprets a tetra file by getting its tree representation
 * Takes a single filename containing the Tetra code as a parameter
 */

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "tetra.h"

/* evaluates operations on data types and returns the value */
Tdata* evaluateExpression(Node* node, Context* context) {
    UNUSED(node);
    UNUSED(context);
    return NULL;
}

/* evaluate a statement node */
void evaluateStatement(Node* node, Context* context) {
    UNUSED(context);
    UNUSED(node);

    /* do different things based on the type of statement this is */
    switch (node->kind()) {
        case NODE_FUNCTION: {
            /* if there were no arguments, child 0 is the body, else child 1 */
            if (node->getNumChildren() == 1) {
                evaluateStatement(node->child(0), context);
            } else {
                evaluateStatement(node->child(1), context);
            }
        } break;

        default:
            throw SystemError("Unhandled node type in eval", 0, node);
            break;
    }
}

/* Equivilant of main for the interpreter module */
int interpret(Node* tree, int debug, int threads) {
    /* set environment settings */
    TetraEnvironment::setDebug(debug);
    TetraEnvironment::setMaxThreads(threads);
    TetraEnvironment::setRunning();

    /* construct a context (this also initializes the global scope) */
    Context context(TetraEnvironment::obtainNewThreadID());

    /* attempt to find the main function */
    Node* main = functions.getFunctionNode("main()");
    if (main == NULL) {
        throw Error("No main function found", 0);
    }

    /* initialize global vars */
    context.initializeGlobalVars(tree);

    /* initialize a scope fpr the main method, and run */
    context.initializeNewScope(main);

    /* evaluate the main function */
    evaluateStatement(main, &context);

    /* wait for any unfinished business */
    ThreadEnvironment::joinDetachedThreads();

    /* leave the scope */
    context.exitScope();

    return 0;
}
