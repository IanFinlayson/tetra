/*
 * This file wraps a call to the executeProgram function of main.cpp
 * This command line running by executing this file, while allowing tools access to a method which will build and run the program
 */
#include<iostream>
#include"frontend.hpp"
#include"backend.hpp"
#include<cstdlib>

//These are defined in frontend.hpp and main.cpp respectively
int interpret(Node*);

int main(int argc, char** argv) {

	//Check that the proper number of arguments were passed
	if(argc < 2) {
		std::cout << "Please pass a file name!" << std::endl;
		exit(EXIT_FAILURE);
	}

	Node* tree;

	//Parse file, and check for initial errors. Print out and exit if an error was found
	try {
		tree = parseFile(argv[1]);
	} catch(Error e) {
		std::cout << "The following error was detected in your program:\n" << e << "\nExecution aborted" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Running: " << argv[1] << endl;
	int ret = 0;
	try {   
		ret = interpret(tree);
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

	return ret;
}
