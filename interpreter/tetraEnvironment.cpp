#include"backend.hpp"
#include<pthread.h>
#include<iostream>

/*
 * This class stores the constants relevant to the Tetra Environment including:
 * number of threads
 * where to print
 */

void TetraEnvironment::initialize() {
	maxThreads = 8;
	console_ptr = NULL;
	//pthread_mutex_init(&next_thread_mutex, NULL);
	//It would seem that this method is not supported on this platform
	//maxThreads = pthread_num_processors_np();
	//cout <<"Max threads:" <<maxThreads;
	outputStream = &std::cout;
}

void TetraEnvironment::initialize(const VirtualConsole& console) {
	maxThreads = 8;
	console_ptr = &console;
	//pthread_mutex_init(&next_thread_mutex, NULL);
	//It would seem that this method is not supported on this platform
	//maxThreads = pthread_num_processors_np();
	//cout <<"Max threads:" <<maxThreads;
	outputStream = &std::cout;
}
void TetraEnvironment::setMaxThreads(int pNum) {
	maxThreads = pNum;
}

void TetraEnvironment::setOutputStream(ostream& pOut) {
	outputStream = &pOut;
}

ostream& TetraEnvironment::getOutputStream() {
	return *outputStream;
}

int TetraEnvironment::getMaxThreads() {
	return maxThreads;
}

void TetraEnvironment::setConsole(const VirtualConsole& pConsole) {
	console_ptr = &pConsole;
}

const VirtualConsole& TetraEnvironment::getConsole() {
	return *console_ptr; 
}

VirtualObserver& TetraEnvironment::getObserver() {
	return *observer;
}

void TetraEnvironment::setObserver(VirtualObserver& pObserver) {
	observer = &pObserver;
}

//Determines whether the interpreter should be executing in debug mode or not
void TetraEnvironment::setDebug(bool toggle) {
	debugMode = toggle;
}

bool TetraEnvironment::isDebugMode() {
	return debugMode;
}

//Sets options based on the flags that get passed in.
//Returns "" on successful parsing, otherwise returns an error message
std::string TetraEnvironment::parseFlags(std::string* flags, int flagCount) {
	if(flags==NULL||flagCount == 0) {
		return "";
	}

	std::string ret = "";

	for(int index = 0; index < flagCount; index++) {
		//For the time being, these will be case sensitive
		std::string arg = flags[index];
		if(arg == "-debug" || arg == "-d") {
			setDebug(true);
		}
		else {
			ret += "Failed to recognize option: " + arg + "\n";
		}
	}

	return ret;
}

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
VirtualConsole const * TetraEnvironment::console_ptr = NULL;
int TetraEnvironment::maxThreads = 8;
bool TetraEnvironment::debugMode = false;
ostream* TetraEnvironment::outputStream = &std::cout;
VirtualObserver* TetraEnvironment::observer = NULL;
