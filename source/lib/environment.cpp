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
    maxThreads = 8;
    console = NULL;
    if (next_thread_mutex == NULL) {
        next_thread_mutex = new QMutex();
    }
}

/* get a new thread ID */
int Environment::obtainNewThreadID() {
    int ret = -1;
    next_thread_mutex->lock();
    ret = nextThreadID;
    nextThreadID++;
    next_thread_mutex->unlock();
    return ret;
}

void Environment::setMaxThreads(int pNum) {
    maxThreads = pNum;
}

int Environment::getMaxThreads() {
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

/* static variable initializations for the environment */
int Environment::maxThreads = 8;
bool Environment::debugMode = false;
bool Environment::running = true;
VirtualConsole* Environment::console = NULL;
long Environment::nextThreadID = 0;
QMutex* Environment::next_thread_mutex = NULL;

/* This class contains everything needed for a thread of execution to keep track
 * of and join back with all threads that it spawns */
ThreadPool::ThreadPool() : currentThreads() {}

/* Since the threadCount is a less-visited area, we will use a coarse mutex for
 * reading and writing
 * Atomically queries the number of registered active threads */
int ThreadPool::queryThreads() {
    /* cout << "Querying threads" << endl; */
    threadCount_mutex.lock();
    int ret = currentThreads.size();
    threadCount_mutex.unlock();
    return ret;
}

/* Atomically increments the number of active threads */
void ThreadPool::addThread(QThread* aThread) {
    /* cout << "Adding thread" << endl; */
    threadCount_mutex.lock();
    currentThreads.push_back(aThread);
    threadCount_mutex.unlock();
}

/* Atomically decrements the number of active threads */

void ThreadPool::removeThread(QThread* rThread) {
    /* cout << "Removing mutex thread" << endl; */
    threadCount_mutex.lock();
    for (std::vector<QThread*>::iterator element = currentThreads.begin();
         element < currentThreads.end(); element++) {
        if (*element == rThread) {
            currentThreads.erase(element);
            break;
        }
    }
    threadCount_mutex.unlock();
}

QThread* ThreadPool::getNextJoin() {
    QThread* ret;
    /* cout << "Getting next join" << endl; */
    threadCount_mutex.lock();
    ret = currentThreads[0];
    threadCount_mutex.unlock();
    return ret;
}

void ThreadPool::waitTillEmpty() {
    /* This contains a slight error if a thread removes itself between the
     * condition and the join
     * Even with the error case, the program should theoretically work as
     * expected, as join should just return (which is what we want) */
    while (queryThreads() != 0) {
        getNextJoin()->wait();
        /* cout << "This lock in join area" << endl; */
        threadCount_mutex.lock();
        currentThreads.erase(currentThreads.begin());
        threadCount_mutex.unlock();
    }
}

ThreadEnvironment::ThreadEnvironment() : backgroundThreads(), mutexes() {}

/* Destructor destroys the allocated mutexes */
ThreadEnvironment::~ThreadEnvironment() {}

/* Since the threadCount is a less-visited area, we will use a coarse mutex for
 * reading and writing
 * Atomically queries the number of registered active threads */
int ThreadEnvironment::queryThreads() {
    return instance.backgroundThreads.queryThreads();
}

/* Atomically increments the number of active threads */
void ThreadEnvironment::addThread(QThread* aThread) {
    instance.backgroundThreads.addThread(aThread);
}

/* Atomically decrements the number of active threads */
void ThreadEnvironment::removeThread(QThread* rThread) {
    instance.backgroundThreads.removeThread(rThread);
}

QThread* ThreadEnvironment::getNextJoin() {
    return instance.backgroundThreads.getNextJoin();
}

void ThreadEnvironment::joinDetachedThreads() {
    instance.backgroundThreads.waitTillEmpty();
}

/* This method returns the mutex associated with a string, or creates a new
 * mutex associated with the string and returns that */
QMutex* ThreadEnvironment::identifyMutex(String mutexName) {
    instance.map_mutex.lock();

    if (instance.mutexes.find(mutexName) == instance.mutexes.end()) {
        /* Mutexes are not copy constructable, so the map will have to store
         * references */
        QMutex* lock = new QMutex();
        instance.mutexes[mutexName] = lock;
    }

    QMutex* ret = instance.mutexes[mutexName];

    instance.map_mutex.unlock();

    return ret;
}

/* Initialize static variable */
ThreadEnvironment ThreadEnvironment::instance;
