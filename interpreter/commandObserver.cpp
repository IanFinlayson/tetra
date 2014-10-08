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
void CommandObserver::notify_E(const Node* foundNode, TetraContext& context) {

	//Check to see if we entered a new scope
	if(foundNode->kind() == NODE_FUNCTION) {
		//cout << "Entering scope: " << foundNode->getString() << endl;
		scopes.push(foundNode);
	}
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
		console.processStandardOutput(msg.str());
		std::string ret = " ";

		while(ret == " ") {
			console.processStandardOutput("Options: (s)tep, (n)ext (c)ontinue (b)reak (p)rint\n");

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
				std::string breakpoint = console.receiveStandardInput();
				int lineNo = atoi(breakpoint.c_str());
				bool success = break_E(lineNo);
				std::stringstream confirmationMessage;
				if(success) {
					confirmationMessage << "Breakpoint set at line: " << lineNo << "\n";
				}
				else {
					confirmationMessage << "Breakpoint already exists at line: " << lineNo << "\n";	
				}
				console.processStandardOutput(confirmationMessage.str());
				//set ret to repeat the input prompt
				ret = " ";
			}
			break;
			case 'r':
			case 'R':
			{
				std::string breakpoint = console.receiveStandardInput();
				int lineNo = atoi(breakpoint.c_str());
				bool success = remove_E(lineNo);
				std::stringstream confirmationMessage;
				if(success) {
					confirmationMessage << "Breakpoint removed from line: " << lineNo << "\n";
				}
				else {
					confirmationMessage << "No breakpoint exists at line " << lineNo << "\n";
				}
				console.processStandardOutput(confirmationMessage.str());
				//set ret to repeat the input prompt
				ret = " ";
			}
			break;
			case 'p':
			case 'P':
			{
				std::string varName = console.receiveStandardInput();
				void* var = fetchVariable(varName,context);
				if(var != NULL) {
					std::stringstream message;
					message << varName;
					const Node* nodey = scopes.top();
					Symbol symbolEntry = nodey->lookupSymbol(varName,0);
					message << " (" << typeToString(symbolEntry.getType()) << "): ";
					switch(symbolEntry.getType()->getKind()) {
						case TYPE_INT:
							//message << " (int): ";
							message << *static_cast<int*>(var);
						break;
						case TYPE_REAL:
							//message << " (real): ";
							message << *static_cast<double*>(var);
						break;
						case TYPE_STRING:
							//message << " (string): ";
							message << *static_cast<std::string*>(var);
						break;
						case TYPE_BOOL:
							//message << " (bool): ";
							message << *static_cast<bool*>(var);
						break;

						case TYPE_VECTOR:
							//message << " (vector): ";
							message << *static_cast<TArray*>(var);
						break;
						case TYPE_VOID:
							//message << " (void): ";
							message << var;
						break;

					}
					message << "\n";
					console.processStandardOutput(message.str());
				}
				else {
					console.processStandardOutput("The variable " + varName + " either does not exist in the current scope, or has not been initialized\n");
				}
				ret = " "; 
			}
			break;
			default:
				console.processStandardOutput("Error, unrecognized command\n");
				//set ret to repeat the prompt for input
				ret = " ";
			}
			//cout << "breakpoints size: " << breakpoints.size() << endl;
		}
	}
}

void CommandObserver::step_E() {
	stepping = true;
}

void CommandObserver::next_E() {
	stopAtNext = true;
}

//Adds a breakpoint associated with the given line number. Returns false if there was already a breakpoint there.
bool CommandObserver::break_E(int lineNum) {
	if(std::find(breakpoints.begin(), breakpoints.end(), lineNum) == breakpoints.end()) {
		breakpoints.push_back(lineNum);
		return true;
	}
	else {
		return false;
	}
}

//Removes a breakpoint from the given line number. Returns false if no breakpoint exists
bool CommandObserver::remove_E(int lineNum) {
	std::vector<int>::iterator location = std::find(breakpoints.begin(), breakpoints.end(), lineNum);
	if(location != breakpoints.end()) {
		breakpoints.erase(location);
		return true;
	}
	else {
		return false;
	}
}


void CommandObserver::continue_E() {
	//do nothing at the moment
}

void CommandObserver::leftScope_E() {
	scopes.pop();
}

