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

/* calls a function, returns the return value */
void evaluateFunction(const Node* node) {
    UNUSED(node);
}

/* evaluates operations on data types */
void evaluateExpression(const Node* node) {
    UNUSED(node);
}

/* Equivilant of main for the interpreter module */
int interpret(Node* tree, int debug, int threads) {
    /* set environment settings */
    TetraEnvironment::setDebug(debug);
    TetraEnvironment::setMaxThreads(threads);
    TetraEnvironment::setRunning();

    /* attempt to find the main function */
    const Node* main = functions.getFunctionNode("main()");
    if (main == NULL) {
        throw Error("No main function found", 0);
    }

    /* evaluate the main function */
    evaluateFunction(main);

    UNUSED(tree);

    return 0;
}
