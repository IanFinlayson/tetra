#include "backend.hpp"
#include "frontend.hpp"
#include<iostream>

/*
 * This class stores constants related to the environment where the program is run, including:
 * -number of allowed threads
 * -where to print for print statements
 */ 

class TetraEnvironment {
public:
	static void initialize()
	static int getMaxThreads();
	static void setMaxThreads(int);
	static ostream& getOutputStream();
	static void setOutputStream(ostream&);
	
private:
	static int maxThreads;
	static ostream* outputStream;
}
