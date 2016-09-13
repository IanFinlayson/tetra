#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "eval.h"

/* stores constants related to the environment where the program is running */
class TetraEnvironment {
   public:
    static void initialize();
    static VirtualConsole* getConsole();
    static void setConsole(VirtualConsole* console);

    static int obtainNewThreadID();
    static int getMaxThreads();
    static void setMaxThreads(int);
    static void setDebug(bool);
    static bool isDebugMode();
    static void halt();
    static bool isRunning();
    static void setRunning();

   private:
    static bool running;
    static int maxThreads;
    static VirtualConsole* console;
    static bool debugMode;

    static long nextThreadID;
    static pthread_mutex_t next_thread_mutex;
};

class ThreadPool {
   private:
    pthread_mutex_t threadCount_mutex;
    std::vector<pthread_t> currentThreads;

   public:
    ThreadPool();

    /* since the threadCount is a less-visited area, we will use a coarse mutex
     * for reading and writing
     * atomically queries the number of registered active threads */
    int queryThreads();

    /* atomically increments the number of active threads */
    void addThread(pthread_t aThread);

    /* atomically decrements the number of active threads */
    void removeThread(pthread_t rThread);

    pthread_t getNextJoin();

    void waitTillEmpty();
};

class ThreadEnvironment {
   private:
    /* this holds the count of ADDITIONAL running p-threads (not including the
     *  main thread)
     *  this allows the main thread to not execute until every thread has finished,
     *  so that resources don;t get unexpectedly destroyed while other threads are
     *  running
     * (e.g. the Node tree doesn;t get destroyed while another thread is exeuting
     * instructions)
     *  mutex insures that threads don;t simultaneously try to destroy
     *  themselves/add new threads */
    std::vector<pthread_t> currentThreads;
    pthread_mutex_t threadCount_mutex;
    ThreadPool backgroundThreads;

    /* This vector holds each MUTEX lock created by the program
     * By putting them all in one global location, we allow threads to query
     * whether a particular mutex has been created or not */
    std::map<Tstring, pthread_mutex_t*> mutexes;
    pthread_mutex_t map_mutex;

    /* we may want to change the constructor to an initializer, as pthread_create
     * may return errors */
    ThreadEnvironment();
    ~ThreadEnvironment();

    static ThreadEnvironment instance;

   public:
    /* since the threadCount is a less-visited area, we will use a coarse mutex
     * for reading and writing
     * atomically queries the number of registered active threads */
    static int queryThreads();

    /* atomically increments the number of active threads */
    static void addThread(pthread_t aThread);

    /* atomically decrements the number of active threads */
    static void removeThread(pthread_t rThread);

    static pthread_t getNextJoin();

    /* join any threads still running */
    static void joinDetachedThreads();

    /* this method returns the mutex associated with a string, or creates a new
     * mutex associated with the string and returns that */
    static pthread_mutex_t* identifyMutex(Tstring mutexName);
};

#endif
