#include "ideCommandObserver.h"
#include <sstream>
#include "backend.h"
#include "frontend.h"

// Global symbol table
extern std::map<std::string, Symbol, less<std::string>, gc_allocator<pair<std::string, Symbol> > > globals;

// as a courtesy, we will provide this method
/*std::string statusToString(ThreadStatus status) {
        switch(status) {
        case RUNNING: return "Running";
        case STOPPED: return "Stopped";
        case DESTROYED: return "Destroyed";
        case WAITING: return "Waiting";
        default: return "";
        }
}
*/
IDECommandObserver::IDECommandObserver() {
  // lastLine.lineNo = 0;
  // stepping = false;
  // stopAtNext = false;

  // int success = pthread_mutex_init(&prompt_mutex,NULL);
  // TODO revise so asserts are no longer used here
  // assert(success==0);

  int success = pthread_mutex_init(&threadList_mutex, NULL);
  assert(success == 0);

  success = pthread_mutex_init(&breakList_mutex, NULL);
  assert(success == 0);

  success = pthread_cond_init(&prompt_condition, NULL);
  assert(success == 0);

  success = pthread_mutex_init(&context_mutex, NULL);
  assert(success == 0);

  allowedThread = -1;  //-1 means any thread may trip the debugger
  yieldEnabled = true;
}

// TODO Since these options were added over time, there are several common
// operations, suh as checking if a breakpoint exists, that should probably put
// into methods

