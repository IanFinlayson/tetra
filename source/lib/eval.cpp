/* eval.cpp
 * this file contains routines for evaluating the different types of program
 * nodes
 */

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <algorithm>
#include <sstream>
#include <string>

#include "tetra.h"

/* all of the background threads that have been launched */
std::vector<ParallelWorker*> backgroundThreads;

extern DataType* PAIR_TYPE;

/* this function populates a scope object with the variables contained in a
 * portion of the subtree containing the actual parameter expressions which are
 * passed in */
void pasteArgList(Node* node1,
                  Node* node2,
                  Scope* destinationScope,
                  Context* sourceContext) {
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

        /* create a data reference for this name in the new scope 
         * the thread id does not matter because this will NOT be a thread specfic
         * variable (i.e. not a parallel for variable */
        Data* destinationValue =
            destinationScope->lookupVar(node1->getStringvalue(), node1->type(), -1);

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

    /* sleep function */
    else if (funcName == "sleep") {
        return tslSleep(node->child(1), context);
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

    } else {
        /* it's just a regular user defined functions */

        Data* funcData;
        if (node->child(0)->type()->getKind() == TYPE_OVERLOAD) {
            /* we need to find the function value as an overload */
            funcData = context->findOverload(node);

        } else {
            /* else we can just look it up from the context iby name */
            funcData = evaluateExpression(node->child(0), context);
        }

        /* get the body node out of the function */
        Node* funcNode = ((Function*) funcData->getValue())->getNode();

        /* check if there are parameters to be passed, and do so if needed */
        if (node->child(1) != NULL) {
            /* make a scope for the new function we are calling */
            Scope* destScope = new Scope();

            /* dump the passed parameters into the new scope */
            pasteArgList(funcNode->child(0), node->child(1), destScope, context);

            /* set the new scope in our context */
            context->initializeNewScope(destScope);
        } else {
            /* if there are no args, we still need to initialize a new scope */
            context->initializeNewScope();
        }

        /* place this node on the call stack, so it can be printed in the stack
         * trace */
        context->getCurrentScope()->setCallNode(node);

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
                               Data* (Data::*operatorMethod)(const Data*)) {
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
    list->add(Data::create(first->getType(), first->getValue()));

    /* recursively add the rest of the list if there is one */
    if (node->getNumChildren() == 2) {
        fillList(list, node->child(1), context);
    }
}

/* fill a dict from the children nodes of a dict value node */
void fillDict(Dict* dict, Node* node, Context* context) {
    /* evaluate the first pair */
    Data* firstKey = evaluateExpression(node->child(0), context);
    Data* firstVal = evaluateExpression(node->child(1), context);

    /*copy */
    Data* keyCopy = Data::create(firstKey->getType(), firstKey->getValue());
    Data* valCopy = Data::create(firstVal->getType(), firstVal->getValue());

    /* add both to a pair */
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

Data* evaluateFor(Node* node, Context* context) {
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
            Data* loopVariable = context->lookupVar(node->child(0)->getStringvalue(),
                                                    node->child(0)->type());

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

    return NULL;
}

/* evaluate a parallel statement */
Data* evaluateParallel(Node* node, Context* context) {
    /* mark the context as being parallel */
    context->notifyParallel();

    /* keep track of all the sub-statements */
    Node* next = node->child(0);
    std::vector<ParallelWorker*> children_threads;

    /* marks this context as parallel */
    context->incrementBackgroundThreads();

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

    /* end parallel mode */
    context->normalizeStatus();
    context->decrementBackgroundThreads();

    /* TODO what should happen if a parallel has a return in it??? */
    return NULL;
}

Data* evaluateLock(Node* node, Context* context) {
    /* find the mutex object here */
    Data* mutex =
        context->lookupVar(node->child(0)->getStringvalue(), node->child(0)->type());

    /* lock the mutex */
    ((Mutex*) mutex->getValue())->lock();

    /* evaluate the body */
    Data* returnValue = evaluateStatement(node->child(1), context);

    /* unlock the mutex */
    ((Mutex*) mutex->getValue())->unlock();

    return returnValue;
}

Data* evaluateBackground(Node* node, Context* context) {
    /* mark the context as being parallel */
    context->notifyParallel();

    /* add in one thread to the scopes */
    context->incrementBackgroundThreads();

    /* if there is a name, then the block is in child 1 instead */
    int blockIdx = node->getNumChildren() == 2 ? 1 : 0;

    /* make a thread for running this node, and add to the list */
    ParallelWorker* worker = new ParallelWorker(node->child(blockIdx), context);
    worker->start();
    backgroundThreads.push_back(worker);

    /* if it had a name, then set it up as a local variable */
    if (node->getNumChildren() == 2) {
        /* find the task object here */
        Data* task =
            context->lookupVar(node->child(0)->getStringvalue(), node->child(0)->type());

        /* assign the worker object into the task */
        ((Task*) task->getValue())->setWorker(worker);
    }

    return NULL;
}

Data* evaluateWait(Node* node, Context* context) {
    /* find the task object here */
    Data* task =
        context->lookupVar(node->child(0)->getStringvalue(), node->child(0)->type());

    /* wait for it to finish, then set it to NULL */
    ((Task*) task->getValue())->wait();
    ((Task*) task->getValue())->setWorker(NULL);

    return NULL;
}

/* evaluate a parallel for loop */
Data* evaluateParFor(Node* node, Context* context) {
    /* keep track of all of thw worker threads */
    std::vector<ParallelWorker*> workers;

    /* mark the context as being parallel */
    context->notifyParallel();

    /* add in one thread to the scopes */
    context->incrementBackgroundThreads();

    /* find the type of thingy we are doing */
    DataTypeKind k = node->child(1)->type()->getKind();
    if (k == TYPE_DICT || k == TYPE_LIST) {
        /* we must mark the induction variable as being from a par for loop */
        context->getCurrentScope()->setupParallelFor(node->child(0)->getStringvalue());

        /* evaluate the list we are looping through */
        Data* containerData = evaluateExpression(node->child(1), context);

        /* pull the actual list out of it */
        Container* container = (Container*) containerData->getValue();

        /* find the number of threads to spawn */
        unsigned int numThreads = std::min(container->length(), Environment::getMaxThreads());

        /* for each thread in the workgroup */
        for (unsigned int i = 0; i < numThreads; i++) {
            /* spawn one thread with these bounds */
            ParallelWorker* worker = new ParallelWorker(node->child(2), context);
            workers.push_back(worker);
        }

        /* for each data value */
        for (unsigned int i = 0; i < container->length(); i++) {
            /* find an idle worker */
            ParallelWorker* idle = NULL;
            while (true) {
                for (unsigned int i = 0; i < workers.size(); i++) {
                    if (!workers[i]->isRunning()) {
                        idle = workers[i];
                        break;
                    }
                }

                /* if there was an idle person we can exit the loop */
                if (idle) {
                    break;
                }

                /* sleep the main thread a tiny amount to give others some time */
                QThread::currentThread()->msleep(10);
            }

            /* now we must assign this thread a value in the scope */
            Data* data = (*container)[i];
            context->getCurrentScope()->assignParallelFor(node->child(0)->getStringvalue(),
                                                          idle->getThreadId(), data);

            /* start the worker back up */
            idle->start();
        }

    } else if (k == TYPE_STRING) {
        throw Error("TODO, handle par fors on strings!!");
    }

    /* wait for all of the threads to finish before moving on */
    for (unsigned int i = 0; i < workers.size(); i++) {
        workers[i]->wait();
        delete workers[i];
    }

    context->normalizeStatus();
    context->decrementBackgroundThreads();

    /* we must reset the parallel for loop variables */
    context->getCurrentScope()->clearParallelFor(node->child(0)->getStringvalue());
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
            Data* global = context->lookupVar(node->child(0)->getStringvalue(),
                                              node->child(0)->type());

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

        case NODE_FOR:
            return evaluateFor(node, context);
            break;

        /* handle the parallel constructs */
        case NODE_PARALLEL:
            return evaluateParallel(node, context);

        case NODE_BACKGROUND:
            return evaluateBackground(node, context);

        case NODE_LOCK:
            return evaluateLock(node, context);

        case NODE_WAIT:
            return evaluateWait(node, context);

        case NODE_PARFOR:
            return evaluateParFor(node, context);

        default:
            /* if it's none of these things, it must be an expression used as a
             * statement */
            return evaluateExpression(node, context);
    }

    /* if we got down here, it was not a return or an expression so just NULL */
    return NULL;
}

/* equivilant of main for the interpreter module */
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
    context.initializeNewScope();

    /* set the thread id of the main thread (will be 0 since first call) */
    unsigned int mainThreadId = Environment::getNextThreadId();
    context.setThreadId(mainThreadId);

    /* evaluate the main function */
    evaluateStatement(main, &context);

    /* wait for any background threads to finish */
    for (unsigned int i = 0; i < backgroundThreads.size(); i++) {
        backgroundThreads[i]->wait();
        delete backgroundThreads[i];
    }

    /* leave the scope */
    context.exitScope();

    return 0;
}
