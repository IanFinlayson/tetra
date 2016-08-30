#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>

#include "tetra.h"

class CommandConsole : public VirtualConsole {
   public:
    Tstring receiveStandardInput();
    void processStandardOutput(const Tstring& output);
};

#endif
