#include <sstream>

#include "console.h"

Tstring CommandConsole::receiveStandardInput() {
    char input[1024];
    std::cin >> input;

    return Tstring(input);
}

void CommandConsole::processStandardOutput(const Tstring& output) {
    std::cout << output;
}
