#include"backend.hpp"

//Returns an untyped pointer to a given variable
//Returns NULL if not found
void* VirtualObserver::fetchVariable(std::string s, TetraContext& context) const {
	
	const std::map<std::string,int>& refTable = refTables.top();
	if(globRefTable.find(s) != globRefTable.end()) {
		//Return a pointer to the requested variable
		//Interface requires passing a node pointer
		//Note that although there is a global variable, their was never any mangling of the number (making it negative), so we are fine passing in the normal number without multiplying it by -1
		Node x(NODE_IDENTIFIER);
		x.setIntval(globRefTable.find(s)->second);
		x.setStringval(s);

		void* ret = context.getGlobalScopeRef()->lookupVar<int*>(&x);
		return ret;
	}
	else if(refTable.find(s) != refTable.end()) {
		//Return a pointer to the requested variable
		//Interface requires passing a node pointer
		Node x(NODE_IDENTIFIER);
		x.setIntval(refTable.find(s)->second);
		x.setStringval(s);

		void* ret = context.lookupVar<int*>(&x);
		return ret;
	}
	else {
		return NULL;
	}

/*	if(context.getGlobalScopeRef()->containsVar(s)) {
		return context.getGlobalScopeRef()->lookupVar<void*>(s);
	}
	else if (context.getScopeRef()->containsVar(s)) {	//check current scope
		return context.getScopeRef()->lookupVar<void*>(s);
	}
	else {	//Not found (variable does not exist at current scope)
		return NULL;
	}*/
}

//Since the basic interpreter abstracts away the variable names, while in debug mode,
//the observer needs to keep a reference table whenever entering or leaving a scope so that
//it can fulfill fetchVariable requests

//Checks if the current node references a new variable, and if so, adds it to the table.
//If is a function node, creates a new reference table for the new scope
//Note that this list is created dynamically because variables are officially initialized
//when they are encountered in the interpreter. A prebuilt table for the scope cannot
//reflect that different variables may have been initialized at different locations or
//logicl paths
void VirtualObserver::updateVarReferenceTable(const Node* node) {

	//We might need to push a new table to the stack, or add a new entry to the present table
	if(node->kind() == NODE_FUNCTION) {
		refTables.push(std::map<std::string,int>());
		//Push the formal params on
		Node* paramNode = node->child(0);

		//The variables should not yet exist
		while(paramNode->kind() == NODE_FORMAL_PARAM_LIST) {
			refTables.top()[paramNode->child(0)->getString()] = paramNode->child(0)->getInt();
			paramNode = paramNode->child(1);
		}
		//Push on final parameter (if it exists)
		if(paramNode->kind() == NODE_FORMAL_PARAM) {
			refTables.top()[paramNode->getString()] = paramNode->getInt();
		}

	}
	else if(node->kind() == NODE_IDENTIFIER) {
		//If we have found an identifier, add an entry for it
		//We need this because it is possible that a user will use a variable before putting it in an assignment
		//e.g.
		//if someCondition
		//	x = 1
		//else
		//	squatdiddly
		//print(x)
		
		//Check if the variable already exists
		if(globRefTable.find(node->getString()) == globRefTable.end() && refTables.top().find(node->getString()) == refTables.top().end()){
			std::cout << "Registered: " <<node->getString() << " at " << node->getInt()<<endl;
			refTables.top()[node->getString()] = node->getInt();
		}
	}

	//ASSUMPTION: These nodes always have an identifier or NODE_VECREF of some sort to their left
	else if(node->kind() == NODE_ASSIGN || node->kind() == NODE_GLOBAL || node->kind() == NODE_CONST) {
		//Check that we are assigning to a variable, rather than say, a vector refernece such as x[0][1]
		if(node->child(0)->kind() == NODE_IDENTIFIER){
			//Make sure the var has not already been registered
			cout << node->child(0)->getString() <<"!!!!!!!" << endl;
			if(globRefTable.find(node->child(0)->getString()) == globRefTable.end() && refTables.top().find(node->child(0)->getString()) == refTables.top().end()){
				std::cout << "Registered: " <<node->child(0)->getString() << " at " << node->child(0)->getInt()<<endl;
				refTables.top()[node->child(0)->getString()] = node->child(0)->getInt();
			}
		}
	}
	//else, we don't need to do anything
}

//When leaving a scope, pops the table of string references
void VirtualObserver::popReferenceTable() {
	refTables.pop();
}


