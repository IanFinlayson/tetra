/* this program takes in Tetra code and produces a parse tree which allows it
 * to be tested and visualized.  It now produces a structured text dump.
 * TODO add graphical output with graphiz or similar */

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "tetra.hpp"
#include "parser.gen.hpp"

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
    /* statements and groups */
    case NODE_FUNCTION: cout << "FUNCTION: " << node->stringval; break;
    case NODE_FUNCTION_LIST: cout << "FUNCTION LIST"; break;
    case NODE_STATEMENT: cout << "STATEMENT"; break;
    case NODE_FORMAL_PARAM: cout << "FORMAL PARAM: " << node->stringval; break;
    case NODE_FORMAL_PARAM_LIST: cout << "FORMAL PARAM LIST"; break;
    case NODE_PASS: cout << "PASS"; break;
    case NODE_RETURN: cout << "RETURN"; break;
    case NODE_BREAK: cout << "BREAK"; break;
    case NODE_CONTINUE: cout << "CONTINUE"; break;
    case NODE_IF: cout << "IF"; break;
    case NODE_WHILE: cout << "WHILE"; break;
    case NODE_FUNCALL: cout << "FUNCALL: " << node->stringval; break;
    case NODE_ACTUAL_PARAM_LIST: cout << "ACTUAL PARAM LIST"; break;

    /* operators */
    case NODE_OR: cout << "OR"; break;
    case NODE_AND: cout << "AND"; break;
    case NODE_LT: cout << "LESS THAN"; break;
    case NODE_LTE: cout << "LESS THAN EQUAL TO"; break;
    case NODE_GT: cout << "GREATER THAN"; break;
    case NODE_GTE: cout << "GREATER THAN EQUAL TO"; break;
    case NODE_EQ: cout << "EQUAL TO"; break;
    case NODE_NEQ: cout << "NOT EQUAL TO"; break;
    case NODE_NOT: cout << "NOT"; break;
    case NODE_BITXOR: cout << "XOR"; break;
    case NODE_BITAND: cout << "BITAND"; break;
    case NODE_BITOR: cout << "BITOR"; break;
    case NODE_BITNOT: cout << "BITNOT"; break;
    case NODE_SHIFTL: cout << "SHIFT LEFT"; break;
    case NODE_SHIFTR: cout << "SHIFT RIGHT"; break;
    case NODE_PLUS: cout << "PLUS"; break;
    case NODE_MINUS: cout << "MINUS"; break;
    case NODE_TIMES: cout << "TIMES"; break;
    case NODE_DIVIDE: cout << "DIVIDE"; break;
    case NODE_MODULUS: cout << "MOD"; break;
    case NODE_EXP: cout << "EXP"; break;

    /* leafs */
    case NODE_INTVAL: cout << "INTVAL: " << node->intval; break;
    case NODE_REALVAL: cout << "REALVAL: " << node->realval;  break;
    case NODE_STRINGVAL: cout << "STRINGVAL: " << node->stringval; break;
    case NODE_IDENTIFIER: cout << "IDENTIFIER: " << node->stringval; break;
    case NODE_BOOLVAL: cout << "BOOLVAL: " << node->boolval; break;
    default: throw Error("Unsupported node type!"); break;
  }
  if (node->data_type != TYPE_VOID) {
    cout << " (TYPE:" << typeToString(node->data_type) << ")";
  }

  if (node->lineno) {
    cout << " (LINE:" << node->lineno << ")";
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
  if (argc < 2) {
    cerr << "Please pass a file name!" << endl;
    return 0;
  }

  /* try to parse the tree from this file and dump it to the screen */
  try {
    Node* tree = parseFile(argv[1]);
    print_tree(tree);
  } catch(Error e) {
    cout << e;
  }

  return 0;
}


