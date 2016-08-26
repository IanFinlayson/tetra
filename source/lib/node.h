/* parse tree node information */

#ifndef NODE_H
#define NODE_H

#include <map>
#include <string>
#include <vector>

#include "values.h"

/* forward declarations */
class DataType;
class tdata;
class Symbol;

class tstring;


/* types of nodes */
enum NodeKind {
  /* statement and grouping nodes */
  NODE_FUNCTION,
  NODE_TOPLEVEL_LIST,
  NODE_STATEMENT,
  NODE_FORMAL_PARAM_LIST,
  NODE_PASS,
  NODE_RETURN,
  NODE_BREAK,
  NODE_CONTINUE,
  NODE_IF,
  NODE_ELIF,
  NODE_ELIF_CHAIN,
  NODE_ELIF_CLAUSE,
  NODE_WHILE,
  NODE_FOR,
  NODE_PARALLEL,
  NODE_PARFOR,
  NODE_BACKGROUND,
  NODE_LOCK,
  NODE_CONST,
  NODE_GLOBAL,
  NODE_WAIT,
  NODE_DECLARATION,
  NODE_LAMBDA,
  NODE_IDENTIFIERS,
  NODE_OPEN,
  NODE_IMPORT,
  NODE_CLASS,
  NODE_CLASS_PART,
  NODE_METHOD_CALL,
  NODE_SELF,
  NODE_TUPLE_TYPES,

  /* operator nodes */
  NODE_DOT,
  NODE_ASSIGN,
  NODE_OR,
  NODE_AND,
  NODE_LT,
  NODE_LTE,
  NODE_GT,
  NODE_GTE,
  NODE_EQ,
  NODE_NEQ,
  NODE_NOT,
  NODE_BITXOR,
  NODE_BITAND,
  NODE_BITOR,
  NODE_BITNOT,
  NODE_SHIFTL,
  NODE_SHIFTR,
  NODE_PLUS,
  NODE_MINUS,
  NODE_TIMES,
  NODE_DIVIDE,
  NODE_MODULUS,
  NODE_EXP,
  NODE_VECRANGE,
  NODE_INDEX,
  NODE_IN,

  /* function call */
  NODE_FUNCALL,
  NODE_ACTUAL_PARAM_LIST,

  /* vector constants */
  NODE_VECVAL,
  NODE_TUPVAL,
  NODE_DICTVAL,

  /* leafy nodes */
  NODE_INTVAL,
  NODE_REALVAL,
  NODE_BOOLVAL,
  NODE_STRINGVAL,
  NODE_NONEVAL,
  NODE_IDENTIFIER
};


/* the node class represents one element of a parse tree */
class Node {
 public:
  /* constructor and modifiers */
  Node(NodeKind type);
  Node(Node* other);
  void setDataType(DataType* data_type);
  void setValue(tdata* val);
  void setLine(int lineno);

  /* accessors */
  int getLine() const;
  NodeKind kind() const;
  DataType* type() const;
  Node* getParent() const;

  /* children functions */
  void addChild(Node* child);
  int numChildren() const;

  /* get the child of a node */
  Node* child(int which) const {
    if (which < 0 || which >= num_children) {
      return NULL;
    } else {
      return children[which];
    }
  }

  /* symbol functions */
  void insertSymbol(Symbol sym);
  Symbol lookupSymbol(tstring name, int lineno) const;
  bool hasSymbol(const tstring& name) const;

  /* the symbol table used for this Node - currently only function nodes have one */
  std::map<tstring, Symbol>* symtable;

  tstring getStrval() const;
  tint getIntval() const;
  treal getRealval() const;
  tbool getBoolval() const;

  void setStrval(const tstring& val);
  void setIntval(const tint& val);
  void setRealval(const treal& val);
  void setBoolval(const tbool& val);

 private:
  /* the children nodes of this node */
  static const int MAX_CHILDREN = 3;
  Node* children[MAX_CHILDREN];
  int num_children;

  /* the type of node it is (eg plus vs stmt vs intval etc.) */
  NodeKind node_type;

  /* the data type of the node (NULL if not applicable) */
  DataType* data_type;

  /* the line number most closely associated with this node */
  int lineno;
  
  /* pointer to parent node */
  Node* parent;

  /* the values which can be present in some nodes */
  tstring strval;
  tint intval;
  treal realval;
  tbool boolval;

};

/* clone a tree */
Node* cloneTree(Node*);

/* dump-tree prototype */
void dumpTreeGraphviz(Node*);

#endif
