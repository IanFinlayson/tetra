#ifndef OBSERVER_H
#define OBSERVER_H

#include<vector>
#include"backend.hpp"

class CommandObserver : public VirtualObserver{

private:
	std::vector<int> breakpoints;
	int lastLine;
	bool stepping;
	bool stopAtNext;
	std::stack<const Node*> scopes;
public:
	CommandObserver();
	void notify_E(const Node*, TetraContext& context);
	void step_E();
	void next_E();
	void break_E(int);
	void continue_E();
	void leftScope_E();
};

#endif
