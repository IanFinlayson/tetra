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

    static long nextThreadID;
};

#endif