// Notifies the observer that a certain node in the program has been reached
void IDECommandObserver::notify_E(const Node* foundNode,
                                  TetraContext& context) {
  // Check to see if we entered a new scope
  if (foundNode->kind() == NODE_FUNCTION) {
    // cout << "Entering scope: " << foundNode->getString() << endl;
    context.getScopes().push(foundNode);
  }

  int currentLine = foundNode->getLine();

  Breakpoint linecomp;
  linecomp.lineNo = currentLine;
  linecomp.threadLabel = context.getThreadID();
  // check if line is a breakpoint
  // If the current line is the same as the last line we broke at, we should not
  // break, and let the instruction finish
  // This is unless we are stepping
  bool willBreak = false;
  pthread_mutex_lock(&breakList_mutex);
  {
    // std::cout << ">>>>>>" << foundNode->getLine() << std::endl;
    willBreak = (std::find(breakpoints.begin(), breakpoints.end(), linecomp) !=
                     breakpoints.end() &&
                 currentLine != context.getLastLineNo()) ||
                (currentLine != context.getLastLineNo() &&
                 context.getStopAtNext() == true) ||
                context.getStepping() == true;
  }
  pthread_mutex_unlock(&breakList_mutex);

  // std::cout << "WILLBREAK: " << (currentLine != context.getLastLineNo())  <<
  // std::endl;

  // Regardless of whether we break or not, we chould note that we have moved to
  // a new line number
  context.setLastLineNo(currentLine);

  // This variable keeps track of whether a hread will be allowed to continue on
  // or not
  // When set to true, will break out of the debugging loop. The thread should
  // stay in this loop, however, until it is explicitely told to continue
  context.setResume(false);
  while (willBreak && context.getResume() == false) {
    // These variables are only valid for 1 instruction, so reset them
    context.setStepping(false);
    context.setStopAtNext(false);
    // Denotes whether or not the thread should reset the wait queue for
    // entering into the interactive part of the debugger
    bool willBroadcast = true;

    // Register that this thread has stopped
    context.setRunStatus(STOPPED);

    VirtualConsole& console =
        TetraEnvironment::getConsole(context.getThreadID(), true);

    pthread_mutex_lock(&threadList_mutex);
    {
      if (std::find(waitingThreads.begin(), waitingThreads.end(),
                    context.getThreadID()) == waitingThreads.end()) {
        std::stringstream msg;

        msg << "\nThread " << context.getThreadID() << ":" << endl;
        msg << "Breakpoint reached at line: " << currentLine << endl;
        msg << "Stopped at node of kind: " << foundNode->kind() << endl;
        console.processStandardOutput(msg.str());
        waitingThreads.push_back(context.getThreadID());
      }
    }
    pthread_mutex_unlock(&threadList_mutex);

    // pthread_mutex_lock(&prompt_mutex);
    TetraEnvironment::getConsoleArray().obtainConsoleMutex(
        context.getThreadID(), false);

    if (allowedThread == -1 || allowedThread == context.getThreadID()) {
      std::string ret = " ";

      // Now that the thread will have some console time, it can resume
      // executing once it is done here
      context.setResume(true);

      // B default, a different thread will be allowed to access the debugger
      // once this thread is done
      // If this thread needs to keep its affinity for this console (such as if
      // stepping), the it must re-set allowedThread
      allowedThread = -1;

      while (ret == " ") {
        /*std::stringstream infoStr;
        pthread_mutex_lock(&threadList_mutex);//As a courtesy, insure that the
        directions are not cut off by an incoming thread
        {
                infoStr << "Thread: " << context.getThreadID();
                console.processStandardOutput(infoStr.str());
                console.processStandardOutput("\nOptions: (s)tep, (n)ext
        (c)ontinue (b)reak (r)emove (p)rint\nFor more options, type (h)elp\n");
        }
        pthread_mutex_unlock(&threadList_mutex);
        */
        ret = console.receiveStandardInput();

        switch (ret[0]) {
          case 'a':  // repeat the prompt
          case 'A': {
            std::stringstream msg;

            msg << "\nThread " << context.getThreadID() << ":" << endl;
            msg << "Breakpoint reached at line: " << currentLine << endl;
            msg << "Stopped at node of kind: " << foundNode->kind() << endl;
            console.processStandardOutput(msg.str());
            ret = " ";
          } break;
          case 'c':  // continue
          case 'C':
            continue_E();
            break;
          case 'n':  // next
          case 'N':
            next_E(context);
            // context.setStopAtNext(true);
            break;
          case 's':  // step
          case 'S':
            step_E(context);
            // context.setStepping(true);
            break;
          case 'i':  // Interrupt thread
          case 'I': {
            std::string arg = " ";
            arg = console.receiveStandardInput();
            int threadNum = atoi(arg.c_str());

            if (threadNum == 0 && arg != "0") {
              console.processStandardOutput(
                  "The second argument could not be parsed into a Thread ID.");
            } else {
              pthread_mutex_lock(&context_mutex);
              {
                bool threadFound = false;
                for (std::list<TetraContext*, gc_allocator<TetraContext*> >::iterator iter =
                         threadContexts.begin();
                     iter != threadContexts.end(); ++iter) {
                  if ((*iter)->getThreadID() == threadNum) {
                    threadFound = true;
                    if ((*iter)->getRunStatus() == RUNNING) {
                      (*iter)->setStepping(true);
                    } else {
                      console.processStandardOutput(
                          "The requested thread is not currently running");
                    }
                  }
                }
                if (!threadFound) {
                  console.processStandardOutput(
                      "Could not find the requested thread.\nUse (l)ist "
                      "(r)unning to show running threads");
                }
              }
              pthread_mutex_unlock(&context_mutex);
            }

          } break;
          case 'l':
          case 'L': {
            std::string arg = " ";
            // When a case is encountered, filter value may be adjusted to
            // determine what values should be included/excluded
            int filter = 1;
            arg = console.receiveStandardInput();
            switch (arg[0]) {
              case 'c':  // call stack
              case 'C':
                console.processStandardOutput("-----\n");
                context.printStackTrace();
                console.processStandardOutput("-----\n");
                break;
              case 'g':  // global variables
              case 'G':  // filter will be 0
                filter = 0;
              // check that the global variable table has been initialized
              /*if(context.getGlobRefTable() == NULL) {
                      console.processStandardOutput("There are no global
              variables\n");
                      break;
              }*/
              case 'v':  // Local Variables
              case 'V':  // filter will be 1
              {
                // get the function node which has the local scope's symbol
                // table
                const Node* nodey = context.getScopes().top();
                std::stringstream output;
                output << "-----\n";
                for (std::map<std::string, int, less<std::string>, gc_allocator<pair<std::string, int> > >::const_iterator varIterator =
                         ((filter == 0) ? context.getGlobRefTable()
                                        : context.getRefTable())
                             .begin();
                     varIterator !=
                     ((filter == 0) ? context.getGlobRefTable()
                                    : context.getRefTable())
                         .end();
                     ++varIterator) {
                  // Determine the datatype of the entry by looking in the local
                  // or global symbol table
                  Symbol symbolEntry;
                  if (filter == 1) {
                    symbolEntry = nodey->lookupSymbol(varIterator->first, 0);
                  } else {  // Variable exists in global scope
                    symbolEntry = globals[varIterator->first];
                  }
                  output << varIterator->first << " ("
                         << typeToString(symbolEntry.getType()) << ")\n";
                }
                output << "-----\n";
                console.processStandardOutput(output.str());
              } break;
              case 's':  // stopped threads
              case 'S':  // filter will be 3
                filter++;
              case 'r':  // running threads
              case 'R':  // filter will be 2
                filter++;
              case 't':  // all threads
              case 'T':  // filter will be 1
              {
                std::stringstream output;
                output << "-----\n";
                pthread_mutex_lock(&context_mutex);
                {
                  for (std::list<TetraContext*, gc_allocator<TetraContext*> >::iterator contextIter =
                           threadContexts.begin();
                       contextIter != threadContexts.end(); ++contextIter) {
                    TetraContext* loopContext = *contextIter;
                    if (filter == 1 ||
                        (filter == 2 &&
                         loopContext->getRunStatus() == RUNNING) ||
                        (filter == 3 &&
                         loopContext->getRunStatus() == STOPPED)) {
                      output << "Thread " << loopContext->getThreadID() << ": "
                             << statusToString(loopContext->getRunStatus())
                             << "\n";
                    }
                  }
                }
                pthread_mutex_unlock(&context_mutex);
                output << "-----\n";
                console.processStandardOutput(output.str());
              } break;
              default: {
                std::stringstream errMessage;
                errMessage << "Unrecognized list option: " << arg
                           << ".\nAvailable list options are:\n(c)allstack "
                              "(v)ariables (g)lobals (t)hreads (r)unning "
                              "(s)topped.\n";
                console.processStandardOutput(errMessage.str());
                arg = " ";
              }
            }
            ret = " ";
          } break;
          case 'b':  // break
          case 'B': {
            std::string breakP = console.receiveStandardInput();
            int lineNo = 0;

            lineNo = atoi(breakP.c_str());
            if (lineNo == 0) {
              console.processStandardOutput(
                  "Could not place breakpoint, unable to parse line number\n");

            } else {
              bool success = false;
              // Note that this method is threadsafe
              success = break_E(lineNo /*,context.getThreadID()*/);

              std::stringstream confirmationMessage;
              if (success) {
                confirmationMessage << "Breakpoint set at line: " << lineNo
                                    << "\n\n";
              } else {
                confirmationMessage
                    << "Breakpoint already exists at line: " << lineNo
                    << "\n\n";
              }
              console.processStandardOutput(confirmationMessage.str());
            }
            // set ret to repeat the input prompt
            ret = " ";
          } break;
          case 'r':  // remove (breakpoint)
          case 'R': {
            std::string breakP = console.receiveStandardInput();
            int lineNo = atoi(breakP.c_str());
            if (lineNo == 0) {
              console.processStandardOutput(
                  "Could not remove a breakpoint, unable to parse line "
                  "number\n");
            } else {
              bool success = false;
              // Note that this method is threadsafe
              success = remove_E(lineNo);

              pthread_mutex_unlock(&breakList_mutex);
              std::stringstream confirmationMessage;
              if (success) {
                confirmationMessage
                    << "Breakpoint removed from line: " << lineNo << "\n\n";
              } else {
                confirmationMessage << "No breakpoint exists at line " << lineNo
                                    << "\n\n";
              }
              console.processStandardOutput(confirmationMessage.str());
            }
            // set ret to repeat the input prompt
            ret = " ";
          } break;
          case 'p':  // print
          case 'P': {
            std::string varName = console.receiveStandardInput();
            void* var = context.fetchVariable(varName);
            if (var != NULL) {
              std::stringstream message;
              message << varName;
              // get the function node which has the local scope's symbol table
              const Node* nodey = context.getScopes().top();
              // Determine the datatype of the entry by looking in the local or
              // global symbol table
              Symbol symbolEntry;
              if (const_cast<Node*>(nodey)->hasSymbol(varName)) {
                symbolEntry = nodey->lookupSymbol(varName, 0);
              } else {  // Variable exists in global scope
                symbolEntry = globals[varName];
              }
              message << " (" << typeToString(symbolEntry.getType()) << "): ";
              switch (symbolEntry.getType()->getKind()) {
                case TYPE_INT:
                  // message << " (int): ";
                  message << *static_cast<int*>(var);
                  break;
                case TYPE_REAL:
                  // message << " (real): ";
                  message << *static_cast<double*>(var);
                  break;
                case TYPE_STRING:
                  // message << " (string): ";
                  message << *static_cast<std::string*>(var);
                  break;
                case TYPE_BOOL:
                  // message << " (bool): ";
                  message << *static_cast<bool*>(var);
                  break;
                case TYPE_MUTEX:
                  //TODO
                  break;
                case TYPE_TASK:
                  //TODO
                  break;
                case TYPE_VECTOR:
                  // message << " (vector): ";
                  message << *static_cast<TArray*>(var);
                  break;
                case TYPE_TUPLE:
                  //TODO
                  break;
                case TYPE_DICT:
                  //TODO
                  break;
                case TYPE_FUNCTION:
                  //TODO
                  break;
                case TYPE_OVERLOAD:
                  //TODO:this should never happen
                  break;
                case TYPE_CLASS:
                  //TODO
                  break;
                case TYPE_NONE:
                  // message << " (void): ";
                  message << var;
                  break;
              }
              message << "\n\n";
              console.processStandardOutput(message.str());
            } else {
              console.processStandardOutput("The variable " + varName +
                                            " either does not exist in the "
                                            "current scope, or has not been "
                                            "initialized\n\n");
            }
            ret = " ";
          } break;
          default:
            console.processStandardOutput("Error, unrecognized command\n\n");
            // set ret to repeat the prompt for input
            ret = " ";
        }
        // cout << "breakpoints size: " << breakpoints.size() << endl;
      }
      // Wakes up all threads waiting on the debugger
      if (willBroadcast) {
        // std::cout << "Broadcast" << std::endl;
        // pthread_cond_broadcast(&prompt_condition);
        TetraEnvironment::getConsoleArray().broadcastCondition(
            context.getThreadID(), true);
      }
    } else {  // If the thread was not expected, it will block here, allowing
              // other threads to get through.
      // When the observer state changes, a broadcast will allow all these
      // threads to return to the waiting area
      // std::cout << "HERE!" << std::endl;
      // pthread_cond_wait(&prompt_condition, &prompt_mutex);
      TetraEnvironment::getConsoleArray().waitOnCondition(context.getThreadID(),
                                                          true);
    }
    // pthread_mutex_unlock(&prompt_mutex);
    TetraEnvironment::getConsoleArray().releaseConsoleMutex(
        context.getThreadID(), true);

    // If the thread is resuming, remove it from the list of waiting threads
    if (context.getResume() == true) {
      pthread_mutex_lock(&threadList_mutex);
      {
        std::remove(waitingThreads.begin(), waitingThreads.end(),
                    context.getThreadID());
        // Needed to actually reduce the size of the container
        // There should be exactly 1 instance of this thread's ID in the list
        waitingThreads.pop_back();
      }
      pthread_mutex_unlock(&threadList_mutex);
    }
  }
  // Register that the thread is running again
  context.setRunStatus(RUNNING);
}

