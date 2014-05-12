/* functions for dealing with node creation, management etc. */

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "tetra.hpp"
#include "parser.gen.hpp"

/* node member functions */
Node::Node(NodeType node_type) {
  this->node_type = node_type;
}
 
void Node::addChild(Node* child) {
  children.push_back(child);
}

void Node::setDataType(DataType data_type) {
  this->data_type = data_type;
}

void Node::setIdentifier(std::string identifier) {
  this->identifier = identifier;
}


