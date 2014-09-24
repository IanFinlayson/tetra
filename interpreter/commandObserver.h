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

public:
	CommandObserver();
	void notify_E(const Node*);
	void step_E();
	void next_E();
	void break_E(int);
	void continue_E();
};

#endif
