#include"backend.hpp"
#include"commandConsole.hpp"
#include<sstream>

std::string CommandConsole::receiveStandardInput() const {
  std::string input;

  std::cin >> input;

  return input;
}

void CommandConsole::processStandardOutput(const std::string output) const {
  std::cout << output;
}
