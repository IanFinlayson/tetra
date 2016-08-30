#include <sstream>

#include "console.h"

tstring CommandConsole::receiveStandardInput() {
    char input[1024];
    std::cin >> input;

    return tstring(input);
}

void CommandConsole::processStandardOutput(const tstring& output) {
    std::cout << output;
}
