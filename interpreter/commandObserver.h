#ifndef OBSERVER_H
#define OBSERVER_H

#include<vector>
#include"backend.hpp"

struct BreakPoint {
        int lineNo;
        std::string threadLabel;
        bool stable;

};

typedef struct BreakPoint Breakpoint;

//Overload == operator for use in std::find. Only care if lineNo is the same
bool operator==(Breakpoint a, Breakpoint b);

class CommandObserver : public VirtualObserver{

private:
	std::vector<Breakpoint> breakpoints;
	Breakpoint lastLine;
	bool stepping;
	bool stopAtNext;
	std::stack<const Node*> scopes;
public:
	CommandObserver();
	void notify_E(const Node*, TetraContext& context);
	void step_E();
	void next_E();
	bool break_E(int);
	void continue_E();
	void leftScope_E();
	bool remove_E(int);
};

#endif
