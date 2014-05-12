/* types and declarations for Tetra */

#ifndef TETRA_HPP
#define TETRA_HPP

#include <string>
#include <vector>

using namespace std;

/* types of nodes */
enum NodeType {
  NODE_FUNCTION,
  NODE_FUNCTION_LIST,
  NODE_STATEMENT,
  NODE_PARAM,
  NODE_PARAM_LIST,
  NODE_TYPE,
  NODE_PASS,
  NODE_RETURN,
  NODE_BREAK,
  NODE_CONTINUE,
  NODE_IF,
  NODE_WHILE,


  NODE_INTVAL
};

/* data types */
enum DataType {
  TYPE_INT,
  TYPE_REAL,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_VOID
};

/* the node class represents one element of a parse tree */
class Node {
  public:
    /* functions */
    Node(NodeType type);
    void addChild(Node* child);
    void setDataType(DataType data_type);
    void setIdentifier(string identifier);

    /* data */
    vector<Node*> children;
    NodeType node_type;
    DataType data_type;
    string identifier;
};

/* error handling routine */
void fail(const string& mesg);

#endif



