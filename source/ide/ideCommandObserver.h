//#ifndef OBSERVER_H
//#define OBSERVER_H

#include "backend.h"

// structure for breakpoints.
// May be specified to work for a specific thread
// May be called to only stop one thread
typedef struct BreakPoint Breakpoint;

// Overload == operator for use in std::find. Only care if lineNo is the same
bool operator==(Breakpoint a, Breakpoint b);

class IDECommandObserver : public VirtualObserver {
 private:
  std::vector<Breakpoint, gc_allocator<Breakpoint> > breakpoints;
  std::vector<long, gc_allocator<long> > waitingThreads;

  // Linked List insures that if we try to access the list while a new thread is
  // being created, the data structure won;t move on us (such as an Arraylist
  // might if it hit capacity)
  std::list<TetraContext*> threadContexts;

  // Mutex makes sure only one thread is accepting input at a time
  // pthread_mutex_t prompt_mutex;

 public:
  IDECommandObserver();
  ~IDECommandObserver();
  void notify_E(const Node*, TetraContext& context);
  void notifyThreadSpecificVariable_E(tstring);
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
