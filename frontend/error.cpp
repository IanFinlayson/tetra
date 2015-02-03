/* functions for error reporting */

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include "frontend.hpp"
#include "parser.gen.hpp"

/* this function search and replaces a string in place */
void replace(string& str, const string& from, const string& to) {
  size_t start_pos = str.find(from);
  if(start_pos != string::npos) {
    str.replace(start_pos, from.length( ), to);
    replace(str, from, to);
  }
}

/* this function takes a string and modifies it in place
 * with TOK_ missing and some other pretty printing */
void prettyPrintMessage(string& str) {
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
Error::Error(const string& mesg, int lineno) {
  this->mesg = mesg;
  prettyPrintMessage(this->mesg);
  this->lineno = lineno;
}

string Error::getMessage( ) const {
  return mesg;
}

int Error::getLine( ) const {
  return lineno;
}

/* print an error */
ostream& operator<<(ostream& out, const Error& error) {
  out << "Error: ";
  if (error.getLine( )) {
    out << "(line " << error.getLine( ) << ") ";
  }

  out << error.getMessage( ) << endl;
  return out;
}

