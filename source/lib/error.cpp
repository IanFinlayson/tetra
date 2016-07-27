/* functions for error reporting */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include "backend.h"
#include "frontend.h"
#include "parser.h"

/* this function search and replaces a string in place */
void replace(tstring& str, const tstring& from, const tstring& to) {
  size_t start_pos = str.find(from);
  if (start_pos != tstring::npos) {
    str.replace(start_pos, from.length(), to);
    replace(str, from, to);
  }
}

/* this function takes a string and modifies it in place
 * with TOK_ missing and some other pretty printing */
void prettyPrintMessage(tstring& str) {
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
Error::Error(const tstring& mesg, int lineno) {
  this->mesg = mesg;
  prettyPrintMessage(this->mesg);
  this->lineno = lineno;
}

tstring Error::getMessage() const { return mesg; }

int Error::getLine() const { return lineno; }

/* print an error */
ostream& operator<<(ostream& out, const Error& error) {
  out << "Error: ";
  if (error.getLine()) {
    out << "(line " << error.getLine() << ") ";
  }

  out << error.getMessage() << endl;
  return out;
}

/* the runtime error class */
RuntimeError::RuntimeError(const tstring& pMessage, int pLine,
                           TetraContext& pContext)
    : Error(pMessage, pLine), context(pContext) {
  // nothing else
}

TetraContext& RuntimeError::getContext() { return context; }

/* the system error class */
SystemError::SystemError(const tstring& pMessage, int pLine,
                         const Node* pNode)
    : Error(pMessage, pLine), node(pNode) {
  // nothing else
}

const Node* SystemError::getNode() { return node; }


InterruptError::InterruptError() : Error("Program Interrupted", -1) {}

