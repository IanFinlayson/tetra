//#ifndef THREAD_ENVIRONMENT_H
//#define THREAD_ENVIRONMENT_H

/*
 * ThreqdEnvironment wraps single global object storing infomration about the states of all threads in the program
 * This class is responsible for ensuring that all threads exit in an orderly fashion
 */

/*#include<iostream>
#include<string>
#include<pthread.h>
#include<vector>
#include<map>
#include<algorithm>
*/

/*
//This class contains everything needed for a thread of execution to keep track of and join back with all threads that it spawns

class ThreadPool {

private:

	pthread_mutex_t threadCount_mutex;
	std::vector<pthread_t> currentThreads;

public:

	ThreadPool();

        //Since the threadCount is a less-visited area, we will use a coarse mutex for reading and writing
        //Atomically queries the number of registered active threads
        int queryThreads();

        //Atomically increments the number of active threads
        void addThread(pthread_t aThread);

        //Atomically decrements the number of active threads
        void removeThread(pthread_t rThread);

        pthread_t getNextJoin();
	
	void waitTillEmpty();
};


class ThreadEnvironment {

private:

	//This holds the count of ADDITIONAL running p-threads (not including the main thread)
	//This allows the main thread to not execute until every thread has finished, so that resources don;t get unexpectedly destroyed while other threads are running
	//(e.g. the Node tree doesn;t get destroyed while another thread is exeuting instructions)
	//Mutex insures that threads don;t simultaneously try to destroy themselves/add new threads
	vector< pthread_t > currentThreads;
	pthread_mutex_t threadCount_mutex;
	ThreadPool backgroundThreads;

	//This vector holds each MUTEX lock created by the program
	//By putting them all in one global location, we allow threads to query whether a particular mutex has been created or not
	map<string, pthread_mutex_t*> mutexes;
	pthread_mutex_t map_mutex;

	//We may want to change the constructor to an initializer, as pthread_create may return errors
	ThreadEnvironment();
	~ThreadEnvironment();

	static ThreadEnvironment instance;

public:

	//Since the threadCount is a less-visited area, we will use a coarse mutex for reading and writing
	//Atomically queries the number of registered active threads
	static int queryThreads();

	//Atomically increments the number of active threads
	static void addThread(pthread_t aThread);

	//Atomically decrements the number of active threads
	static void removeThread(pthread_t rThread);

	static pthread_t getNextJoin();

	//This could use a better name, as the name implies an oxymoron
	static void joinDetachedThreads();

	//This method returns the mutex associated with a string, or creates a new mutex associated with the string and returns that
	static pthread_mutex_t* identifyMutex(string mutexName);
};
//Initializes static variable
//ThreadEnvironment ThreadEnvironment::instance;

#endif
*/
