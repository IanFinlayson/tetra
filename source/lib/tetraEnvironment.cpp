#include <pthread.h>
#include <iostream>

#include "tetra.h"

/*
 * This class stores the constants relevant to the Tetra Environment including:
 * number of threads
 * where to print
 */

void TetraEnvironment::initialize() {
  maxThreads = 8;
  console = NULL;
}

void TetraEnvironment::setMaxThreads(int pNum) {
    maxThreads = pNum;
}

int TetraEnvironment::getMaxThreads() {
    return maxThreads;
}

VirtualConsole* TetraEnvironment::getConsole() {
  return console;
}

void TetraEnvironment::setConsole(VirtualConsole* vc) {
    console = vc;
}

// halt the given program ASAP - used so the user can halt buggy programs
void TetraEnvironment::halt() {
    TetraEnvironment::running = false;
}

bool TetraEnvironment::isRunning() {
    return running;
}

void TetraEnvironment::setRunning() {
    running = true;
}

// Determines whether the interpreter should be executing in debug mode or not
void TetraEnvironment::setDebug(bool toggle) {
    debugMode = toggle;
}

bool TetraEnvironment::isDebugMode() {
    return debugMode;
}


int TetraEnvironment::maxThreads = 8;
bool TetraEnvironment::debugMode = false;
bool TetraEnvironment::running = true;
VirtualConsole* TetraEnvironment::console = NULL;


