/* this program takes in Tetra code and produces a parse tree which allows it
 * to be tested and visualized.  It now produces a structured text dump.
 * TODO add graphical output with graphiz or similar */

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "tetra.hpp"
#include "parser.gen.hpp"

extern Node* root;
using namespace std;

void print_tree(Node* node, int level = 0) {
  /* base case */
  if (!node) {
    return;
  }

  /* print leading spaces */
  for (int i = 0; i < level; i++) {
    cout << "    ";
  }

  /* print this type of node */
  switch (node->node_type) {
    case NODE_FUNCTION: cout << "FUNCTION"; break;
    case NODE_FUNCTION_LIST: cout << "FUNCTION LIST"; break;
    case NODE_STATEMENT: cout << "STATEMENT"; break;
    case NODE_PARAM: cout << "PARAM"; break;
    case NODE_PARAM_LIST: cout << "PARAM LIST"; break;
    case NODE_TYPE: cout << "TYPE"; break;
    case NODE_PASS: cout << "PASS"; break;
    case NODE_RETURN: cout << "RETURN"; break;
    case NODE_BREAK: cout << "BREAK"; break;
    case NODE_CONTINUE: cout << "CONTINUE"; break;
    case NODE_IF: cout << "IF"; break;
    case NODE_WHILE: cout << "WHILE"; break;
    case NODE_INTVAL: cout << "INTVAL"; break;
    default: fail("Unsupported node type!"); break;
  }
  if (node->identifier != "") {
    cout << " (ID:" << node->identifier << ")";
  }
  if (node->data_type != TYPE_VOID) {
    cout << " (TYPE:" << node->data_type << ")";
  }

  cout << endl;

  /* print children */
  for (unsigned int i = 0; i < node->children.size( ); i++) {
    print_tree(node->children[i], level + 1);
  }
}


/* the main function */
int main(int argc, char** argv) {
  /* if a file was passed, use that over stdin */
  if (argc >= 2) {
    stdin = fopen(argv[1], "r");
    if (!stdin) {
      fail(string("Could not open file ") + argv[1] + string(" for reading."));
    }
  }

  /* parse it */
  yyparse( );

  /* dump the tree to the screen */
  print_tree(root);

  return 0;
}


