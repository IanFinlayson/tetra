/* types and declarations for Tetra */

#ifndef TETRA_HPP
#define TETRA_HPP

#include <map>
#include <string>
#include <vector>

using namespace std;

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

/* a data type contains the above enum, along with a pointer to the "sub" type
 * currently this is only used for vectors */
class DataType {
  public:
    DataType(DataTypeType kind);
    void setSubType(DataType* subtype);
    DataTypeType getKind( ) const;
    DataType* getSub( ) const;

  private:
    DataTypeType kind;
    DataType* subtype;
};

/* operators for comparing types for equality */
bool operator==(const DataType& lhs, const DataType& rhs);
bool operator!=(const DataType& lhs, const DataType& rhs);

/* function which returns a string representation of a data type */
string typeToString(DataType* t);

/* an entry in the symbol table */
class Symbol {
  public:
    Symbol( );
    Symbol(string name, DataType* type, int lineno);
    int getLine( ) const;
    string getName( ) const;
    DataType* getType( ) const;

  private:
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
    void setStringval(const string& stringval);
    void setIntval(int intval);
    void setBoolval(bool boolval);
    void setRealval(double realval);
    void setLine(int lineno);

    /* begin cut */
    int getLine( ) const {
      return lineno;
    }
    string getString( ) const {
      return stringval;
    }
    int getInt( ) const {
      return intval;
    }
    double getReal( ) const {
      return realval;
    }
    bool getBool( ) const {
      return boolval;
    }
    NodeType kind( ) const {
      return node_type;
    }
    DataType* type( ) const {
      return data_type;
    }
    void setType(DataType* t) {
      data_type = t;
    }
    int numChildren( ) const {
      return children.size( );
    }
    Node* child(int which) const {
      return children[which];
    }
    /* insert a symbol into the symtable */
    void insertSymbol(Symbol sym) {
      /* create symtable if needed */
      if (!symtable) {
        symtable = new map<string, Symbol>( );
      }

      /* check if it's there first */
      if (symtable->count(sym.getName( )) > 0) {
        throw Error("'" + sym.getName( ) + "' has already been declared", sym.getLine( ));
      }

      /* add it in */
      symtable->insert(pair<string, Symbol>(sym.getName( ), sym));
    }

    /* lookup a symbol from a symbol table */
    Symbol lookupSymbol(string name, int lineno) {
      map<string, Symbol>::iterator it = symtable->find(name);

      if (it == symtable->end( )) {
        throw Error("Symbol '" + name + "' not found!", lineno);
      }

      /* return the record */
      return it->second;
    }
    bool hasSymbol(const string& name) {
      if (!symtable) {
        return false;
      }
      return (symtable->count(name) > 0);
    }
    /* end cut */


  private:
    /* the children nodes of this node */
    vector<Node*> children;

    /* the symbol table used for this Node - currently only function nodes have one */
    map<string, Symbol>* symtable;

    /* the type of node it is (eg plus vs stmt vs intval etc.) */
    NodeType node_type;

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

/* function which parses a file and returns the parse tree */
Node* parseFile(const string& fname);

#endif



