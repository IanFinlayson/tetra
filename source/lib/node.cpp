/* functions for dealing with node creation, management etc. */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "tetra.h"

extern int yylineno;

/* node member functions */
Node::Node(NodeKind node_type) {
    this->node_type = node_type;
    this->data_type = NULL;
    parent = NULL;
    symtable = NULL;
    lineno = yylineno; /* this is often inaccurate! */
    num_children = 0;
}

Node::Node(Node* other) {
    this->node_type = other->node_type;
    if (other->data_type) {
        this->data_type = new DataType(*other->data_type);
    } else {
        this->data_type = NULL;
    }
    parent = NULL;
    symtable = NULL;
    lineno = other->lineno; /* this is often inaccurate! */
    num_children = 0;
}

void Node::addChild(Node* child) {
    if (child) {
        children[num_children] = child;
        num_children++;
        child->parent = this;
    }
}

void Node::setDataType(DataType* data_type) {
    this->data_type = data_type;
}

void Node::setLine(int lineno) {
    this->lineno = lineno;
}

int Node::getLine() const {
    return lineno;
}
Node* Node::getParent() const {
    return parent;
}
NodeKind Node::kind() const {
    return node_type;
}
DataType* Node::type() const {
    return data_type;
}
int Node::numChildren() const {
    return num_children;
}

/* insert a symbol into the symtable */
void Node::insertSymbol(Symbol sym) {
    /* create symtable if needed */
    if (!symtable) {
        symtable = new std::map<tstring, Symbol>();
    }

    /* check if it's there first */
    if (symtable->count(sym.getName()) > 0) {
        throw Error("'" + sym.getName() + "' has already been declared", sym.getLine());
    }

    /* add it in */
    symtable->insert(std::pair<tstring, Symbol>(sym.getName(), sym));
}

/* lookup a symbol from a symbol table */
Symbol Node::lookupSymbol(tstring name, int lineno) const {
    /* if there is no symbol table, it's not found! */
    if (!symtable) {
        throw Error("Symbol '" + name + "' not found!", lineno);
    }

    std::map<tstring, Symbol>::iterator it = symtable->find(name);

    if (it == symtable->end()) {
        throw Error("Symbol '" + name + "' not found!", lineno);
    }

    /* return the record */
    return it->second;
}

bool Node::hasSymbol(const tstring& name) const {
    if (!symtable) {
        return false;
    }
    return (symtable->count(name) > 0);
}

/* these are also here */
Symbol::Symbol(tstring name, DataType* type, int lineno, bool constant) {
    this->name = name;
    this->type = type;
    this->lineno = lineno;
    this->constant = constant;
}

bool Symbol::isConst() {
    return constant;
}

int Symbol::getLine() const {
    return lineno;
}
tstring Symbol::getName() const {
    return name;
}
DataType* Symbol::getType() const {
    return type;
}

Symbol::Symbol() {
    name = "";
    type = NULL;
    lineno = 0;
}

/* copy a subtree of nodes */
Node* cloneTree(Node* foot) {
    /* base case */
    if (!foot)
        return NULL;

    /* make this node */
    Node* newRoot = new Node(foot);

    /* copy members */
    newRoot->setIntval(foot->getIntval());
    newRoot->setStrval(foot->getStrval());
    newRoot->setRealval(foot->getRealval());
    newRoot->setBoolval(foot->getBoolval());

    /* make and attach its children */
    for (int i = 0; i < foot->numChildren(); i++) {
        /* make the child */
        Node* newChild = cloneTree(foot->child(i));
        /* add it */
        newRoot->addChild(newChild);
    }

    return newRoot;
}

tstring Node::getStrval() const {
    return strval;
}

tint Node::getIntval() const {
    return intval;
}

treal Node::getRealval() const {
    return realval;
}

tbool Node::getBoolval() const {
    return boolval;
}

void Node::setStrval(const tstring& val) {
    strval = val;
}

void Node::setIntval(const tint& val) {
    intval = val;
}

void Node::setRealval(const treal& val) {
    realval = val;
}

void Node::setBoolval(const tbool& val) {
    boolval = val;
}
