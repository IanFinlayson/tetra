#include <sstream>
#include "backend.h"

std::string CommandConsole::receiveStandardInput() const {
  std::string input;

  std::cin >> input;

  return input;
}

void CommandConsole::processStandardOutput(const std::string output) const {
  std::cout << output;
}
