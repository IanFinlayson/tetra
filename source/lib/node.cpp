/* functions for dealing with node creation, management etc. */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "tetra.h"

extern int yylineNumber;

/* node member functions */
Node::Node(NodeKind nodeType) {
    this->nodeType = nodeType;
    this->dataType = NULL;
    parent = NULL;
    symtable = NULL;
    lineNumber = yylineNumber; /* this is often inaccurate! */
    numChildren = 0;
}

Node::Node(Node* other) {
    this->nodeType = other->nodeType;
    if (other->dataType) {
        this->dataType = new DataType(*other->dataType);
    } else {
        this->dataType = NULL;
    }
    parent = NULL;
    symtable = NULL;
    lineNumber = other->lineNumber; /* this is often inaccurate! */
    numChildren = 0;
}

Node::~Node() {
    for (int i = 0; i < this->numChildren; i ++) {
        delete this->child(i);
    }

    //delete dataType;
    delete symtable;
}

void Node::addChild(Node* child) {
    if (child) {
        children[numChildren] = child;
        numChildren++;
        child->parent = this;
    }
}

void Node::setDataType(DataType* dataType) {
    this->dataType = dataType;
}

void Node::setLine(int lineNumber) {
    this->lineNumber = lineNumber;
}

int Node::getLine() const {
    return lineNumber;
}
Node* Node::getParent() const {
    return parent;
}
NodeKind Node::kind() const {
    return nodeType;
}
DataType* Node::type() const {
    return dataType;
}
int Node::getNumChildren() const {
    return numChildren;
}

/* insert a symbol into the symtable */
void Node::insertSymbol(Symbol sym) {
    /* create symtable if needed */
    if (!symtable) {
        symtable = new std::map<Tstring, Symbol>();
    }

    /* check if it's there first */
    if (symtable->count(sym.getName()) > 0) {
        throw Error("'" + sym.getName() + "' has already been declared", sym.getLine());
    }

    /* add it in */
    symtable->insert(std::pair<Tstring, Symbol>(sym.getName(), sym));
}

/* lookup a symbol from a symbol table */
Symbol Node::lookupSymbol(Tstring name, int lineNumber) const {
    /* if there is no symbol table, it's not found! */
    if (!symtable) {
        throw Error("Symbol '" + name + "' not found!", lineNumber);
    }

    std::map<Tstring, Symbol>::iterator it = symtable->find(name);

    if (it == symtable->end()) {
        throw Error("Symbol '" + name + "' not found!", lineNumber);
    }

    /* return the record */
    return it->second;
}

bool Node::hasSymbol(const Tstring& name) const {
    if (!symtable) {
        return false;
    }
    return (symtable->count(name) > 0);
}

/* these are also here */
Symbol::Symbol(Tstring name, DataType* type, int lineNumber, bool constant) {
    this->name = name;
    this->type = type;
    this->lineNumber = lineNumber;
    this->constant = constant;
}

bool Symbol::isConst() {
    return constant;
}

int Symbol::getLine() const {
    return lineNumber;
}
Tstring Symbol::getName() const {
    return name;
}
DataType* Symbol::getType() const {
    return type;
}

Symbol::Symbol() {
    name = "";
    type = NULL;
    lineNumber = 0;
}

/* copy a subtree of nodes */
Node* cloneTree(Node* foot) {
    /* base case */
    if (!foot)
        return NULL;

    /* make this node */
    Node* newRoot = new Node(foot);

    /* copy members */
    newRoot->setIntvalue(foot->getIntvalue());
    newRoot->setStringvalue(foot->getStringvalue());
    newRoot->setRealvalue(foot->getRealvalue());
    newRoot->setBoolvalue(foot->getBoolvalue());

    /* make and attach its children */
    for (int i = 0; i < foot->getNumChildren(); i++) {
        /* make the child */
        Node* newChild = cloneTree(foot->child(i));
        /* add it */
        newRoot->addChild(newChild);
    }

    return newRoot;
}

Tstring Node::getStringvalue() const {
    return stringValue;
}

Tint Node::getIntvalue() const {
    return intValue;
}

Treal Node::getRealvalue() const {
    return realValue;
}

Tbool Node::getBoolvalue() const {
    return boolValue;
}

void Node::setStringvalue(const Tstring& value) {
    stringValue = value;
}

void Node::setIntvalue(const Tint& value) {
    intValue = value;
}

void Node::setRealvalue(const Treal& value) {
    realValue = value;
}

void Node::setBoolvalue(const Tbool& value) {
    boolValue = value;
}
