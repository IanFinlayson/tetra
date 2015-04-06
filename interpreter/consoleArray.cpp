#include"backend.hpp"

//Default console policy
//Used for when only one console is registered
int singleConsolePolicy(int, bool) {
	return 0;
}

ConsoleArray::ConsoleArray(){
	consoles = std::vector<VirtualConsole*>();
	setConsolePolicy(singleConsolePolicy);
}

ConsoleArray::ConsoleArray(int (policy)(int,bool)){
	consoles = std::vector<VirtualConsole*>();
	setConsolePolicy(policy);
}

VirtualConsole& ConsoleArray::getSpecifiedConsole(int thread, bool debug) {
	return *(consoles[invokeConsolePolicy(thread,debug)]);
}

void ConsoleArray::setConsolePolicy(int (policy)(int,bool)) {
	invokeConsolePolicy = policy;
}

int ConsoleArray::registerConsole(VirtualConsole& newConsole) {
	consoles.push_back(&newConsole);
	return consoles.size()-1;
}

bool ConsoleArray::removeConsole(VirtualConsole& toRemove) {
	for(std::vector<VirtualConsole*>::iterator index = consoles.begin(); index < consoles.end(); index++) {
		if((*index) == &toRemove) {
			consoles.erase(index);
			return true;	
		}
	}
	return false;
}
