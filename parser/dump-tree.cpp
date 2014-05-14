/* this program takes in Tetra code and produces a parse tree which allows it
 * to be tested and visualized.  It now produces a structured text dump.
 * TODO add graphical output with graphiz or similar */

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include "tetra.hpp"
#include "parser.gen.hpp"

using namespace std;

/* returns a string representation of a node type */
string stringType(Node* node) {
  stringstream ss;

  switch (node->node_type) {
    /* statements and groups */
    case NODE_FUNCTION: return "FUNC " + node->stringval;
    case NODE_FUNCTION_LIST: return "FUNCS";
    case NODE_STATEMENT: return "STMTS";
    case NODE_FORMAL_PARAM: return "PARAM: " + node->stringval;
    case NODE_FORMAL_PARAM_LIST: return "PARAMS";
    case NODE_PASS: return "PASS";
    case NODE_RETURN: return "RETURN";
    case NODE_BREAK: return "BREAK";
    case NODE_CONTINUE: return "CONTINUE";
    case NODE_IF: return "IF";
    case NODE_WHILE: return "WHILE";
    case NODE_ELIF: return "ELIF";
    case NODE_ELIF_CHAIN: return "ELIF CHAIN";
    case NODE_ELIF_CLAUSE: return "ELIF CLAUSE";

    /* operators */
    case NODE_ASSIGN: return "=";
    case NODE_OR: return "or";
    case NODE_AND: return "and";
    case NODE_LT: return "<";
    case NODE_LTE: return "<=";
    case NODE_GT: return ">";
    case NODE_GTE: return ">=";
    case NODE_EQ: return "==";
    case NODE_NEQ: return "!=";
    case NODE_NOT: return "not";
    case NODE_BITXOR: return "^";
    case NODE_BITAND: return "&";
    case NODE_BITOR: return "|";
    case NODE_BITNOT: return "~";
    case NODE_SHIFTL: return "<<";
    case NODE_SHIFTR: return ">>";
    case NODE_PLUS: return "+";
    case NODE_MINUS: return "-";
    case NODE_TIMES: return "*";
    case NODE_DIVIDE: return "/";
    case NODE_MODULUS: return "%";
    case NODE_EXP: return "EXP";

    /* functions */
    case NODE_FUNCALL: return "CALL: " + node->stringval;
    case NODE_ACTUAL_PARAM_LIST: return "ARGS";

    /* leafs */
    case NODE_INTVAL:
      ss << "INT: " << node->intval;
      return ss.str( );
    case NODE_REALVAL:
      ss << "REAL: " << node->realval;
      return ss.str( );
    case NODE_STRINGVAL: return "\\\"" +  node->stringval + "\\\"";
    case NODE_IDENTIFIER: return "ID " + node->stringval;
    case NODE_BOOLVAL:
      if (node->boolval) {
        return "true";
      } else {
        return "false";
      }
    default:
      throw Error("Unsupported node type!");
  }
}

/* generate a unique label for a node */
string genId( ) {
  static int count = 0;
  count++;
  stringstream ss;
  ss << "n" << count;
  return ss.str( );
}


void dumpNodeGraphviz(Node* node, string id, ofstream& out) {
  if (!node) {
    return;
  }

  /* dump the node designator */
  out << "  " << id << "[label=\"";
  out << stringType(node);
  out << "\"];\n";

  /* for each child */
  for (unsigned int i = 0; i < node->children.size( ); i++) {
    /* generate a new id */
    string childId = genId( );

    /* dump the connection */
    out << "  " << id << " -> " << childId << ";\n";

    /* dump the child itself */
    dumpNodeGraphviz(node->children[i], childId, out);
  }
}


void dumpTreeGraphviz(Node* node) {
  ofstream out("tree.gv");
  out << "digraph G {\n";
  dumpNodeGraphviz(node, "n0", out);
  out << "}\n";
  out.close( );

  system("rm tree.png");
  system("dot -Tpng tree.gv -o tree.png");
  system("rm tree.gv");
  system("xdg-open tree.png");
}


void dumpTreeStdout(Node* node, int level = 0) {
  /* base case */
  if (!node) {
    return;
  }

  /* dump leading spaces */
  for (int i = 0; i < level; i++) {
    cout << "    ";
  }

  /* dump this type of node */
  cout << stringType(node);

  /* dump the type and line if relevant */
  if (node->data_type != TYPE_VOID) {
    cout << " (TYPE:" << typeToString(node->data_type) << ")";
  }

  if (node->lineno) {
    cout << " (LINE:" << node->lineno << ")";
  }

  cout << endl;

  /* dump children */
  for (unsigned int i = 0; i < node->children.size( ); i++) {
    dumpTreeStdout(node->children[i], level + 1);
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
    dumpTreeStdout(tree);
    dumpTreeGraphviz(tree);
  } catch(Error e) {
    cout << e;
  }

  return 0;
}


