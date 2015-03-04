/*
 * This file wraps a call to the executeProgram function of main.cpp
 * This command line running by executing this file, while allowing tools access to a method which will build and run the program
 */
#include<iostream>
#include"frontend.hpp"
#include"backend.hpp"
#include"commandConsole.h"
#include"commandObserver.h"
#include<cstdlib>

//These are defined in frontend.hpp and main.cpp respectively
int interpret(Node*);
int interpret(Node*,std::string*,int);

int main(int argc, char** argv) {

	//Check that the proper number of arguments were passed
	if(argc < 2) {
		std::cout << "Please pass a file name!" << std::endl;
		exit(EXIT_FAILURE);
	}

	CommandConsole console;
	CommandObserver observer;

	//observer.break_E(5);

	TetraEnvironment::initialize(console);
	TetraEnvironment::setObserver(observer);

	//cout << &observer << endl;
	//cout << &(TetraEnvironment::getObserver()) << endl;

	//Parse flags
	std::string* flags = NULL;
	int numFlags = argc - 2; //number of flags - prog name and file name
	if(numFlags > 0) {
		flags = new string[numFlags];
		for(int index = 0; index < numFlags; index++) {
			//index+1 to get past program argument
			flags[index] = std::string(argv[index+1]);
		}
	}

	Node* tree;

	//Parse file, and check for initial errors. Print out and exit if an error was found
	try {
		//File is last parameter
		tree = parseFile(argv[argc-1]);
	} catch(Error e) {
		std::cout << "The following error was detected in your program:\n" << e << "\nExecution aborted" << std::endl;
		exit(EXIT_FAILURE);
	}
#ifdef USE_OBSERVER
	//Make it so that the debug prompt will be started at the very beginning
	//TetraEnvironment::getObserver().step_E();
#endif
	std::cout << "Running: " << argv[argc-1] << endl;
	int ret = 0;
	try {   
		ret = interpret(tree,flags,numFlags);
	}
	catch (SystemError e) {
		cout << "The interpreter has entered an undefined state: " << endl;
		cout << e << endl;
		exit(EXIT_FAILURE);
	}
	catch (RuntimeError e) {
		cout << "The following error was detected while running your program: " << endl;
		cout << e << endl;
		e.getContext().printStackTrace();
		exit(EXIT_FAILURE);
	}
	catch (Error e) {
		cout << "The following error was detected in your program: " << endl;
		cout << e << endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "+------------------------------------" << endl;
	std::cout << "|Main Returned: " << ret << endl;
	std::cout << "+------------------------------------" << endl;

	//cout << endl << "For vectors: there were " << numAllocs_T << " allocations and " << numDeallocs_T << " Deallocations." << endl;

	return ret;
}
