#include"backend.h"

//Default console policy
//Used for when only one console is registered
int singleConsolePolicy(int, bool) {
  return 0;
}

ConsoleArray::ConsoleArray() {
  consoles = std::vector<VirtualConsole*>();
  setConsolePolicy(singleConsolePolicy);
}

ConsoleArray::ConsoleArray(int (policy)(int,bool)) {
  consoles = std::vector<VirtualConsole*>();
  setConsolePolicy(policy);
}

//Delete the dynamically allocated threading primitives
ConsoleArray::~ConsoleArray() {
  for(unsigned int x = 0; x < consoleMutexes.size(); ++x) {
    delete consoleMutexes[x].first;
    delete consoleMutexes[x].second;
  }
}

VirtualConsole& ConsoleArray::getSpecifiedConsole(int thread, bool debug) const {
  return *(consoles[invokeConsolePolicy(thread,debug)]);
}

void ConsoleArray::obtainConsoleMutex(int thread, bool debug) const {
  pthread_mutex_lock(consoleMutexes[invokeConsolePolicy(thread, debug)].first);
}

void ConsoleArray::releaseConsoleMutex(int thread, bool debug) const {
  pthread_mutex_unlock(consoleMutexes[invokeConsolePolicy(thread, debug)].first);
}

void ConsoleArray::waitOnCondition(int thread, bool debug) const {
  pthread_cond_wait(consoleMutexes[invokeConsolePolicy(thread, debug)].second, consoleMutexes[invokeConsolePolicy(thread,debug)].first);
}

void ConsoleArray::broadcastCondition(int thread, bool debug) const {
  pthread_cond_broadcast(consoleMutexes[invokeConsolePolicy(thread, debug)].second);
}



void ConsoleArray::setConsolePolicy(int (policy)(int,bool)) {
  invokeConsolePolicy = policy;
}


//Registers the console and creates an associated mutex
int ConsoleArray::registerConsole(VirtualConsole& newConsole) {
  consoles.push_back(&newConsole);
  pthread_mutex_t* cMut = new pthread_mutex_t();
  int success = pthread_mutex_init(cMut,NULL);
  assert(success == 0);
  pthread_cond_t* cCon = new pthread_cond_t();
  success = pthread_cond_init(cCon,NULL);
  assert(success == 0);
  consoleMutexes.push_back(std::pair<pthread_mutex_t*,pthread_cond_t*>(cMut,cCon));
  return consoles.size()-1;
}

//Admittedly, it is easier to just clean these up at the end of the program.
//The user is already required to make sure the thread policy is correct, no need
//To put an additional burden on ourselves!
/*
   bool ConsoleArray::removeConsole(VirtualConsole& toRemove) {
   for(std::vector<VirtualConsole*>::iterator index = consoles.begin(); index < consoles.end(); index++) {
   if((*index) == &toRemove) {
   consoles.erase(index);
   return true;	
   }
   }
   return false;
   }
   */
