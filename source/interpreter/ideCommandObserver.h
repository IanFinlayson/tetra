//#ifndef OBSERVER_H
//#define OBSERVER_H

#include "backend.h"
#include "commandObserver.h"

// structure for breakpoints.
// May be specified to work for a specific thread
// May be called to only stop one thread
typedef struct BreakPoint Breakpoint;

// Overload == operator for use in std::find. Only care if lineNo is the same
bool operator==(Breakpoint a, Breakpoint b);

class IDECommandObserver : public CommandObserver {
 private:
  std::vector<Breakpoint, gc_allocator<Breakpoint> > breakpoints;
  // Breakpoint lastLine;
  // bool stepping;
  // bool stopAtNext;
  // std::stack<const Node*> scopes;
  // Stack of vectors of thread-specific variables so the observer can treat
  // them specially
  // std::stack<std::vector<std::string> > threadSpecificVars;

  std::vector<long, gc_allocator<long> > waitingThreads;
  long allowedThread;
  bool yieldEnabled;  // Some imlementations may find it more useful to disable
                      // this feature

  // Linked List insures that if we try to access the list while a new thread is
  // being created, the data structure won;t move on us (such as an Arraylist
  // might if it hit capacity)
  std::list<TetraContext*> threadContexts;

  // Mutex makes sure only one thread is accepting input at a time
  // pthread_mutex_t prompt_mutex;

  // Mutex insures threads atomically add/remove themselves from the debug
  // waiting queue
  pthread_mutex_t threadList_mutex;

  // Mutex allows access to a list of thread contexts. This list allows the
  // debugger to signal that a thread should stop. Can;t use signals
  // TODO there MIGHT be a tiny chance of a bug if we attempt to access this
  // list while a new scope is being pushed on
  pthread_mutex_t context_mutex;

  // Mutex for the breakpoint list. We could use a r/w mutex, but performance is
  // not an issue
  pthread_mutex_t breakList_mutex;

  // Condition variable for when a current thread is done with the console
  pthread_cond_t prompt_condition;

 public:
  IDECommandObserver();
  void notify_E(const Node*, TetraContext& context);
  void notifyThreadSpecificVariable_E(std::string);
  void threadCreated_E(int, TetraContext&);
  void threadDestroyed_E(int);
  void step_E(TetraContext&);
  void next_E(TetraContext&);
  bool break_E(int);
  void continue_E();
  void leftScope_E(TetraContext& context);
  bool remove_E(int);
  void setYieldEnabled(bool);

  // Returns a list of linenumbers of stopped threads
  std::vector<int, gc_allocator<int> > getThreadLocations();
};
