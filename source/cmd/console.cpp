/* console.cpp
 * this file defines the command line console for I/O */

#include <sstream>
#include "console.h"

String CommandConsole::receiveStandardInput() {
    char input[1024];
    std::cin >> input;

    return String(input);
}

void CommandConsole::processStandardOutput(const String& output) {
    std::cout << output;
}
