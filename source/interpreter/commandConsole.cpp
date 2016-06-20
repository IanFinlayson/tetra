#include <sstream>
#include "backend.h"

std::string CommandConsole::receiveStandardInput() {
  std::string input;

  std::cin >> input;

  return input;
}

void CommandConsole::processStandardOutput(const std::string& output) {
  std::cout << output;
}

