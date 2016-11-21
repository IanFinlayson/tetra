/* error.cpp
* this file contains functions for error reporting */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "tetra.h"

/* this function takes a string and modifies it in place
 * with TOK_ missing and some other pretty printing */
void prettyPrintMessage(String& str) {
    str.replace("PLUS", "'+'");
    str.replace("MINUS", "'-'");
    str.replace("TIMES", "'*'");
    str.replace("DIVIDE", "'/'");
    str.replace("MODULUS", "'%'");
    str.replace("BITXOR", "'^'");
    str.replace("BITAND", "'&'");
    str.replace("BITOR", "'|'");
    str.replace("BITNOT", "'~'");
    str.replace("LEFTPARENS", "'('");
    str.replace("RIGHTPARENS", "')'");
    str.replace("COMMA", "','");
    str.replace("SEMICOLON", "';'");
    str.replace("COLON", "':'");
    str.replace("LSHIFT", "'<<'");
    str.replace("RSHIFT", "'>>'");
    str.replace("EXP", "'**'");
    str.replace("LTE", "'<='");
    str.replace("GTE", "'>='");
    str.replace("EQ", "'=='");
    str.replace("NEQ", "'!='");
    str.replace("LT", "'<'");
    str.replace("GT", "'>'");
    str.replace("MINUSEQ", "'-='");
    str.replace("ASSIGN", "'='");
    str.replace("PLUSEQ", "'+='");
    str.replace("TIMESEQ", "'*='");
    str.replace("DIVIDEEQ", "'/='");
    str.replace("MODULUSEQ", "'%='");
    str.replace("EXPEQ", "'**='");
    str.replace("RSHIFTEQ", "'>>='");
    str.replace("LSHIFTEQ", "'<<='");
    str.replace("ANDEQ", "'&='");
    str.replace("XOREQ", "'^='");
    str.replace("OREQ", "'|='");
    str.replace("TOK_", "");
}

/* Error exception functions */
Error::Error(const String& mesg, int lineNumber) {
    this->mesg = mesg;
    prettyPrintMessage(this->mesg);
    this->lineNumber = lineNumber;
}

String Error::getMessage() const {
    return mesg;
}

int Error::getLine() const {
    return lineNumber;
}

/* print an error */
std::ostream& operator<<(std::ostream& out, const Error& error) {
    out << "Error: ";
    if (error.getLine()) {
        out << "(line " << error.getLine() << ") ";
    }

    out << error.getMessage() << std::endl;
    return out;
}

/* the runtime error class */
RuntimeError::RuntimeError(const String& message, int line) : Error(message, line) {}

/* the system error class */
SystemError::SystemError(const String& message, int line, Node* nodeArg)
    : Error(message, line), node(nodeArg) {}

Node* SystemError::getNode() {
    return node;
}

InterruptError::InterruptError() : Error("Program Interrupted", -1) {}