// Note that just because this is the nth time the function has been called, it
// might not be to register thread N!
// i.e. might call this function with threadNum=3 after calling it with
// threadNum = 4, but before threadNum = 2
void IDECommandObserver::threadCreated_E(int threadNum, TetraContext& context) {
  /*pthread_mutex_lock(&context_mutex);
  std::stringstream threadMessage;
  threadMessage << "Thread launched: " << threadNum << "\n";

  TetraEnvironment::getConsole(context.getThreadID(),true).processStandardOutput(threadMessage.str());

  TetraContext* threadData = &context;

  threadContexts.push_back(threadData);

  pthread_mutex_unlock(&context_mutex);*/
  CommandObserver::threadCreated_E(threadNum, context);
}

void IDECommandObserver::threadDestroyed_E(int threadNum) {
  /*pthread_mutex_lock(&context_mutex);

  for(std::list<TetraContext*>::iterator candidate = threadContexts.begin();
  candidate != threadContexts.end(); ++candidate) {
          if((*candidate)->getThreadID() == threadNum) {
                  //(*candidate)->setRunStatus(DESTROYED);
                  //Erase the element from the array
                  std::remove(threadContexts.begin(),threadContexts.end(),(*candidate));
                  threadContexts.pop_back();
                  //break necessary, since we may have invalidated the iterator
                  break;
          }

  }

  pthread_mutex_unlock(&context_mutex);*/
  CommandObserver::threadDestroyed_E(threadNum);
}

