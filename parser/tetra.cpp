/* functions for dealing with node creation, management etc. */

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "tetra.hpp"
#include "parser.gen.hpp"

extern int yylineno;

string typeToString(DataType t) {
  switch (t) {
    case TYPE_INT: return "int";
    case TYPE_REAL: return "real";
    case TYPE_STRING: return "string";
    case TYPE_BOOL: return "bool";
    case TYPE_VOID: return "void";
    default: throw Error("typeToString: Unknown data type");
  }
}

/* node member functions */
Node::Node(NodeType node_type) {
  this->node_type = node_type;
  data_type = TYPE_VOID;
  stringval = "";
  intval = 0;
  realval = 0.0;
  boolval = false;
  lineno = yylineno;  /* this is often inaccurate! */
}
 
void Node::addChild(Node* child) {
  if (child) {
    children.push_back(child);
  }
}

void Node::setDataType(DataType data_type) {
  this->data_type = data_type;
}

void Node::setStringval(TetraString stringval) {
  this->stringval = stringval;
}

void Node::setIntval(TetraInt intval) {
  this->intval = intval;
}

void Node::setBoolval(TetraBool boolval) {
  this->boolval = boolval;
}

void Node::setRealval(TetraReal realval) {
  this->realval = realval;
}

void Node::setLine(int lineno) {
  this->lineno = lineno;
}

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



