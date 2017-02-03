/* tsp.cpp
 * this file defines Tetra Standard Library functions */

#include <iostream>

#include "tetra.h"

/* prints the expression(s) denoted by arguments given */
Data* tslPrint(Node* args, Context* context) {
    /* if no args given, just print a new line */
    if (args == NULL) {
        VirtualConsole* console = Environment::getConsole();
        console->processStandardOutput("\n");
        return NULL;
    }

    /* check if args is a structual node */
    if (args->kind() == NODE_ACTUAL_PARAM_LIST) {
        /* print the first thing */
        tslPrint(args->child(0), context);
        /* print the rest if there are any */
        if (args->getNumChildren() == 2) {
            tslPrint(args->child(1), context);
        }
    }

    /* if args is an expression, actually print it */
    else {
        VirtualConsole* console = Environment::getConsole();

        /* evaluate the node and get the value of it */
        Data* arg = evaluateExpression(args, context);

        /* convert to a string and print it to the connected console */
        console->processStandardOutput(arg->getValue()->toString());
    }

    /* print returns nothing */
    return NULL;
}

/* sleeps for some real number of seconds */
Data* tslSleep(Node* args, Context* context) {
    /* evaluate the arguments */
    Data* argument = evaluateExpression(args->child(0), context);

    /* get it as a real */
    Real* rarg = (Real*) argument->getValue();

    /* sleep on the current thread */
    QThread::msleep(rarg->toDouble() * 1000.0);
    return NULL;
}

/* get input from the user and return it */
Data* tslInput(Node* args, Context* context) {
    /* if there is an argument, print it first */
    if (args != NULL) {
        tslPrint(args, context);
    }

    /* read the string from the user */
    String ret = Environment::getConsole()->receiveStandardInput();

    /* if we are echoing input, do it, surrounded by tildes the whole reason
     * this is supported is so that we can run the interpreter with input
     * re-directed from a file, and still have the program's execution look
     * like a user typed it.  This is used in producing the output listings
     * for the Tetra book automatically (the ~ get replaced with LaTeX
     * formatting.  It's unlikely this feature will be useful in any other
     * context! */
    if (Environment::isEcho()) {
        VirtualConsole* console = Environment::getConsole();
        console->processStandardOutput("~");
        console->processStandardOutput(ret);
        console->processStandardOutput("~\n");
    }

    /* wrap it up and return it */
    DataType t(TYPE_STRING);
    return Data::create(&t, &ret);
}

/* the type conversions */
Data* tslInt(Node* args, Context* context) {
    /* evaluate the argument */
    Data* argument = evaluateExpression(args->child(0), context);
    DataType t(TYPE_INT);

    /* the three type possibilities */
    switch (argument->getType()->getKind()) {
        case TYPE_STRING: {
            String* sarg = (String*) argument->getValue();
            Int result = sarg->toInt();
            return Data::create(&t, &result);
        }
        case TYPE_REAL: {
            Real* rarg = (Real*) argument->getValue();
            Int result = rarg->toDouble();
            return Data::create(&t, &result);
        }
        case TYPE_BOOL: {
            Bool* barg = (Bool*) argument->getValue();
            Int result = barg->toBool() ? 1 : 0;
            return Data::create(&t, &result);
        }
        default:
            throw RuntimeError("Invalid type conversion", args->getLine());
    }
}

Data* tslReal(Node* args, Context* context) {
    /* evaluate the argument */
    Data* argument = evaluateExpression(args->child(0), context);
    DataType t(TYPE_REAL);

    /* the type possibilities */
    switch (argument->getType()->getKind()) {
        case TYPE_STRING: {
            String* sarg = (String*) argument->getValue();
            Real result = sarg->toReal();
            return Data::create(&t, &result);
        }
        case TYPE_INT: {
            Int* iarg = (Int*) argument->getValue();
            Real result = iarg->toInt();
            return Data::create(&t, &result);
        }
        default:
            throw RuntimeError("Invalid type conversion", args->getLine());
    }
}

Data* tslString(Node* args, Context* context) {
    /* evaluate the argument */
    Data* argument = evaluateExpression(args->child(0), context);
    DataType t(TYPE_STRING);

    /* the three type possibilities */
    switch (argument->getType()->getKind()) {
        case TYPE_INT: {
            Int* iarg = (Int*) argument->getValue();
            String result = iarg->toString();
            return Data::create(&t, &result);
        }
        case TYPE_REAL: {
            Real* rarg = (Real*) argument->getValue();
            String result = rarg->toString();
            return Data::create(&t, &result);
        }
        case TYPE_BOOL: {
            Bool* barg = (Bool*) argument->getValue();
            String result = barg->toString();
            return Data::create(&t, &result);
        }
        default:
            throw RuntimeError("Invalid type conversion", args->getLine());
    }
}

Data* tslBool(Node* args, Context* context) {
    /* evaluate the argument */
    Data* argument = evaluateExpression(args->child(0), context);
    DataType t(TYPE_BOOL);

    /* the type possibilities */
    switch (argument->getType()->getKind()) {
        case TYPE_STRING: {
            String* sarg = (String*) argument->getValue();
            Bool result = sarg->toBool();
            return Data::create(&t, &result);
        }
        case TYPE_INT: {
            Int* iarg = (Int*) argument->getValue();
            Bool result = (iarg->toInt() == 0) ? false : true;
            return Data::create(&t, &result);
        }
        default:
            throw RuntimeError("Invalid type conversion", args->getLine());
    }
}

Data* tslLen(Node* args, Context* context) {
    /* evaluate the argument */
    Data* argument = evaluateExpression(args->child(0), context);
    DataType t(TYPE_INT);
    Int length;

    /* the type possibilities */
    switch (argument->getType()->getKind()) {
        case TYPE_STRING:
            length = ((String*) argument->getValue())->length();
            return Data::create(&t, &length);
        case TYPE_DICT:
            length = ((Dict*) argument->getValue())->length();
            return Data::create(&t, &length);
        case TYPE_LIST:
            length = ((List*) argument->getValue())->length();
            return Data::create(&t, &length);
        case TYPE_TUPLE:
            length = ((Tuple*) argument->getValue())->length();
            return Data::create(&t, &length);
        default:
            throw RuntimeError("Invalid argument to len function", args->getLine());
    }
}
