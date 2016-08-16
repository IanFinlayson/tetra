#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>

#include "tetra.h"


class CommandConsole : public VirtualConsole {
 public:
  tstring receiveStandardInput();

  void processStandardOutput(const tstring& output);
};

#endif

