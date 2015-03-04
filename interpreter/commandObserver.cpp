#include"frontend.hpp"
#include"backend.hpp"
#include"commandObserver.h"
#include<sstream>

//Global symbol table
extern std::map<std::string, Symbol> globals;


CommandObserver::CommandObserver() {
	lastLine.lineNo = 0;
	stepping = false;
	stopAtNext = false;
}

bool operator==(Breakpoint a, Breakpoint b) {
	return a.lineNo == b.lineNo;
}

bool operator!=(Breakpoint a, Breakpoint b) {
	return a.lineNo != b.lineNo;
}

//Notifies the observer that a certain node in the program has been reached
void CommandObserver::notify_E(const Node* foundNode, TetraContext& context) {

	//Check to see if we entered a new scope
	if(foundNode->kind() == NODE_FUNCTION) {
		cout << "Entering scope: " << foundNode->getString() << endl;
		scopes.push(foundNode);
	}

	int currentLine = foundNode->getLine();
	
	Breakpoint linecomp;
	linecomp.lineNo = currentLine;
	//check if line is a breakpoint
	//If the current line is the same as the last line we broke at, we should not break, and let the instruction finish
	//This is unless we are stepping
	if((std::find(breakpoints.begin(), breakpoints.end(),linecomp) != breakpoints.end()
		&& currentLine != lastLine.lineNo) || (currentLine != lastLine.lineNo && stopAtNext == true) || stepping == true) {

		stepping = false;
		stopAtNext = false;
		lastLine.lineNo = currentLine;
		//break, and wait for input
		const VirtualConsole& console = TetraEnvironment::getConsole();

		std::stringstream msg;

		msg << "Breakpoint reached at line: " << currentLine << endl;
		msg << "Stopped at node of kind: " << foundNode->kind() << endl;
		console.processStandardOutput(msg.str());
		std::string ret = " ";

		while(ret == " ") {
			console.processStandardOutput("Options: (s)tep, (n)ext (c)ontinue (b)reak (r)emove (p)rint\n");

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
				std::string breakP = console.receiveStandardInput();
				int lineNo = atoi(breakP.c_str());
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
				std::string breakP = console.receiveStandardInput();
				int lineNo = atoi(breakP.c_str());
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
					//get the function node which has the local scope's symbol table
					const Node* nodey = scopes.top();

					//Determine the datatype of the entry by looking in the local or global symbol table
					Symbol symbolEntry;
					if(const_cast<Node*>(nodey)->hasSymbol(varName)) {
						symbolEntry = nodey->lookupSymbol(varName,0);
					}
					else {	//Variable exists in global scope
						symbolEntry = globals[varName];		
					}
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
	Breakpoint toPush;
	toPush.lineNo = lineNum;

	if(std::find(breakpoints.begin(), breakpoints.end(), toPush) == breakpoints.end()) {
		breakpoints.push_back(toPush);
		return true;
	}
	else {
		return false;
	}
}

//Removes a breakpoint from the given line number. Returns false if no breakpoint exists
bool CommandObserver::remove_E(int lineNum) {
	Breakpoint toPop;
	toPop.lineNo = lineNum;
	std::vector<Breakpoint>::iterator location = std::find(breakpoints.begin(), breakpoints.end(), toPop);
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
