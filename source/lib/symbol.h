/* symbol.h
 * function, class and variable information */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <map>
#include <string>
#include <vector>

#include "types.h"
#include "values.h"

/* an entry in the symbol table */
class Symbol {
   public:
    Symbol();
    Symbol(String name, DataType type, int lineNumber, bool constant = false);
    Symbol(String name, DataType* type, int lineNumber, bool constant = false);
    Symbol(const Symbol&);
    ~Symbol();
    Symbol operator=(const Symbol& other);
    int getLine() const;
    String getName() const;
    DataType* getType() const;
    bool isConst();

   private:
    String name;
    DataType* type;
    bool constant;
    int lineNumber;
};

/*
 * This file builds a function lookup table so that when the interpreter
 * encounters a function call, it can easily find the address of the
 * appropriate node where the called function code res ides Since there is only
 * one funciton table per program (even if using multiple files, the further
 * functions should be addable by calling buildTree for each file's syntax
 * tree)
 */
class FunctionMap {
   private: std::map<String, Node*> lookup;

   public:
    FunctionMap();
    /* Returns the address of a node containing the function body of the
       function denoted by functionSignature */
    Node* getFunctionNode(const String functionSignature);
    Node* getFunctionNode(DataType*, String);

    Node* getFunctionNode(const Node* callNode);

    /* Generates a unique function signature based on the name AND the arguments
    */
    static String getFunctionSignature(const Node* node);

    /* Fills the function map given the specified base node */
    void build(Node* tree);

    /* returns true if the map contains a function with the */
    /* provided name, regardless of params and return types */
    bool hasFuncNamed(String name);

    /* returns true if the map contains the function */
    bool hasFunction(Node* node);
    bool hasFunction(DataType*, String);

    DataType getFunctionsNamed(String);

    /* renames functions with the name provided and returns them as a */
    /* vector of pairs */
    std::map<String, Node*> remove(String);

    /* wrapper around std::map.insert */
    void insert(std::pair<String, Node*>);

    /* clear everything out of the function map, e.g. to reuse it */
    void clearAll();
};

/* stores a class definition's context (methods & members) */
class ClassContext {
   public:
    ClassContext(String name);
    ClassContext();
    String getName();
    bool hasMember(String);
    bool hasMethod(DataType*, String);
    bool hasMethodNamed(String);
    void addMember(Symbol);
    void addMembers(Node* node);
    void addMethod(Node*);
    void addMethods(Node*);
    Symbol& getMember(String);
    DataType getMethods(String);
    const Node* getMethod(DataType*, String);
    std::map<String, Node*> removeInits();

   private:
    String name;
    FunctionMap methods;
    std::map<String, Symbol> members;
};

/* the global symbol table */
extern std::map<String, Symbol> globals;

/* the global function map */
extern FunctionMap functions;

#endif
