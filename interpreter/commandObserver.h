#ifndef OBSERVER_H
#define OBSERVER_H

#include<vector>
#include"backend.hpp"

struct BreakPoint {
        int lineNo;
        long threadLabel;
        bool stable;
};

typedef struct BreakPoint Breakpoint;

//Overload == operator for use in std::find. Only care if lineNo is the same
bool operator==(Breakpoint a, Breakpoint b);

class CommandObserver : public VirtualObserver{

private:
	std::vector<Breakpoint> breakpoints;
	//Breakpoint lastLine;
	//bool stepping;
	//bool stopAtNext;
	std::stack<const Node*> scopes;

	std::vector<long> waitingThreads;
	long allowedThread;

	//Linked List insures that if we try to access the list while a new thread is being created, the data structure won;t move on us (such as an Arraylist might if it hit capacity)
	std::list<TetraContext*> threadContexts;


	//Mutex makes sure only one thread is accepting input at a time
	pthread_mutex_t prompt_mutex;

	//Mutex insures threads atomically add/remove themselves from the debug waiting queue
	pthread_mutex_t threadList_mutex;

	//Mutex allows access to a list of thread contexts. This list allows the debugger to signal that a thread should stop. Can;t use signals 
	//TODO there MIGHT be a tiny chance of a bug if we attempt to access this list while a new scope is being pushed on
	pthread_mutex_t contextMutex;

	//Mutex for the breakpoint list. We could use a r/w mutex, but performance is not an issue
	pthread_mutex_t breakList_mutex;

	//Condition variable for when a current thread is done with the console
	pthread_cond_t prompt_condition;
public:
	CommandObserver();
	void notify_E(const Node*, TetraContext& context);
	void threadCreated_E(int,TetraContext&);
	void threadDestroyed_E(int);
	void step_E(TetraContext&);
	void next_E(TetraContext&);
	bool break_E(int);
	void continue_E();
	void leftScope_E();
	bool remove_E(int);
};

#endif
