#include <sstream>
#include "backend.h"

tstring CommandConsole::receiveStandardInput() {
  char input [1024];
  std::cin >> input;

  return input;
}

void CommandConsole::processStandardOutput(const tstring& output) {
  std::cout << output;
}

