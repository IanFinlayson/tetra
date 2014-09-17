#ifndef OBSERVER_H
#define OBSERVER_H

#include<vector>

//Forward declarations
class Node;

class CommandObserver {

private:
	std::vector<int> breakpoints;
	int lastLine;
	bool stepping;
	bool stopAtNext;

public:
	CommandObserver();
	void notify_E(const Node*);
	void step_E();
	void next_E();
	void break_E(int);
	void continue_E();
};

#endif