void IDECommandObserver::step_E(TetraContext& context) {
  /*context.setStepping(true);
  allowedThread = context.getThreadID();*/
  CommandObserver::step_E(context);
}

void IDECommandObserver::next_E(TetraContext& context) {
  /*context.setStopAtNext(true);
  allowedThread = context.getThreadID();*/
  CommandObserver::next_E(context);
}

// Adds a breakpoint associated with the given line number. Returns false if
// there was already a breakpoint there.
bool IDECommandObserver::break_E(int lineNum) {
  /*Breakpoint toPush;
  toPush.lineNo = lineNum;
  toPush.threadLabel = -1;
  bool ret = true;

  pthread_mutex_lock(&breakList_mutex);
  {
          if(std::find(breakpoints.begin(), breakpoints.end(), toPush) ==
  breakpoints.end()) {
                  breakpoints.push_back(toPush);
          }
          else {
                  ret = false;
          }
  }
  pthread_mutex_unlock(&breakList_mutex);

  return ret;*/
  return CommandObserver::break_E(lineNum);
}

// Removes a breakpoint from the given line number. Returns false if no
// breakpoint exists
bool IDECommandObserver::remove_E(int lineNum) {
  /*bool ret = false;
  Breakpoint toPop;
  toPop.lineNo = lineNum;
  toPop.threadLabel = -1;
  std::vector<Breakpoint>::iterator location = std::find(breakpoints.begin(),
  breakpoints.end(), toPop);

  pthread_mutex_lock(&breakList_mutex);
  {
          while(location != breakpoints.end()) {
                  breakpoints.erase(location);
                  ret = true;
                  location = std::find(breakpoints.begin(), breakpoints.end(),
  toPop);
          }
  }
  pthread_mutex_unlock(&breakList_mutex);
  return ret;*/
  return CommandObserver::remove_E(lineNum);
}

void IDECommandObserver::continue_E() {
  // do nothing at the moment
  CommandObserver::continue_E();
}

void IDECommandObserver::leftScope_E(TetraContext& context) {
  /*context.getScopes().pop();*/
  CommandObserver::leftScope_E(context);
}

void IDECommandObserver::notifyThreadSpecificVariable_E(std::string varName) {
  CommandObserver::notifyThreadSpecificVariable_E(varName);
}

void IDECommandObserver::setYieldEnabled(bool enable) {
  /*yieldEnabled = enable;*/
  CommandObserver::setYieldEnabled(enable);
}

std::vector<int, gc_allocator<int> > IDECommandObserver::getThreadLocations() {
  /*std::vector<int> ret;
  pthread_mutex_lock(&context_mutex);
  {
          for(std::list<TetraContext*>::iterator context =
  threadContexts.begin(); context != threadContexts.end();context++) {
                  if((*context)->getRunStatus() == STOPPED) {
                          ret.push_back((*context)->getLastLineNo());
                  }
          }
  }
  pthread_mutex_unlock(&context_mutex);
  return ret;
  */
  return CommandObserver::getThreadLocations();
}
