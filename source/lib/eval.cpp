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

/* this function populates a scope object with the variables contained in a
 * portion of the subtree containing the actual parameter expressions which are
 * passed in */
void pasteArgList(Node* node1, Node* node2, Scope* destinationScope, Context* sourceContext) {
    /* check if we are a NODE_FORMAL_PARAM_LIST (structure), or an actual value */
    if (node1->kind() == NODE_FORMAL_PARAM_LIST) {
        /* recursively paste in the subtrees */
        pasteArgList(node1->child(0), node2->child(0), destinationScope, sourceContext);
        if (node1->child(1)) {
            pasteArgList(node1->child(1), node2->child(1), destinationScope, sourceContext);
        }

    } else {
        /* we are an actual value - paste the value from the source to the
         * destination */

        /* evaluate node2 to get a value */
        Tdata* sourceValue = evaluateExpression(node2, sourceContext);

        /* create a data reference for this name in the new scope */
        Tdata* destinationValue = destinationScope->lookupVar(node1->getStringvalue());

        /* do the assignment */
        destinationValue->opAssign(sourceValue);
    }
}


Tdata* evaluateFunctionCall(Node* node, Context* context) {
    /* check to see if this is a standard library function */
    Tstring funcName = node->child(0)->getStringvalue();
    if (funcName == "print") {
        if (node->child(1) != NULL) {
            tslPrint(node->child(1), context);
        }
    }

    /* TODO add the other standard lib functions */

    else {
        /* it's user defined, find it in the tree */
        Node* funcNode = functions.getFunctionNode(node);

        /* check if there are parameters to be passed, and do so if needed */
        if (node->child(1) != NULL) {
            /* make a scope for the new function we are calling */
            Scope destScope(node);

            /* dump the passed parameters into the new scope */
            pasteArgList(funcNode->child(0), node->child(1), &destScope, context);

            /* set the new scope in our context */
            context->initializeNewScope(destScope);
        } else {
            /* if there are no args, we still need to initialize a new scope */
            context->initializeNewScope(node);
        }

        /* place this node on the call stack, so it can be printed in the stack
         * trace */
        context->getCurrentScope().setCallNode(node);

        /* transfer control to the function */
        evaluateStatement(funcNode, context);

        /* returns to the old scope once the function has finished evaluating */
        context->exitScope();
    }

    /* FIXME how to return the value back out? */
    return NULL;
}




/* evaluates operations on data types and returns the value */
Tdata* evaluateExpression(Node* node, Context* context) {
    /* do different things based on the type of statement this is */
    switch (node->kind()) {
        case NODE_FUNCALL:
            return evaluateFunctionCall(node, context);

        default:
            throw SystemError("Unhandled node type in eval", 0, node);
            break;
    }
}

/* evaluate a statement node */
void evaluateStatement(Node* node, Context* context) {
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

        case NODE_STATEMENT: {
            /* evaluate the first child */
            evaluateStatement(node->child(0), context);

            /* if it didn't result in a break of some kind, do the second one */
            ExecutionStatus status = context->queryExecutionStatus();
            if (status != RETURN && status != BREAK && status != CONTINUE) {
                evaluateStatement(node->child(1), context);
            }
        } break;

        case NODE_IF: {
            /* evaluate the conditional expression */
            Tdata* conditional = evaluateExpression(node->child(0), context);
            /* if true execute the 2nd child */
            if (dynamic_cast<Tbool*>(conditional->getValue())->toBool()) {
                evaluateStatement(node->child(1), context);
            } else {
                /* check for else block and execute it if it exists */
                if (node->child(2) != NULL) {
                    evaluateStatement(node->child(2), context);
                }
            }
        } break;








        default:
            /* if it's none of these things, it must be an expression used as a
             * statement */
            evaluateExpression(node, context);
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
