/* eval.cpp
 * this file contains routines for evaluating the different tpyes of program
 * nodes
 */

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <QThread>

#include "tetra.h"

extern DataType* PAIR_TYPE;

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
        Data* sourceValue = evaluateExpression(node2, sourceContext);

        /* create a data reference for this name in the new scope */
        Data* destinationValue =
            destinationScope->lookupVar(node1->getStringvalue(), node1->type());

        /* do the assignment */
        destinationValue->opAssign(sourceValue);
    }
}

/* evaluate a function call node */
Data* evaluateFunctionCall(Node* node, Context* context) {
    /* check to see if this is a standard library function */
    String funcName = node->child(0)->getStringvalue();

    /* i/o functions */
    if (funcName == "print") {
        return tslPrint(node->child(1), context);
    } else if (funcName == "input") {
        return tslInput(node->child(1), context);
    }

    /* type conversion ones */
    else if (funcName == "int") {
        return tslInt(node->child(1), context);
    } else if (funcName == "real") {
        return tslReal(node->child(1), context);
    } else if (funcName == "string") {
        return tslString(node->child(1), context);
    } else if (funcName == "bool") {
        return tslBool(node->child(1), context);
    }

    /* len function */
    else if (funcName == "len") {
        return tslLen(node->child(1), context);
    }

    /* regular user defined functions */
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
        Data* returnValue = evaluateStatement(funcNode, context);

        /* returns to the old scope once the function has finished evaluating */
        context->exitScope();

        /* return the functions return value back */
        return returnValue;
    }
}

/* evaluate any regular binary operator expression node the operatorMethod
 * parameter refers to one of the Data operator methods e.g.  opOR, opAnd etc.
 * from that class */
Data* evaluateBinaryExpression(Node* node,
                               Context* context,
                               Data* (Data::*operatorMethod)(const Data*) ) {
    /* evaluate both of the children */
    Data* lhs = evaluateExpression(node->child(0), context);
    Data* rhs = evaluateExpression(node->child(1), context);

    /* call the operator method on the left, passing the right, and return the
     * result */
    return (lhs->*operatorMethod)(rhs);
}

/* fill a list from the children nodes of a list value node */
void fillList(List* list, Node* node, Context* context) {
    /* evaluate the first item */
    Data* first = evaluateExpression(node->child(0), context);

    /* add it to the list */
    list->add(first);

    /* recursively add the rest of the list if there is one */
    if (node->getNumChildren() == 2) {
        fillList(list, node->child(1), context);
    }
}

/* fill a dict from the children nodes of a dict value node */
void fillDict(Dict* dict, Node* node, Context* context) {
    /* evaluate the first item */
    Data* firstKey = evaluateExpression(node->child(0), context);
    Data* firstVal = evaluateExpression(node->child(1), context);

    /*copy */
    Data* keyCopy = Data::create(firstKey->getType(), firstKey->getValue());
    Data* valCopy = Data::create(firstVal->getType(), firstVal->getValue());

    /* add it to the list */
    Pair p;
    p.set(keyCopy, valCopy);
    /* wrap this pair in a tdata */
    Data* elem = Data::create(PAIR_TYPE, &p);
    dict->add(elem);

    /* recursively add the rest of the dict if there is one */
    if (node->getNumChildren() == 3) {
        fillDict(dict, node->child(2), context);
    }
}

/* return true if the passed node is on the left
 * hand side of an assignment */
bool isLValue(Node* node) {
    return (node->getParent()->kind() == NODE_ASSIGN) && (node->getParent()->child(0) == node);
}

