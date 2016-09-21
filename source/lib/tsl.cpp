/* this file defines Tetra Standard Library functions */

#include <string.h>
#include <iostream>

#include "tetra.h"

/* prints the expression(s) denoted by arguments given */
Tdata* tslPrint(Node* args, Context* context) {
    /* if no args given, just print a new line */
    if (args == NULL) {
        VirtualConsole* console = TetraEnvironment::getConsole();
        console->processStandardOutput("\n");
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
        VirtualConsole* console = TetraEnvironment::getConsole();

        /* evaluate the node and get the value of it */
        Tdata* arg = evaluateExpression(args, context);

        /* convert to a string and print it to the connected console */
        console->processStandardOutput(arg->getValue()->toString());
    }

    /* print returns nothing */
    return NULL;
}

/* TODO these too
int readInt(int threadNum) {
    int ret = 0;
    // while(!(cin >> ret)) {
    while (!(tstringstream(TetraEnvironment::getConsole(threadNum, false)
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
    while (!(tstringstream(TetraEnvironment::getConsole(threadNum, false)
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
        TetraEnvironment::getConsole(threadNum, false).receiveStandardInput();
    // Compare input against the recognized values for false
    if (input == "false" || input == "no" || input == "0") {
        return false;
    } else
        return true;
}

tstring readString(int threadNum) {
    tstring ret =
        TetraEnvironment::getConsole(threadNum, false).receiveStandardInput();

    return ret;
}
*/
