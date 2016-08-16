/* function, class and variable information */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <map>
#include <string>
#include <vector>

#include "types.h"
#include "values.h"

using namespace std;

/* an entry in the symbol table */
class Symbol {
 public:
  Symbol();
  Symbol(tstring name, DataType* type, int lineno, bool constant = false);
  int getLine() const;
  tstring getName() const;
  DataType* getType() const;
  bool isConst();

 private:
  tstring name;
  DataType* type;
  bool constant;
  int lineno;
};

/*
 * This file builds a function lookup table so that when the interpreter
 * encounters a function     call, it can easily find the address of the
 * appropriate node where the called function code res    ides
 * Since there is only one funciton table per program (even if using multiple
 * files, the further     functions should be addable by calling buildTree for
 * each file's syntax tree) 
 */
class FunctionMap {
 private:
  std::map<tstring, Node*> lookup;

 public:
  FunctionMap();
  // Returns the address of a node containing the function body of the function
  // denoted by functionSignature
  const Node* getFunctionNode(const tstring functionSignature);
  const Node* getFunctionNode(DataType*, tstring);

  const Node* getFunctionNode(const Node* callNode);

  // Generates a unique function signature based on the name AND the arguments
  static tstring getFunctionSignature(const Node* node);

  // Fills the function map given the specified base node
  void build(Node* tree);

  // returns true if the map contains a function with the
  // provided name, regardless of params and return types
  bool hasFuncNamed(tstring name);

  //returns true if the map contains the function
  bool hasFunction(Node* node);
  bool hasFunction(DataType*, tstring);

  DataType* getFunctionsNamed(tstring) ;

  //renames functions with the name provided and returns them as a
  //vector of pairs
  std::map<tstring, Node*> remove(tstring);

  //wrapper around std::map.insert
  void insert(std::pair<tstring, Node*>);

  // clear everything out of the function map, e.g. to reuse it
  void clearAll();
};

/* stores a class definition's context (methods & members) */
class ClassContext {
  public:
    ClassContext(tstring name);
    ClassContext();
    tstring getName();
    bool hasMember(tstring);
    bool hasMethod(DataType*, tstring);
    bool hasMethodNamed(tstring);
    void addMember(Symbol);
    void addMembers(Node* node);
    void addMethod(Node*);
    void addMethods(Node*);
    Symbol getMember(tstring);
    DataType* getMethods(tstring);
    const Node* getMethod(DataType*, tstring);
    std::map<tstring,Node*> removeInits();
  private:
    tstring name;
    FunctionMap methods;
    std::map<tstring, Symbol> members;
};

/* the global symbol table */
extern std::map<tstring, Symbol> globals;

/* the global function map */
extern FunctionMap functions;

#endif
