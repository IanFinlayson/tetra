/* functions for dealing with node creation, management etc. */

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "tetra.hpp"
#include "parser.gen.hpp"

string typeToString(DataType t) {
  switch (t) {
    case TYPE_INT: return "int";
    case TYPE_REAL: return "real";
    case TYPE_STRING: return "string";
    case TYPE_BOOL: return "bool";
    case TYPE_VOID: return "void";
    default: fail("typeToString: Unknown data type");
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
}
 
void Node::addChild(Node* child) {
  children.push_back(child);
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
