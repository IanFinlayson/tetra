/* functions for dealing with node creation, management etc. */

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include "frontend.hpp"
#include "parser.gen.hpp"

extern int yylineno;


/* node member functions */
Node::Node(NodeKind node_type) {
  this->node_type = node_type;
  this->data_type = NULL;
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

void Node::setDataType(DataType* data_type) {
  this->data_type = data_type;
}

void Node::setStringval(const string& stringval) {
  this->stringval = stringval;
}

void Node::setIntval(int intval) {
  this->intval = intval;
}

void Node::setBoolval(bool boolval) {
  this->boolval = boolval;
}

void Node::setRealval(double realval) {
  this->realval = realval;
}

void Node::setLine(int lineno) {
  this->lineno = lineno;
}

int Node::getLine( ) const {
  return lineno;
}
string Node::getString( ) const {
  return stringval;
}
int Node::getInt( ) const {
  return intval;
}
double Node::getReal( ) const {
  return realval;
}
bool Node::getBool( ) const {
  return boolval;
}
NodeKind Node::kind( ) const {
  return node_type;
}
DataType* Node::type( ) const {
  return data_type;
}
void Node::setType(DataType* t) {
  data_type = t;
}
int Node::numChildren( ) const {
  return children.size( );
}
Node* Node::child(int which) const {
  if (which < 0 || which >= (int)children.size( )) {
    return NULL;
  } else {
    return children[which];
  }
}
/* insert a symbol into the symtable */
void Node::insertSymbol(Symbol sym) {
  /* create symtable if needed */
  if (!symtable) {
    symtable = new map<string, Symbol>( );
  }

  /* check if it's there first */
  if (symtable->count(sym.getName( )) > 0) {
    throw Error("'" + sym.getName( ) + "' has already been declared", sym.getLine( ));
  }

  /* add it in */
  symtable->insert(pair<string, Symbol>(sym.getName( ), sym));
}

/* lookup a symbol from a symbol table */
Symbol Node::lookupSymbol(string name, int lineno) {
  /* if there is no symbol table, it's not found! */
  if (!symtable) {
    throw Error("Symbol '" + name + "' not found!", lineno);
  }

  map<string, Symbol>::iterator it = symtable->find(name);

  if (it == symtable->end( )) {
    throw Error("Symbol '" + name + "' not found!", lineno);
  }

  /* return the record */
  return it->second;
}

bool Node::hasSymbol(const string& name) {
  if (!symtable) {
    return false;
  }
  return (symtable->count(name) > 0);
}

/* these are also here */
Symbol::Symbol(string name, DataType* type, int lineno) {
  this->name = name;
  this->type = type;
  this->lineno = lineno;
}

int Symbol::getLine( ) const {
  return lineno;
}
string Symbol::getName( ) const {
  return name;
}
DataType* Symbol::getType( ) const {
  return type;
}

Symbol::Symbol( ) {
  name = "";
  type = NULL;
  lineno = 0;
}

