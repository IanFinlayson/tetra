#ifndef TETRA_HPP
#define TETRA_HPP

#include <string>
#include <vector>

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
  NODE_WHILE

  
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
    Node(NodeType type);
    void addChild(Node* child);
    void setDataType(DataType data_type);
    void setIdentifier(std::string identifier);

  private:
    NodeType node_type;
    DataType data_type;
    std::vector<Node*> children;
    std::string identifier;
};

/* error handling routine */
void fail(const std::string& mesg);

#endif



