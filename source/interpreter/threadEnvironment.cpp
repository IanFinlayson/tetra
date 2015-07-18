/*
 * ThreqdEnvironment wraps single global object storing infomration about the states of all threads in the program
 * This class is responsible for ensuring that all threads exit in an orderly fashion
 */

#include<iostream>
#include<string>
#include<pthread.h>
#include<vector>
#include<map>
#include<algorithm>
#include"backend.hpp"

//This will be removed
using namespace std;

//This class contains everything needed for a thread of execution to keep track of and join back with all threads that it spawns
ThreadPool::ThreadPool() : currentThreads() {
  pthread_mutex_init(&threadCount_mutex, NULL);
}

//Since the threadCount is a less-visited area, we will use a coarse mutex for reading and writing
//Atomically queries the number of registered active threads
int ThreadPool::queryThreads() {
  //cout << "Querying threads" << endl;
  pthread_mutex_lock(&threadCount_mutex);
  int ret = currentThreads.size();    
  pthread_mutex_unlock(&threadCount_mutex);
  return ret;
}   

//Atomically increments the number of active threads
void ThreadPool::addThread(pthread_t aThread) {
  //cout << "Adding thread" << endl;
  pthread_mutex_lock(&threadCount_mutex);
  currentThreads.push_back(aThread);
  pthread_mutex_unlock(&threadCount_mutex);
}   

//Atomically decrements the number of active threads

void ThreadPool::removeThread(pthread_t rThread) {
  //cout << "Removing mutex thread" << endl;
  pthread_mutex_lock(&threadCount_mutex);
  for(vector<pthread_t>::iterator element = currentThreads.begin(); element < currentThreads.end(); element++) {
    if(*element == rThread) {
      currentThreads.erase(element);
      break;
    }   
  }   
  pthread_mutex_unlock(&threadCount_mutex);
}   

pthread_t ThreadPool::getNextJoin() {
  pthread_t ret;
  //cout << "Getting next join" << endl;
  pthread_mutex_lock(&threadCount_mutex);
  ret = currentThreads[0];
  pthread_mutex_unlock(&threadCount_mutex);
  return ret;
}

void ThreadPool::waitTillEmpty() {
  //This contains a slight error if a thread removes itself between the condition and the join
  //Even with the error case, the program should theoretically work as expected, as join should just return (which is what we want)
  while(queryThreads() != 0) {
    pthread_join(getNextJoin(), NULL);
    //cout << "This lock in join area" << endl;
    pthread_mutex_lock(&threadCount_mutex);
    currentThreads.erase(currentThreads.begin());
    pthread_mutex_unlock(&threadCount_mutex);
  }
}



//We may want to change the constructor to an initializer, as pthread_create may return errors
ThreadEnvironment::ThreadEnvironment() : backgroundThreads(), mutexes() {
  pthread_mutex_init(&threadCount_mutex,NULL);
  pthread_mutex_init(&map_mutex, NULL);
}
/*
   void ThreadEnvironment::initializeThreadEnvironment() {
   ThreadPool instance.backgroundThreads;
   map<string, pthread_mutex_t*> instance.mutexes;
   pthread_mutex_init(&instance.map_mutex, NULL);
   }
   */

//Destructor destroys the allocated mutexes
ThreadEnvironment::~ThreadEnvironment() {
  for(std::map<string,pthread_mutex_t*>::iterator iter = mutexes.begin(); iter != mutexes.end(); iter++) {
    delete iter->second;
  }
}

//Since the threadCount is a less-visited area, we will use a coarse mutex for reading and writing
//Atomically queries the number of registered active threads
int ThreadEnvironment::queryThreads() {
  /*pthread_mutex_lock(&instance.threadCount_mutex);
    int ret = instance.currentThreads.size();	
    pthread_mutex_unlock(&instance.threadCount_mutex);
    return ret;*/
  return instance.backgroundThreads.queryThreads();
}

//Atomically increments the number of active threads
void ThreadEnvironment::addThread(pthread_t aThread) {
  /*pthread_mutex_lock(&instance.threadCount_mutex);
    instance.currentThreads.push_back(aThread);
    pthread_mutex_unlock(&instance.threadCount_mutex);*/
  instance.backgroundThreads.addThread(aThread);
}

//Atomically decrements the number of active threads
void ThreadEnvironment::removeThread(pthread_t rThread) {
  /*
     pthread_mutex_lock(&instance.threadCount_mutex);
     for(vector<pthread_t>::iterator element = instance.currentThreads.begin(); element < instance.currentThreads.end(); element++) {
     if(*element == rThread) {
     instance.currentThreads.erase(element);
     break;
     }
     }
     pthread_mutex_unlock(&instance.threadCount_mutex);*/
  instance.backgroundThreads.removeThread(rThread);
}

pthread_t ThreadEnvironment::getNextJoin() {
  /*pthread_t ret;
    pthread_mutex_lock(&instance.threadCount_mutex);
    ret = instance.currentThreads[0];
    pthread_mutex_unlock(&instance.threadCount_mutex);
    return ret;*/
  return instance.backgroundThreads.getNextJoin();
}

//This could use a better name, as the name implies an oxymoron
void ThreadEnvironment::joinDetachedThreads() {
  instance.backgroundThreads.waitTillEmpty();
}

//This method returns the mutex associated with a string, or creates a new mutex associated with the string and returns that
//TODO:This is a candidate for a read/write mutex
pthread_mutex_t* ThreadEnvironment::identifyMutex(string mutexName) {

  //cout << "Identifying mutex" << endl;
  pthread_mutex_lock(&instance.map_mutex);

  if(instance.mutexes.find(mutexName) == instance.mutexes.end()) {
    //cout << "Inserting new mutex" << endl;
    //Mutexes are not copy constructable, so the map will have to store references
    pthread_mutex_t* lock = new pthread_mutex_t();
    pthread_mutex_init(lock,NULL);
    instance.mutexes[mutexName] = lock;
  }

  pthread_mutex_t* ret = instance.mutexes[mutexName];

  pthread_mutex_unlock(&instance.map_mutex);

  return ret;
}

//Initialize static variable
ThreadEnvironment ThreadEnvironment::instance;

