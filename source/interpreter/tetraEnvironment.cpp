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
  // pthread_mutex_init(&next_thread_mutex, NULL);
  // It would seem that this method is not supported on this platform
  // maxThreads = pthread_num_processors_np();
  // cout <<"Max threads:" <<maxThreads;
  outputStream = &std::cout;
}

void TetraEnvironment::initialize(const ConsoleArray& console) {
  maxThreads = 8;
  consoleArray_ptr = &console;
  // pthread_mutex_init(&next_thread_mutex, NULL);
  // It would seem that this method is not supported on this platform
  // maxThreads = pthread_num_processors_np();
  // cout <<"Max threads:" <<maxThreads;
  outputStream = &std::cout;
}
void TetraEnvironment::setMaxThreads(int pNum) { maxThreads = pNum; }

void TetraEnvironment::setOutputStream(ostream& pOut) { outputStream = &pOut; }

ostream& TetraEnvironment::getOutputStream() { return *outputStream; }

int TetraEnvironment::getMaxThreads() { return maxThreads; }

void TetraEnvironment::setConsoleArray(const ConsoleArray& pConsole) {
  consoleArray_ptr = &pConsole;
}

const ConsoleArray& TetraEnvironment::getConsoleArray() {
  return *consoleArray_ptr;
}

const VirtualConsole& TetraEnvironment::getConsole(int thread, bool debug) {
  return consoleArray_ptr->getSpecifiedConsole(thread, debug);
}

VirtualObserver& TetraEnvironment::getObserver() { return *observer; }

void TetraEnvironment::setObserver(VirtualObserver& pObserver) {
  observer = &pObserver;
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
ConsoleArray const* TetraEnvironment::consoleArray_ptr = NULL;
int TetraEnvironment::maxThreads = 8;
bool TetraEnvironment::debugMode = false;
ostream* TetraEnvironment::outputStream = &std::cout;
VirtualObserver* TetraEnvironment::observer = NULL;
