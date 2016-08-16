/* functions for error reporting */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "tetra.h"

/* this function search and replaces a string in place */
void replace(std::string& str, const std::string& from, const std::string& to) {
  size_t start_pos = str.find(from);
  if (start_pos != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    replace(str, from, to);
  }
}

/* this function takes a string and modifies it in place
 * with TOK_ missing and some other pretty printing */
void prettyPrintMessage(std::string& str) {
  replace(str, "PLUS", "'+'");
  replace(str, "MINUS", "'-'");
  replace(str, "TIMES", "'*'");
  replace(str, "DIVIDE", "'/'");
  replace(str, "MODULUS", "'%'");
  replace(str, "BITXOR", "'^'");
  replace(str, "BITAND", "'&'");
  replace(str, "BITOR", "'|'");
  replace(str, "BITNOT", "'~'");
  replace(str, "LEFTPARENS", "'('");
  replace(str, "RIGHTPARENS", "')'");
  replace(str, "COMMA", "','");
  replace(str, "SEMICOLON", "';'");
  replace(str, "COLON", "':'");
  replace(str, "LSHIFT", "'<<'");
  replace(str, "RSHIFT", "'>>'");
  replace(str, "EXP", "'**'");
  replace(str, "LTE", "'<='");
  replace(str, "GTE", "'>='");
  replace(str, "EQ", "'=='");
  replace(str, "NEQ", "'!='");
  replace(str, "LT", "'<'");
  replace(str, "GT", "'>'");
  replace(str, "MINUSEQ", "'-='");
  replace(str, "PLUSEQ", "'+='");
  replace(str, "TIMESEQ", "'*='");
  replace(str, "DIVIDEEQ", "'/='");
  replace(str, "MODULUSEQ", "'%='");
  replace(str, "EXPEQ", "'**='");
  replace(str, "RSHIFTEQ", "'>>='");
  replace(str, "LSHIFTEQ", "'<<='");
  replace(str, "ANDEQ", "'&='");
  replace(str, "XOREQ", "'^='");
  replace(str, "OREQ", "'|='");
  replace(str, "TOK_", "");
}

/* Error exception functions */
Error::Error(const std::string& mesg, int lineno) {
  this->mesg = mesg;
  prettyPrintMessage(this->mesg);
  this->lineno = lineno;
}

std::string Error::getMessage() const { return mesg; }

int Error::getLine() const { return lineno; }

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
RuntimeError::RuntimeError(const std::string& pMessage, int pLine) : Error(pMessage, pLine) {
}

/* the system error class */
SystemError::SystemError(const std::string& pMessage, int pLine, const Node* pNode) : Error(pMessage, pLine), node(pNode) {
}

const Node* SystemError::getNode() {
    return node;
}

InterruptError::InterruptError() : Error("Program Interrupted", -1) {
}

