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

/* get input from the user and return it */
Data* tslInput(Node* args, Context* context) {
    /* if there is an argument, print it first */
    if (args != NULL) {
        tslPrint(args, context);
    }

    /* read the string from the user */
    String ret = Environment::getConsole()->receiveStandardInput();

    /* wrap it up and return it */
    DataType t(TYPE_STRING);
    return Data::create(&t, &ret);
}


/* TODO these too
int readInt(int threadNum) {
    int ret = 0;
    // while(!(cin >> ret)) {
    while (!(tstringstream(Environment::getConsole(threadNum, false)
                    .receiveStandardInput()) >>
                ret)) {
        // Moves down the stream until it finds a readable number
        cin.clear();
        cin.ignore();
    }
    return ret;
}

double readReal(int threadNum) {
    double ret = 0;
    // while(!(cin >> ret)) {
    while (!(tstringstream(Environment::getConsole(threadNum, false)
                    .receiveStandardInput()) >>
                ret)) {
        // moves along the buffer until it finds a readable value
        cin.clear();
        cin.ignore();
    }
    return ret;
}

bool readBool(int threadNum) {
    // Returns false on some variation of 'false', 'no', or 0
    tstring input =
        Environment::getConsole(threadNum, false).receiveStandardInput();
    // Compare input against the recognized values for false
    if (input == "false" || input == "no" || input == "0") {
        return false;
    } else
        return true;
}

tstring readString(int threadNum) {
    tstring ret =
        Environment::getConsole(threadNum, false).receiveStandardInput();

    return ret;
}
*/
