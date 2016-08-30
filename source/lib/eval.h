/* code for interpreting and evaluating programs */

#ifndef EVAL_H
#define EVAL_H

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

/* macro to squelch unused variable warnings */
#define UNUSED(x) (void)x;

/* interpret a tetra program parsed into a Node tree */
int interpret(Node* tree, int debug, int threads);

/* allows other libraries to define how the interpreter should handle I/O */
class VirtualConsole {
   public:
    /* read a string from the user and return it */
    virtual tstring receiveStandardInput() = 0;
    /* print a string for the user */
    virtual void processStandardOutput(const tstring&) = 0;
};

/* stores constants related to the environment where the program is running */
class TetraEnvironment {
   public:
    static void initialize();
    static VirtualConsole* getConsole();
    static void setConsole(VirtualConsole* console);

    static int getMaxThreads();
    static void setMaxThreads(int);
    static void setDebug(bool);
    static bool isDebugMode();
    static void halt();
    static bool isRunning();
    static void setRunning();

   private:
    static bool running;
    static int maxThreads;
    static VirtualConsole* console;
    static bool debugMode;
};

#endif
