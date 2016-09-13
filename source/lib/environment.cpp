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

/* get a new thread ID */
int TetraEnvironment::obtainNewThreadID() {
    int ret = -1;
    pthread_mutex_lock(&next_thread_mutex);
    ret = nextThreadID;
    nextThreadID++;
    pthread_mutex_unlock(&next_thread_mutex);
    return ret;
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

/* halt the given program ASAP - used so the user can halt buggy programs */
void TetraEnvironment::halt() {
    TetraEnvironment::running = false;
}

bool TetraEnvironment::isRunning() {
    return running;
}

void TetraEnvironment::setRunning() {
    running = true;
}

/* determines whether the interpreter should be executing in debug mode or not
*/
void TetraEnvironment::setDebug(bool toggle) {
    debugMode = toggle;
}

bool TetraEnvironment::isDebugMode() {
    return debugMode;
}

/* static variable initializations for the environment */
int TetraEnvironment::maxThreads = 8;
bool TetraEnvironment::debugMode = false;
bool TetraEnvironment::running = true;
VirtualConsole* TetraEnvironment::console = NULL;
pthread_mutex_t TetraEnvironment::next_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
long TetraEnvironment::nextThreadID = 0;

/* This class contains everything needed for a thread of execution to keep track
 * of and join back with all threads that it spawns */
ThreadPool::ThreadPool() : currentThreads() {
    pthread_mutex_init(&threadCount_mutex, NULL);
}

/* Since the threadCount is a less-visited area, we will use a coarse mutex for
 * reading and writing
 * Atomically queries the number of registered active threads */
int ThreadPool::queryThreads() {
    /* cout << "Querying threads" << endl; */
    pthread_mutex_lock(&threadCount_mutex);
    int ret = currentThreads.size();
    pthread_mutex_unlock(&threadCount_mutex);
    return ret;
}

/* Atomically increments the number of active threads */
void ThreadPool::addThread(pthread_t aThread) {
    /* cout << "Adding thread" << endl; */
    pthread_mutex_lock(&threadCount_mutex);
    currentThreads.push_back(aThread);
    pthread_mutex_unlock(&threadCount_mutex);
}

/* Atomically decrements the number of active threads */

void ThreadPool::removeThread(pthread_t rThread) {
    /* cout << "Removing mutex thread" << endl; */
    pthread_mutex_lock(&threadCount_mutex);
    for (std::vector<pthread_t>::iterator element = currentThreads.begin();
         element < currentThreads.end(); element++) {
        if (*element == rThread) {
            currentThreads.erase(element);
            break;
        }
    }
    pthread_mutex_unlock(&threadCount_mutex);
}

pthread_t ThreadPool::getNextJoin() {
    pthread_t ret;
    /* cout << "Getting next join" << endl; */
    pthread_mutex_lock(&threadCount_mutex);
    ret = currentThreads[0];
    pthread_mutex_unlock(&threadCount_mutex);
    return ret;
}

void ThreadPool::waitTillEmpty() {
    /* This contains a slight error if a thread removes itself between the
     * condition and the join
     * Even with the error case, the program should theoretically work as
     * expected, as join should just return (which is what we want) */
    while (queryThreads() != 0) {
        pthread_join(getNextJoin(), NULL);
        /* cout << "This lock in join area" << endl; */
        pthread_mutex_lock(&threadCount_mutex);
        currentThreads.erase(currentThreads.begin());
        pthread_mutex_unlock(&threadCount_mutex);
    }
}

/* We may want to change the constructor to an initializer, as pthread_create
 * may return errors */
ThreadEnvironment::ThreadEnvironment() : backgroundThreads(), mutexes() {
    pthread_mutex_init(&threadCount_mutex, NULL);
    pthread_mutex_init(&map_mutex, NULL);
}

/* Destructor destroys the allocated mutexes */
ThreadEnvironment::~ThreadEnvironment() {}

/* Since the threadCount is a less-visited area, we will use a coarse mutex for
 * reading and writing
 * Atomically queries the number of registered active threads */
int ThreadEnvironment::queryThreads() {
    return instance.backgroundThreads.queryThreads();
}

/* Atomically increments the number of active threads */
void ThreadEnvironment::addThread(pthread_t aThread) {
    instance.backgroundThreads.addThread(aThread);
}

/* Atomically decrements the number of active threads */
void ThreadEnvironment::removeThread(pthread_t rThread) {
    instance.backgroundThreads.removeThread(rThread);
}

pthread_t ThreadEnvironment::getNextJoin() {
    return instance.backgroundThreads.getNextJoin();
}

void ThreadEnvironment::joinDetachedThreads() {
    instance.backgroundThreads.waitTillEmpty();
}

/* This method returns the mutex associated with a string, or creates a new
 * mutex associated with the string and returns that
 * TODO:This is a candidate for a read/write mutex */
pthread_mutex_t* ThreadEnvironment::identifyMutex(Tstring mutexName) {
    pthread_mutex_lock(&instance.map_mutex);

    if (instance.mutexes.find(mutexName) == instance.mutexes.end()) {
        /* Mutexes are not copy constructable, so the map will have to store
         * references */
        pthread_mutex_t* lock = new pthread_mutex_t();
        pthread_mutex_init(lock, NULL);
        instance.mutexes[mutexName] = lock;
    }

    pthread_mutex_t* ret = instance.mutexes[mutexName];

    pthread_mutex_unlock(&instance.map_mutex);

    return ret;
}

/* Initialize static variable */
ThreadEnvironment ThreadEnvironment::instance;
