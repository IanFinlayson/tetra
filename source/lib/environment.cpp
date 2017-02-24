/* environment.cpp
 * this file implements the Environment class which keeps track of
 * values defining the way the program executes */

#include <iostream>

#include "tetra.h"

/*
 * This class stores the constants relevant to the Tetra Environment including:
 * number of threads
 * where to print
 */

void Environment::initialize() {
    maxThreads = QThread::idealThreadCount();
    console = NULL;
}

void Environment::setMaxThreads(int pNum) {
    maxThreads = pNum;
}

unsigned int Environment::getMaxThreads() {
    return maxThreads;
}

VirtualConsole* Environment::getConsole() {
    return console;
}

void Environment::setConsole(VirtualConsole* vc) {
    console = vc;
}

/* halt the given program ASAP - used so the user can halt buggy programs */
void Environment::halt() {
    Environment::running = false;
}

bool Environment::isRunning() {
    return running;
}

void Environment::setRunning() {
    running = true;
}

/* determines whether the interpreter should be executing in debug mode or not
*/
void Environment::setDebug(bool toggle) {
    debugMode = toggle;
}

bool Environment::isDebugMode() {
    return debugMode;
}

void Environment::setEcho() {
    inputEcho = true;
}

bool Environment::isEcho() {
    return inputEcho;
}

/* static variable initializations for the environment */
unsigned int Environment::maxThreads = QThread::idealThreadCount();
bool Environment::inputEcho = false;
bool Environment::debugMode = false;
bool Environment::running = true;
VirtualConsole* Environment::console = NULL;
