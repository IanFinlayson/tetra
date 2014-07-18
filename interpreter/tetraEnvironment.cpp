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

int TetraEnvironment::maxThreads = 8;
ostream* TetraEnvironment::outputStream = &std::cout;
