/* types and declarations for Tetra */

#ifndef TETRA_HPP
#define TETRA_HPP

#include <map>
#include <string>
#include <vector>

using namespace std;

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

  /* operator nodes */
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
  NODE_INDREF,
  NODE_VECRANGE,
  NODE_INDEX,

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

/* any type of error is handled with this exception */
class Error {
 public:
  Error(const string& mesg, int lineno = 0);
  string getMessage() const;
  int getLine() const;

 private:
  string mesg;
  int lineno;
};

/* print an error */
ostream& operator<<(ostream& out, const Error& error);

/* the different possibilities a data type can be */
enum DataTypeKind {
  TYPE_INT,
  TYPE_REAL,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_NONE,
  TYPE_VECTOR,
  TYPE_TUPLE,
  TYPE_MUTEX,
  TYPE_TASK
};

/* a data type contains the above enum, along with a pointer to the "sub" type
 * currently this is only used for vectors */
class DataType {
 public:
  DataType(DataTypeKind kind);
  void addSubtype(DataType* subtype);
  DataTypeKind getKind() const;
  DataType* vectorSub() const;

 private:
  DataTypeKind kind;
  std::vector<DataType>* subtypes;
};

/* operators for comparing types for equality */
bool operator==(const DataType& lhs, const DataType& rhs);
bool operator!=(const DataType& lhs, const DataType& rhs);

/* function which returns a string representation of a data type */
string typeToString(DataType* t);

/* an entry in the symbol table */
class Symbol {
 public:
  Symbol();
  Symbol(string name, DataType* type, int lineno);
  int getLine() const;
  string getName() const;
  DataType* getType() const;

 private:
  string name;
  DataType* type;
  int lineno;
};

const int MAX_CHILDREN = 3;

/* the node class represents one element of a parse tree */
class Node {
 public:
  /* constructor and modifiers */
  Node(NodeKind type);
  void setDataType(DataType* data_type);
  void setStringval(const string& stringval);
  void setIntval(int intval);
  void setBoolval(bool boolval);
  void setRealval(double realval);
  void setLine(int lineno);

  /* accessors */
  int getLine() const;
  string getString() const;
  int getInt() const;
  double getReal() const;
  bool getBool() const;
  NodeKind kind() const;
  DataType* type() const;

  /* children functions */
  void addChild(Node* child);
  int numChildren() const;

  /* inlined for performance */
  Node* child(int which) const {
    if (which < 0 || which >= num_children) {
      return NULL;
    } else {
      return children[which];
    }
  }

  /* symbol functions */
  void insertSymbol(Symbol sym);
  Symbol lookupSymbol(string name, int lineno) const;
  bool hasSymbol(const string& name) const;

 private:
  /* the children nodes of this node */
  Node* children[MAX_CHILDREN];
  int num_children;

  /* the symbol table used for this Node - currently only function nodes have
   * one */
  map<string, Symbol>* symtable;

  /* the type of node it is (eg plus vs stmt vs intval etc.) */
  NodeKind node_type;

  /* the data type of the node (NULL if not applicable) */
  DataType* data_type;

  /* the values associated with the node (many will be blank) */
  string stringval;
  double realval;
  int intval;
  bool boolval;

  /* the line number most closely associated with this node */
  int lineno;
};

/* this function does type checking/type inference on a parse tree */
void inferTypes(Node* node);

/* reset lexer internal state */
void reset_lexer();

/* function which parses a file and returns the parse tree */
Node* parseFile(const string& fname);

/* the global symbol table */
extern map<string, Symbol> globals;

/* the lexer function */
int yylex( );

/* dump-tree prototype */
void dumpTreeGraphviz(Node*);

#endif
