#include"frontend.hpp"
#include"backend.hpp"
#include"commandObserver.h"
#include<sstream>

CommandObserver::CommandObserver() {
	lastLine = 0;
	stepping = false;
	stopAtNext = true;
}

//Notifies the observer that a certain node in the program has been reached
void CommandObserver::notify_E(const Node* foundNode) {

	/*if(breakpoints.size() <= 0) {
		return;
	}*/
	int currentLine = foundNode->getLine();
	//check if line is a breakpoint
	//If the current line is the same as the last line we broke at, we should not break, and let the instruction finish
	//This is unless we are stepping
	if((std::find(breakpoints.begin(), breakpoints.end(),currentLine) != breakpoints.end()
		&& currentLine != lastLine) || (currentLine != lastLine && stopAtNext == true) || stepping == true) {

		stepping = false;
		stopAtNext = false;
		lastLine = currentLine;
		//break, and wait for input
		const VirtualConsole& console = TetraEnvironment::getConsole();

		std::stringstream msg;

		msg << "Breakpoint reached at line: " << currentLine << endl;
		msg << "Stopped at node of kind: " << foundNode->kind() << endl;
		msg << "Options: (s)tep, (n)ext (c)ontinue (b)reak" << endl;

		console.processStandardOutput(msg.str());
		std::string ret = " ";

		while(ret == " ") {
			ret = console.receiveStandardInput();
	
			switch (ret[0]) {
			case 'c':
			case 'C':
				continue_E();
			break;
			case 'n':
			case 'N':
				next_E();
			break;
			case 's':
			case 'S':
				step_E();
			break;
			case 'b':
			case 'B':
			{
				string breakpoint = console.receiveStandardInput();
				int lineNo = atoi(breakpoint.c_str());
				break_E(lineNo);
				//set ret to repeat the input prompt
				ret = " ";
			}
			break;
			default:
				console.processStandardOutput("Error, unrecognized command\n");
				//set ret to repeat the prompt for input
				ret = " ";
			}
		}
	}
}

void CommandObserver::step_E() {
	stepping = true;
}

void CommandObserver::next_E() {
	stopAtNext = true;
}

void CommandObserver::break_E(int lineNum) {
	breakpoints.push_back(lineNum);
}

void CommandObserver::continue_E() {
	//do nothing at the moment
}

