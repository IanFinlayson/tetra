#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "tetra.hpp"
#include "parser.gen.hpp"

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