/* evaluates operations on data types and returns the value */
Data* evaluateExpression(Node* node, Context* context) {
    /* do different things based on the type of statement this is */
    switch (node->kind()) {
        /* evaluate the function call using the function above */
        case NODE_FUNCALL:
            return evaluateFunctionCall(node, context);

        case NODE_STRINGVAL: {
            /* make a Data for the value */
            String value = node->getStringvalue();
            return Data::create(node->type(), &value);
        }

        case NODE_REALVAL: {
            /* make a Data for the value */
            Real value = node->getRealvalue();
            return Data::create(node->type(), &value);
        }

        case NODE_INTVAL: {
            /* make a Data for the value */
            Int value = node->getIntvalue();
            return Data::create(node->type(), &value);
        }

        case NODE_BOOLVAL: {
            /* make a Data for the value */
            Bool value = node->getBoolvalue();
            return Data::create(node->type(), &value);
        }

        case NODE_TUPVAL:
        case NODE_LISTVAL: {
            /* make a list data structure */
            List l;
            /* if there are data elements, get them */
            if (node->getNumChildren() > 0) {
                fillList(&l, node, context);
            }

            /* wrap this list in a tdata */
            Data* list = Data::create(node->type(), &l);
            return list;
        }

        case NODE_DICTVAL: {
            /* make a dict data structure */
            Dict d;
            /* if there are data elements, get them */
            if (node->getNumChildren() > 0) {
                fillDict(&d, node, context);
            }

            /* wrap this dict in a tdata */
            Data* dict = Data::create(node->type(), &d);
            return dict;
        }

        case NODE_LISTRANGE: {
            /* make the list data structure */
            List l;

            /* evaluate the start and end points */
            Data* start = evaluateExpression(node->child(0), context);
            Data* end = evaluateExpression(node->child(1), context);

            /* assemble the array */
            for (Int t = *((Int*) (start->getValue())); (t <= *((Int*) (end->getValue()))).toBool();
                 t++) {
                l.add(Data::create(node->child(0)->type(), &t));
            }

            /* wrap this list in a tdata */
            Data* list = Data::create(node->type(), &l);
            return list;
        }

        case NODE_ASSIGN: {
            /* evaluate both of the children */
            Data* lhs = evaluateExpression(node->child(0), context);
            Data* rhs = evaluateExpression(node->child(1), context);
            lhs->opAssign(rhs);
            return lhs;
        }
        case NODE_LT:
            return evaluateBinaryExpression(node, context, &Data::opLt);
        case NODE_LTE:
            return evaluateBinaryExpression(node, context, &Data::opLte);
        case NODE_GT:
            return evaluateBinaryExpression(node, context, &Data::opGt);
        case NODE_GTE:
            return evaluateBinaryExpression(node, context, &Data::opGte);
        case NODE_EQ:
            return evaluateBinaryExpression(node, context, &Data::opEq);
        case NODE_NEQ:
            return evaluateBinaryExpression(node, context, &Data::opNeq);
        case NODE_BITXOR:
            return evaluateBinaryExpression(node, context, &Data::opBitxor);
        case NODE_BITAND:
            return evaluateBinaryExpression(node, context, &Data::opBitand);
        case NODE_BITOR:
            return evaluateBinaryExpression(node, context, &Data::opBitor);
        case NODE_SHIFTL:
            return evaluateBinaryExpression(node, context, &Data::opShiftl);
        case NODE_SHIFTR:
            return evaluateBinaryExpression(node, context, &Data::opShiftr);
        case NODE_PLUS:
            return evaluateBinaryExpression(node, context, &Data::opPlus);
        case NODE_MINUS:
            return evaluateBinaryExpression(node, context, &Data::opMinus);
        case NODE_TIMES:
            return evaluateBinaryExpression(node, context, &Data::opTimes);
        case NODE_DIVIDE:
            return evaluateBinaryExpression(node, context, &Data::opDivide);
        case NODE_MODULUS:
            return evaluateBinaryExpression(node, context, &Data::opModulus);
        case NODE_EXP:
            return evaluateBinaryExpression(node, context, &Data::opExp);

        /* these are done differently to support short-circuit evaluation */
        case NODE_OR: {
            /* evaluate lhs */
            Data* lhs = evaluateExpression(node->child(0), context);

            /* if true, return a true value */
            if (((Bool*) lhs->getValue())->toBool()) {
                Bool falseValue(true);
                return Data::create(lhs->getType(), &falseValue);
            } else {
                /* do the rest of it */
                Data* rhs = evaluateExpression(node->child(1), context);
                return lhs->opOr(rhs);
            }
        }
        case NODE_AND: {
            /* evaluate lhs */
            Data* lhs = evaluateExpression(node->child(0), context);

            /* if false, return a false value */
            if (((Bool*) lhs->getValue())->toBool() == false) {
                Bool falseValue(false);
                return Data::create(lhs->getType(), &falseValue);
            } else {
                /* do the rest of it */
                Data* rhs = evaluateExpression(node->child(1), context);
                return lhs->opAnd(rhs);
            }
        }

        case NODE_UMINUS: {
            /* evaluate the child */
            Data* operand = evaluateExpression(node->child(0), context);

            /* return the negative of this */
            return operand->opNegate();
        }

        case NODE_BITNOT: {
            /* evaluate the child */
            Data* operand = evaluateExpression(node->child(0), context);

            /* return the not of this */
            return operand->opBitnot();
        }

        case NODE_NOT: {
            /* evaluate the child */
            Data* operand = evaluateExpression(node->child(0), context);

            /* return the not of this */
            return operand->opNot();
        }

        case NODE_INDEX: {
            /* evaluate the list on the left and the index on the right */
            Data* container = evaluateExpression(node->child(0), context);
            Data* index = evaluateExpression(node->child(1), context);

            return container->opIndex(index, isLValue(node));
        }

        /* simply get the identifier out of the context */
        case NODE_IDENTIFIER:
            return context->lookupVar(node->getStringvalue(), node->type());

        default:
            throw SystemError("Unhandled node type in eval", 0, node);
            break;
    }
}

