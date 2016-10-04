/* function, class and variable information */

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
    Symbol(Tstring name, DataType type, int lineNumber, bool constant = false);
    Symbol(Tstring name, DataType* type, int lineNumber, bool constant = false);
    Symbol(const Symbol&);
    ~Symbol();
    Symbol operator=(const Symbol& other);
    int getLine() const;
    Tstring getName() const;
    DataType* getType() const;
    bool isConst();

   private:
    Tstring name;
    DataType* type;
    bool constant;
    int lineNumber;
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
    std::map<Tstring, Node*> lookup;

   public:
    FunctionMap();
    /* Returns the address of a node containing the function body of the
       function
       denoted by functionSignature */
    Node* getFunctionNode(const Tstring functionSignature);
    Node* getFunctionNode(DataType*, Tstring);

    Node* getFunctionNode(const Node* callNode);

    /* Generates a unique function signature based on the name AND the arguments
    */
    static Tstring getFunctionSignature(const Node* node);

    /* Fills the function map given the specified base node */
    void build(Node* tree);

    /* returns true if the map contains a function with the */
    /* provided name, regardless of params and return types */
    bool hasFuncNamed(Tstring name);

    /* returns true if the map contains the function */
    bool hasFunction(Node* node);
    bool hasFunction(DataType*, Tstring);

    DataType getFunctionsNamed(Tstring);

    /* renames functions with the name provided and returns them as a */
    /* vector of pairs */
    std::map<Tstring, Node*> remove(Tstring);

    /* wrapper around std::map.insert */
    void insert(std::pair<Tstring, Node*>);

    /* clear everything out of the function map, e.g. to reuse it */
    void clearAll();
};

/* stores a class definition's context (methods & members) */
class ClassContext {
   public:
    ClassContext(Tstring name);
    ClassContext();
    Tstring getName();
    bool hasMember(Tstring);
    bool hasMethod(DataType*, Tstring);
    bool hasMethodNamed(Tstring);
    void addMember(Symbol);
    void addMembers(Node* node);
    void addMethod(Node*);
    void addMethods(Node*);
    Symbol& getMember(Tstring);
    DataType getMethods(Tstring);
    const Node* getMethod(DataType*, Tstring);
    std::map<Tstring, Node*> removeInits();

   private:
    Tstring name;
    FunctionMap methods;
    std::map<Tstring, Symbol> members;
};

/* the global symbol table */
extern std::map<Tstring, Symbol> globals;

/* the global function map */
extern FunctionMap functions;

#endif
