/* types and declarations for Tetra */

#ifndef TETRA_HPP
#define TETRA_HPP

#include <map>
#include <string>
#include <vector>

using namespace std;

/* rather than hard code the C types associated with Tetra types, we use these */
typedef int TetraInt;
typedef float TetraReal;
typedef bool TetraBool;
typedef string TetraString;

/* types of nodes */
enum NodeType {
  /* statement and grouping nodes */
  NODE_FUNCTION,
  NODE_FUNCTION_LIST,
  NODE_STATEMENT,
  NODE_FORMAL_PARAM,
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
  NODE_VECREF,
  NODE_INDEX,

  /* function call */
  NODE_FUNCALL,
  NODE_ACTUAL_PARAM_LIST,

  /* leafy nodes */
  NODE_INTVAL,
  NODE_REALVAL,
  NODE_BOOLVAL,
  NODE_STRINGVAL,
  NODE_IDENTIFIER
};

/* the type of data type it is */
enum DataTypeType {
  TYPE_INT,
  TYPE_REAL,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_VOID,
  TYPE_VECTOR
};

/* a data type contains the above enum, along with a pointer to the "sub" type
 * currently this is only used for vectors */
class DataType {
  public:
    DataType(DataTypeType kind);
    void setSubType(DataType* subtype);

    DataTypeType kind;
    DataType* subtype;
};

bool operator==(const DataType& lhs, const DataType& rhs);
bool operator!=(const DataType& lhs, const DataType& rhs);

string typeToString(DataType* t);

/* an entry in the symbol table */
class Symbol {
  public:
    Symbol( );
    Symbol(string name, DataType* type, int lineno);

    string name;
    DataType* type;
    int lineno;
};


/* the node class represents one element of a parse tree */
class Node {
  public:
    /* functions */
    Node(NodeType type);
    void addChild(Node* child);
    void setDataType(DataType* data_type);
    void setStringval(TetraString stringval);
    void setIntval(TetraInt intval);
    void setBoolval(TetraBool boolval);
    void setRealval(TetraReal realval);
    void setLine(int lineno);

    /* the children nodes of this node */
    vector<Node*> children;

    /* the symbol table used for this Node - currently only function nodes have one */
    map<string, Symbol>* symtable;

    /* the type of node it is (eg plus vs stmt vs intval etc.) */
    NodeType node_type;

    /* the data type of the node (NULL if not applicable) */
    DataType* data_type;

    /* the values associated with the node (many will be blank) */
    TetraString stringval;
    TetraReal realval;
    TetraInt intval;
    TetraBool boolval;

    /* the line number most closely associated with this node */
    int lineno;
};

/* this function does type checking/type inference on a parse tree */
void inferTypes(Node* node);

/* function which parses a file and returns the parse tree */
Node* parseFile(const string& fname);

/* any type of error is handled with this exception */
class Error {
  public:
    Error(const string& mesg, int lineno = 0);
    string getMessage( ) const;
    int getLine( ) const;

  private:
    string mesg;
    int lineno;
};

/* print an error */
ostream& operator<<(ostream& out, const Error& error);

#endif



