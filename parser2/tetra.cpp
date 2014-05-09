#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "parser.gen.hpp"
#include "tetra.hpp"

/* print an error message and quit */
void fail(const std::string& mesg, int lineno) {
  std::cerr << "Tetra error: " << mesg;
  if (lineno) {
    std::cerr << " (Line " << lineno << ")";
  }
  std::cerr << std::endl;
}

/* node functions */
Node* make_node(int type) {
  Node* n = new Node;
  n->type = type;
  return n;
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

  /* parse it */
  yyparse( );

  return 0;
}


