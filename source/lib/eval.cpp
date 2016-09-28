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
        Tdata* destinationValue =
            destinationScope->lookupVar(node1->getStringvalue(), node1->type());

        /* do the assignment */
        destinationValue->opAssign(sourceValue);
    }
}

/* evaluate a function call node */
Tdata* evaluateFunctionCall(Node* node, Context* context) {
    /* check to see if this is a standard library function */
    Tstring funcName = node->child(0)->getStringvalue();

    if (funcName == "print") {
        if (node->child(1) != NULL) {
            return tslPrint(node->child(1), context);
        } else {
            /* print with no arguments */
            return tslPrint(NULL, context);
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

        /* transfer control to the function capturing the return value */
        Tdata* returnValue = evaluateStatement(funcNode, context);

        /* returns to the old scope once the function has finished evaluating */
        context->exitScope();

        /* return the functions return value back */
        return returnValue;
    }
}

/* evaluate any regular binary operator expression node the operatorMethod
 * parameter refers to one of the Tdata operator methods e.g.  opOR, opAnd etc.
 * from that class */
Tdata* evaluateBinaryExpression(Node* node,
                                Context* context,
                                Tdata* (Tdata::*operatorMethod)(const Tdata*) ) {
    /* evaluate both of the children */
    Tdata* lhs = evaluateExpression(node->child(0), context);
    Tdata* rhs = evaluateExpression(node->child(1), context);

    /* call the operator method on the left, passing the right, and return the
     * result */
    return (lhs->*operatorMethod)(rhs);
}

/* evaluates operations on data types and returns the value */
Tdata* evaluateExpression(Node* node, Context* context) {
    /* do different things based on the type of statement this is */
    switch (node->kind()) {
        /* evaluate the function call using the function above */
        case NODE_FUNCALL:
            return evaluateFunctionCall(node, context);

        case NODE_STRINGVAL: {
            /* make a Tdata for the value */
            Tstring value = node->getStringvalue();
            return Tdata::create(node->type(), &value);
        }

        case NODE_REALVAL: {
            /* make a Tdata for the value */
            Treal value = node->getRealvalue();
            return Tdata::create(node->type(), &value);
        }

        case NODE_INTVAL: {
            /* make a Tdata for the value */
            Tint value = node->getIntvalue();
            return Tdata::create(node->type(), &value);
        }

        case NODE_BOOLVAL: {
            /* make a Tdata for the value */
            Tbool value = node->getBoolvalue();
            return Tdata::create(node->type(), &value);
        }

        /* for all of these, we can simply call the binary expression function with
         * the appropriate method parameter */
        case NODE_ASSIGN:
            return evaluateBinaryExpression(node, context, &Tdata::opAssign);

        case NODE_OR:
            return evaluateBinaryExpression(node, context, &Tdata::opOr);
        case NODE_AND:
            return evaluateBinaryExpression(node, context, &Tdata::opAnd);

        case NODE_LT:
            return evaluateBinaryExpression(node, context, &Tdata::opLt);
        case NODE_LTE:
            return evaluateBinaryExpression(node, context, &Tdata::opLte);
        case NODE_GT:
            return evaluateBinaryExpression(node, context, &Tdata::opGt);
        case NODE_GTE:
            return evaluateBinaryExpression(node, context, &Tdata::opGte);
        case NODE_EQ:
            return evaluateBinaryExpression(node, context, &Tdata::opEq);
        case NODE_NEQ:
            return evaluateBinaryExpression(node, context, &Tdata::opNeq);

        case NODE_BITXOR:
            return evaluateBinaryExpression(node, context, &Tdata::opBitxor);
        case NODE_BITAND:
            return evaluateBinaryExpression(node, context, &Tdata::opBitand);
        case NODE_BITOR:
            return evaluateBinaryExpression(node, context, &Tdata::opBitor);
        case NODE_SHIFTL:
            return evaluateBinaryExpression(node, context, &Tdata::opShiftl);
        case NODE_SHIFTR:
            return evaluateBinaryExpression(node, context, &Tdata::opShiftr);

        case NODE_PLUS:
            return evaluateBinaryExpression(node, context, &Tdata::opPlus);
        case NODE_MINUS:
            return evaluateBinaryExpression(node, context, &Tdata::opMinus);
        case NODE_TIMES:
            return evaluateBinaryExpression(node, context, &Tdata::opTimes);
        case NODE_DIVIDE:
            return evaluateBinaryExpression(node, context, &Tdata::opDivide);
        case NODE_MODULUS:
            return evaluateBinaryExpression(node, context, &Tdata::opModulus);
        case NODE_EXP:
            return evaluateBinaryExpression(node, context, &Tdata::opExp);

        case NODE_BITNOT: {
            /* evaluate the child */
            Tdata* operand = evaluateExpression(node->child(0), context);

            /* return the not of this */
            return operand->opBitnot();
        }

        case NODE_NOT: {
            /* evaluate the child */
            Tdata* operand = evaluateExpression(node->child(0), context);

            /* return the not of this */
            return operand->opNot();
        }

        // case NODE_DOT:
        // return evaluateBinaryExpression(node, context, &Tdata::opDot);

        /* simply get the identifier out of the context */
        case NODE_IDENTIFIER:
            return context->lookupVar(node->getStringvalue(), node->type());

        default:
            throw SystemError("Unhandled node type in eval", 0, node);
            break;
    }
}

/* evaluate a statement node - only returns a value for return statements */
Tdata* evaluateStatement(Node* node, Context* context) {
    /* do different things based on the type of statement this is */
    switch (node->kind()) {
        case NODE_FUNCTION: {
            /* if there were no arguments, child 0 is the body, else child 1 */
            if (node->getNumChildren() == 1) {
                return evaluateStatement(node->child(0), context);
            } else {
                return evaluateStatement(node->child(1), context);
            }
        } break;

        case NODE_STATEMENT: {
            /* evaluate the first child */
            Tdata* value = evaluateStatement(node->child(0), context);

            /* if it didn't result in a break of some kind, do the second one */
            ExecutionStatus status = context->queryExecutionStatus();
            if (status != RETURN && status != BREAK && status != CONTINUE) {
                return evaluateStatement(node->child(1), context);

                /* if it is a return, we return that value back */
            } else if (status == RETURN) {
                return value;
            }
        } break;

        case NODE_RETURN: {
            /* the return value, if any */
            Tdata* returnValue = NULL;

            /* check if there is a child to return or not */
            if (node->child(0)) {
                /* evaluate the expression */
                returnValue = evaluateExpression(node->child(0), context);
            }

            context->notifyReturn();
            return returnValue;
        }

        case NODE_IF: {
            /* evaluate the conditional expression */
            Tdata* conditional = evaluateExpression(node->child(0), context);
            /* if true execute the 2nd child */
            if (dynamic_cast<Tbool*>(conditional->getValue())->toBool()) {
                return evaluateStatement(node->child(1), context);
            } else {
                /* check for else block and execute it if it exists */
                if (node->child(2) != NULL) {
                    return evaluateStatement(node->child(2), context);
                }
            }
        } break;

        default:
            /* if it's none of these things, it must be an expression used as a
             * statement */
            return evaluateExpression(node, context);
    }

    /* if we got down here, it was not a return or an expression so just NULL */
    return NULL;
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
