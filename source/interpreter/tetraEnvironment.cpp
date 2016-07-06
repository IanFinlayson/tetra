#include <pthread.h>
#include <iostream>
#include "backend.h"

/*
 * This class stores the constants relevant to the Tetra Environment including:
 * number of threads
 * where to print
 */

void TetraEnvironment::initialize() {
  maxThreads = 8;
  consoleArray_ptr = NULL;
  outputStream = &std::cout;
}

void TetraEnvironment::initialize(ConsoleArray& console) {
  maxThreads = 8;
  consoleArray_ptr = &console;
  outputStream = &std::cout;
}
void TetraEnvironment::setMaxThreads(int pNum) { maxThreads = pNum; }

void TetraEnvironment::setOutputStream(ostream& pOut) { outputStream = &pOut; }

ostream& TetraEnvironment::getOutputStream() { return *outputStream; }

int TetraEnvironment::getMaxThreads() { return maxThreads; }

void TetraEnvironment::setConsoleArray(ConsoleArray& pConsole) {
  consoleArray_ptr = &pConsole;
}

ConsoleArray& TetraEnvironment::getConsoleArray() {
  return *consoleArray_ptr;
}

VirtualConsole& TetraEnvironment::getConsole(int thread, bool debug) {
  return consoleArray_ptr->getSpecifiedConsole(thread, debug);
}

VirtualObserver& TetraEnvironment::getObserver() { return *observer; }

void TetraEnvironment::setObserver(VirtualObserver& pObserver) {
  observer = &pObserver;
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
void TetraEnvironment::setDebug(bool toggle) { debugMode = toggle; }

bool TetraEnvironment::isDebugMode() { return debugMode; }

int TetraEnvironment::obtainNewThreadID() {
  int ret = -1;
  pthread_mutex_lock(&next_thread_mutex);
  ret = nextThreadID;
  nextThreadID++;
  pthread_mutex_unlock(&next_thread_mutex);
  return ret;
}

pthread_mutex_t TetraEnvironment::next_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
long TetraEnvironment::nextThreadID = 0;
ConsoleArray* TetraEnvironment::consoleArray_ptr = NULL;
int TetraEnvironment::maxThreads = 8;
bool TetraEnvironment::debugMode = false;
ostream* TetraEnvironment::outputStream = &std::cout;
VirtualObserver* TetraEnvironment::observer = NULL;
bool TetraEnvironment::running = true;
