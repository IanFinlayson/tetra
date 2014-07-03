#ifndef THREAD_ENVIRONMENT_H
#define THREAD_ENVIRONMENT_H

/*
 * This class wraps single global object storing infomration about the states of all threads in the program
 * This class is responsible for ensuring that all threads exit in an orderly fashion
 */

#include<iostream>
#include<string>
#include<pthread.h>
#include<vector>
#include<map>
#include<algorithm>

//This will be removed
using namespace std;

class ThreadEnvironment {

private:

	//This holds the count of ADDITIONAL running p-threads (not including the main thread)
	//This allows the main thread to not execute until every thread has finished, so that resources don;t get unexpectedly destroyed while other threads are running
	//(e.g. the Node tree doesn;t get destroyed while another thread is exeuting instructions)
	//Mutex insures that threads don;t simultaneously try to destroy themselves/add new threads
	vector< pthread_t > currentThreads;
	pthread_mutex_t threadCount_mutex;

	//This vector holds each MUTEX lock created by the program
	//By putting them all in one global location, we allow threads to query whether a particular mutex has been created or not
	map<string, pthread_mutex_t*> mutexes;
	pthread_mutex_t map_mutex;

	//We may want to change the constructor to an initializer, as pthread_create may return errors
	ThreadEnvironment() : currentThreads(), mutexes() {
		pthread_mutex_init(&threadCount_mutex,NULL);
		pthread_mutex_init(&map_mutex, NULL);
	}

	static ThreadEnvironment instance;

public:

	//Since the threadCount is a less-visited area, we will use a coarse mutex for reading and writing
	//Atomically queries the number of registered active threads
	static int queryThreads() {
		pthread_mutex_lock(&instance.threadCount_mutex);
		int ret = instance.currentThreads.size();	
		pthread_mutex_unlock(&instance.threadCount_mutex);
		return ret;
	}

	//Atomically increments the number of active threads
	static void addThread(pthread_t aThread) {
		pthread_mutex_lock(&instance.threadCount_mutex);
		instance.currentThreads.push_back(aThread);
		pthread_mutex_unlock(&instance.threadCount_mutex);
	}

	//Atomically decrements the number of active threads
	static void removeThread(pthread_t rThread) {
		pthread_mutex_lock(&instance.threadCount_mutex);
		for(vector<pthread_t>::iterator element = instance.currentThreads.begin(); element < instance.currentThreads.end(); element++) {
			if(*element == rThread) {
				instance.currentThreads.erase(element);
				break;
			}
		}
		pthread_mutex_unlock(&instance.threadCount_mutex);
	}

	static pthread_t getNextJoin() {
		pthread_t ret;
		pthread_mutex_lock(&instance.threadCount_mutex);
		ret = instance.currentThreads[0];
		pthread_mutex_unlock(&instance.threadCount_mutex);
		return ret;
	}

	//This method returns the mutex associated with a string, or creates a new mutex associated with the string and returns that
	static pthread_mutex_t* identifyMutex(string mutexName) {

		pthread_mutex_lock(&instance.map_mutex);

		if(instance.mutexes.find(mutexName) == instance.mutexes.end()) {
			//Mutexes are not copy constructable, so the map will have to store references
			pthread_mutex_t* lock = new pthread_mutex_t();
			instance.mutexes[mutexName] = lock;
		}

		pthread_mutex_t* ret = instance.mutexes[mutexName];

		pthread_mutex_unlock(&instance.map_mutex);

		return ret;
	}
};
//Initializes static variable
ThreadEnvironment ThreadEnvironment::instance;
#endif
