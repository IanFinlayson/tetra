#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "tetra.hpp"
#include "parser.gen.hpp"

int yylex( );

/* print an error message and quit */
void fail(const std::string& mesg, int lineno) {
  std::cerr << "Tetra error: " << mesg;
  if (lineno) {
    std::cerr << " (Line " << lineno << ")";
  }
  std::cerr << std::endl;
}

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

/* the main function */
int main(int argc, char** argv) {
  /* if a file was passed, use that over stdin */
  if (argc >= 2) {
    stdin = fopen(argv[1], "r");
    if (!stdin) {
      fail(std::string("Could not open file ") + argv[1] + std::string(" for reading."));
    }
  }

  int tok;
  do {
    std::cout << (tok = yylex( )) << std::endl;
  } while (tok);

  /* parse it */
  //yyparse( );

  return 0;
}