/* evaluate a parallel statement */
Data* evaluateParallel(Node* node, Context* context) {
    Node* next = node->child(0);
    std::vector<ParallelWorker*> children_threads;

    while (next != NULL) {
        /* peel off the left child */
        Node* left = next->child(0);

        /* launch a thread for it */
        ParallelWorker* worker1 = new ParallelWorker(left, context);
        worker1->start();
        children_threads.push_back(worker1);

        /* get the other child */
        next = next->child(1);

        /* if it's not a stmts line, do it and stop */
        if (next->kind() != NODE_STATEMENT) {
            /* launch a thread for it */
            ParallelWorker* worker2 = new ParallelWorker(next, context);
            worker2->start();
            children_threads.push_back(worker2);
            next = NULL;
        }
    }

    /* wait for them to finish and deallocate them */
    for (unsigned int i = 0; i < children_threads.size(); i++) {
        children_threads[i]->wait();
        delete children_threads[i];
    }

    /* TODO what should happen if a parallel has a return in it??? */
    return NULL;
}

/* evaluate a statement node - only returns a value for return statements */
Data* evaluateStatement(Node* node, Context* context) {
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
            Data* value = evaluateStatement(node->child(0), context);

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
            Data* returnValue = NULL;

            /* check if there is a child to return or not */
            if (node->child(0)) {
                /* evaluate the expression */
                returnValue = evaluateExpression(node->child(0), context);
            }

            context->notifyReturn();
            return returnValue;
        } break;

        /* just do nothing */
        case NODE_PASS:
            return NULL;
            break;

        /* set these in the context so we know where yo go next */
        case NODE_BREAK:
            context->notifyBreak();
            break;
        case NODE_CONTINUE:
            context->notifyContinue();
            break;

        case NODE_GLOBAL:
        case NODE_CONST: {
            /* evaluate the  right hand side */
            Data* value = evaluateExpression(node->child(1), context);

            /* get a pointer to the global thing on the left */
            Data* global =
                context->lookupVar(node->child(0)->getStringvalue(), node->child(0)->type());

            /* do the assignment */
            global->opAssign(value);

        } break;

        /* handle simple if expressions */
        case NODE_IF: {
            /* evaluate the conditional expression */
            Data* conditional = evaluateExpression(node->child(0), context);
            /* if true execute the 2nd child */
            if (((Bool*) (conditional->getValue()))->toBool()) {
                return evaluateStatement(node->child(1), context);
            } else {
                /* check for else block and execute it if it exists */
                if (node->child(2) != NULL) {
                    return evaluateStatement(node->child(2), context);
                }
            }
        } break;

        /* handle elif nodes */
        case NODE_ELIF: {
            context->notifyElif();

            /* check the first branch */
            Data* returnValue = evaluateStatement(node->child(0), context);

            /* check if the first one was false */
            ExecutionStatus status = context->queryExecutionStatus();
            if (status == ELIF) {
                /* if so, do the next one */
                returnValue = evaluateStatement(node->child(1), context);
            }

            /* check to see if we need to execute the catchall else statement
             * if it exists */
            status = context->queryExecutionStatus();
            if (status == ELIF && node->child(2) != NULL) {
                /* if so, execute that */
                returnValue = evaluateStatement(node->child(2), context);
            }

            return returnValue;
        } break;

        case NODE_ELIF_CHAIN: {
            /* try to execute the given case of the ELIF statement */
            Data* returnValue = evaluateStatement(node->child(0), context);

            /* check to see if we are doing the next one */
            ExecutionStatus status = context->queryExecutionStatus();
            if (status == ELIF) {
                /* if so, do it */
                returnValue = evaluateStatement(node->child(1), context);
            }

            return returnValue;
        } break;

        case NODE_ELIF_CLAUSE: {
            /* check the condition on the left */
            Data* conditional = evaluateExpression(node->child(0), context);

            /* if condition is true, execute the body */
            if (((Bool*) conditional->getValue())->toBool()) {
                /* we no longer need to check the rest of the branches */
                context->normalizeStatus();
                return evaluateStatement(node->child(1), context);
            }
            return NULL;
        } break;

        case NODE_WHILE: {
            /* evaluate the condition */
            Data* conditional = evaluateExpression(node->child(0), context);

            /* while it is true */
            while (((Bool*) conditional->getValue())->toBool()) {
                /* evaluate the loop body */
                Data* returnValue = evaluateStatement(node->child(1), context);

                /* check the execution status */
                ExecutionStatus status = context->queryExecutionStatus();

                /* if we returned, pass the value up */
                if (status == RETURN) {
                    return returnValue;
                }

                /* if we broke, reset status and break out of the loop */
                if (status == BREAK) {
                    context->normalizeStatus();
                    break;
                }

                /* if we continued, just reset status and carry on */
                if (status == CONTINUE) {
                    context->normalizeStatus();
                }

                /* recheck the condition */
                conditional = evaluateExpression(node->child(0), context);
            }
        } break;

        case NODE_FOR: {

            DataTypeKind k = node->child(1)->type()->getKind();
            if (k == TYPE_DICT || k == TYPE_LIST) {
                /* evaluate the list we are looping through */
                Data* containerData = evaluateExpression(node->child(1), context);

                /* pull the list out of it */
                Container* container = (Container*) containerData->getValue();

                /* the return value if we hit one */
                Data* returnValue = NULL;

                /* for each item in this list */
                for (unsigned i = 0; i < container->length(); i++) {
                    /* if we are breaking or returning, stop */
                    ExecutionStatus status = context->queryExecutionStatus();
                    if (status == BREAK) {
                        context->normalizeStatus();
                        return NULL;
                    } else if (status == RETURN) {
                        context->normalizeStatus();
                        return returnValue;
                    }

                    /* set context to normal for now */
                    context->normalizeStatus();

                    /* look the induction variable up in the context */
                    Data* loopVariable =
                        context->lookupVar(node->child(0)->getStringvalue(), node->child(0)->type());

                    /* set it to the next value */
                    loopVariable->opAssign((*container)[i]);

                    /* evaluate the body of the loop */
                    returnValue = evaluateStatement(node->child(2), context);
            }

            } else if (k == TYPE_STRING) {
                /* evaluate the string we are looping through */
                Data* stringData = evaluateExpression(node->child(1), context);

                /* pull the string out of it */
                String* string = (String*) stringData->getValue();

                /* the return value if we hit one */
                Data* returnValue = NULL;

                /* for each item in this string */
                for (unsigned int i = 0; i < string->length(); i++) {
                    /* if we are breaking or returning, stop */
                    ExecutionStatus status = context->queryExecutionStatus();
                    if (status == BREAK) {
                        context->normalizeStatus();
                        return NULL;
                    } else if (status == RETURN) {
                        context->normalizeStatus();
                        return returnValue;
                    }

                    /* set context to normal for now */
                    context->normalizeStatus();

                    /* look the induction variable up in the context */
                    Data* loopVariable = context->lookupVar(node->child(0)->getStringvalue(),
                                                            node->child(0)->type());

                    /* set it to the next value */
                    String letter = string->substring(i, 1);
                    DataType d(TYPE_STRING);
                    Data* letterD = Data::create(&d, &letter);
                    loopVariable->opAssign(letterD);

                    /* evaluate the body of the loop */
                    returnValue = evaluateStatement(node->child(2), context);
                }
            }
        } break;


        case NODE_PARALLEL:
            return evaluateParallel(node, context);
            break;

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
    Environment::setDebug(debug);
    Environment::setMaxThreads(threads);
    Environment::setRunning();

    /* construct a context (this also initializes the global scope) */
    Context context;

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

    /* leave the scope */
    context.exitScope();

    return 0;
}
