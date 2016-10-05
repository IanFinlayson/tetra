/* eval.h
 * declares functions for interpreting and evaluating programs */

#ifndef EVAL_H
#define EVAL_H

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "context.h"

/* evaluate a statement node */
Data* evaluateStatement(Node* node, Context* context);

/* evaluate an expression node */
Data* evaluateExpression(Node* node, Context* context);

/* interpret a tetra program parsed into a Node tree */
int interpret(Node* tree, int debug, int threads);

/* allows other libraries to define how the interpreter should handle I/O */
class VirtualConsole {
   public:
    /* read a string from the user and return it */
    virtual String receiveStandardInput() = 0;
    /* print a string for the user */
    virtual void processStandardOutput(const String&) = 0;
};

#endif
