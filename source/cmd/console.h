/* console.h
 * this file declares the console class for performing console I/O */

#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>

#include "tetra.h"

class CommandConsole : public VirtualConsole {
   public:
    String receiveStandardInput();
    void processStandardOutput(const String& output);
};

#endif
