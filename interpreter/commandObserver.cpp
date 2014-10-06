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
		cout << "Entering scope: " << foundNode->getString() << endl;
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
				break_E(lineNo);
				std::stringstream confirmationMessage;
				confirmationMessage << "Breakpoint set at line: " << lineNo << "\n";
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
					Symbol symbolEntry = const_cast<Node*>(nodey)->lookupSymbol(varName,0);
					switch(symbolEntry.getType()->getKind()) {
						case TYPE_INT:
							message << " (int): ";
							message << *static_cast<int*>(var);
						break;
						case TYPE_REAL:
							message << " (real): ";
							message << *static_cast<double*>(var);
						break;
						case TYPE_STRING:
							message << " (string): ";
							message << *static_cast<std::string*>(var);
						break;
						case TYPE_BOOL:
							message << " (bool): ";
							message << *static_cast<bool*>(var);
						break;

						case TYPE_VECTOR:
							message << " (vector): ";
							message << *static_cast<TArray*>(var);
						break;
						case TYPE_VOID:
							message << " (void): ";
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

void CommandObserver::leftScope_E() {
	scopes.pop();
}

