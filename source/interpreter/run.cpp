/*
 * This file wraps a call to the executeProgram function of main.cpp
 * This command line running by executing this file, while allowing tools access to a method which will build and run the program
 */
#include<iostream>
#include"frontend.h"
#include"backend.h"
#include"commandConsole.h"
#include"commandObserver.h"
#include<cstdlib>

//These are defined in frontend.h and main.cpp respectively
int interpret(Node*);
int interpret(Node*,std::string*,int);

int main(int argc, char** argv) {

  //Check that the proper number of arguments were passed
  if(argc < 2) {
    std::cout << "Please pass a file name!" << std::endl;
    exit(EXIT_FAILURE);
  }

  ConsoleArray console;
  CommandConsole mainConsole = CommandConsole();
  console.registerConsole(mainConsole);
  CommandObserver observer;

  //observer.break_E(5);

  TetraEnvironment::initialize(console);
  TetraEnvironment::setObserver(observer);

  TetraEnvironment::setMaxThreads(8);

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

  //The flags were dynamically allocated
  //TODO i is probably possible to just pass a sub-array of argv to the interpreter
  delete [] flags;

  std::cout << std::endl;

  return ret;
}


