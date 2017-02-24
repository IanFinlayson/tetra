/* environment.h
 * this class keeps track of the running environment of programs being
 * interpreted */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "eval.h"

/* stores constants related to the environment where the program is running */
class Environment {
   public:
    static void initialize();
    static VirtualConsole* getConsole();
    static void setConsole(VirtualConsole* console);

    static unsigned int getMaxThreads();
    static void setMaxThreads(int);
    static void setDebug(bool);
    static void setEcho();
    static bool isEcho();
    static bool isDebugMode();
    static void halt();
    static bool isRunning();
    static void setRunning();

   private:
    static bool running;
    static bool inputEcho;
    static unsigned int maxThreads;
    static VirtualConsole* console;
    static bool debugMode;
};

#endif
