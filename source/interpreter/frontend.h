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
  TYPE_DICT,
  TYPE_FUNCTION,
  TYPE_CLASS,
  TYPE_MUTEX,
  TYPE_TASK,
  TYPE_OVERLOAD
};

/* a data type contains the above enum, along with a pointer to the "sub" type
 * currently this is only used for vectors */
class DataType {
 public:
  DataType(DataTypeKind kind);
  DataType(const DataType& other);
  ~DataType();
  DataTypeKind getKind() const;
  std::vector<DataType>* subtypes;
  std::string* className;
  DataType operator=(const DataType& other);
  bool isEmptyContainerType() const;

 private:
  DataTypeKind kind;
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
  Symbol(string name, DataType* type, int lineno, bool constant = false);
  int getLine() const;
  string getName() const;
  DataType* getType() const;
  bool isConst();

 private:
  string name;
  DataType* type;
  bool constant;
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
  Node* getParent() const;

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

  /* the symbol table used for this Node - currently only function nodes have
   * one */
  map<string, Symbol>* symtable;

 private:
  /* the children nodes of this node */
  Node* children[MAX_CHILDREN];
  int num_children;

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
  
  /* pointer to parent node */
  Node* parent;
};

/*
 * This file builds a function lookup table so that when the interpreter
 *encounters a function     call, it can easily find the address of the
 *appropriate node where the called function code res    ides
 *Since there is only one funciton table per program (even if using multiple
 *files, the further     functions should be addable by calling buildTree for
 *each file's syntax tree) 
 */
class FunctionMap {
 private:
  std::map<std::string, Node*> lookup;
  Node** functionLookup;
  static void concatSignature(const Node*, std::string&);

 public:
  FunctionMap();
  // Returns the address of a node containing the function body of the function
  // denoted by functionSignature
  const Node* getFunctionNode(const std::string functionSignature);
  const Node* getFunctionNode(DataType*, std::string);

  const Node* getFunctionNode(const Node* callNode);

  // Generates a unique function signature based on the name AND the arguments
  static std::string getFunctionSignature(const Node* node);

  // Fills the function map given the specified base node
  void build(Node* tree);

  // does some pre-work to optimize variable lookup
  void optimizeLookup(const Node*);
  void optimizeFunctionLookup(Node*);

  // Release allocated resources from the instance
  void cleanup();

  // returns true if the map contains a function with the
  // provided name, regardless of params and return types
  bool hasFuncNamed(std::string name);

  //returns true if the map contains the function
  bool hasFunction(Node* node);
  bool hasFunction(DataType*, std::string);

  DataType* getFunctionsNamed(std::string) ;

  //renames functions with the name provided and returns them as a
  //vector of pairs
  std::map<std::string, Node*> remove(std::string);

  //wrapper around std::map.insert
  void insert(std::pair<string, Node*>);
};

/* stores a class definition's context (methods & members) */
class ClassContext {
  public:
    ClassContext(string name);
    ClassContext();
    string getName();
    bool hasMember(std::string);
    bool hasMethod(DataType*, std::string);
    bool hasMethodNamed(std::string);
    void addMember(Symbol);
    void addMembers(Node* node);
    void addMethod(Node*);
    void addMethods(Node*);
    Symbol getMember(std::string);
    DataType* getMethods(std::string);
    const Node* getMethod(DataType*, std::string);
    std::map<std::string,Node*> removeInits();
  private:
    string name;
    FunctionMap methods;
    std::map<string, Symbol> members;
};

/* takes a tree of actual params and builds a tuple_type */
void buildParamTupleType(DataType*, Node*, Node*);
  
/* this function does initial pass to populate 
 * globals/classes/free functions for type checker */
void initTypes(Node* node);

/* this function does type checking/type inference on a parse tree */
void inferTypes(Node* node);

/* reset lexer internal state */
void reset_lexer();

/* function which parses a file and returns the parse tree */
Node* parseFile(const string& fname);

/* the global symbol table */
extern map<string, Symbol> globals;

/* the global function map */
extern FunctionMap functions;

/* the lexer function */
int yylex( );

/* dump-tree prototype */
void dumpTreeGraphviz(Node*);

/* populate symbol table in function node with params */
void inferParams(Node*,Node* func=NULL);

/* recursively provides string representation of a datatype */
string typeToString(DataType*);

#endif
