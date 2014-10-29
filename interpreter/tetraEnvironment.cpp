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
	//It would seem that this method is not supported on this platform
	//maxThreads = pthread_num_processors_np();
	//cout <<"Max threads:" <<maxThreads;
	outputStream = &std::cout;
}

void TetraEnvironment::initialize(const VirtualConsole& console) {
	maxThreads = 8;
	console_ptr = &console;
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

VirtualConsole const * TetraEnvironment::console_ptr = NULL;
int TetraEnvironment::maxThreads = 8;
ostream* TetraEnvironment::outputStream = &std::cout;
VirtualObserver* TetraEnvironment::observer = NULL;
