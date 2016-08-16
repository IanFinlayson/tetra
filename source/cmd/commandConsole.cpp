#include <sstream>

#include "commandConsole.h"

tstring CommandConsole::receiveStandardInput() {
  tstring input;
  std::cin >> input;

  return input;
}

void CommandConsole::processStandardOutput(const tstring& output) {
  std::cout << output;
}

