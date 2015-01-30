#include"backend.hpp"

//Returns an untyped pointer to a given variable
//Returns NULL if not found
void* VirtualObserver::fetchVariable(std::string s, TetraContext& context) const {
	//Check global scope
	if(context.getGlobalScopeRef()->containsVar(s)) {
		return context.getGlobalScopeRef()->lookupVar<void*>(s);
	}
	else if (context.getScopeRef()->containsVar(s)) {	//check current scope
		return context.getScopeRef()->lookupVar<void*>(s);
	}
	else {	//Not found (variable does not exist at current scope)
		return NULL;
	}
}
