/* node.cpp
 * this file contains functions for dealing with node creation, management etc.
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "tetra.h"

extern int yylineNumber;

/* returns a string representation of a node type */
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
    for (int i = 0; i < this->numChildren; i++) {
        delete this->child(i);
    }
    // std::cout << "deleting node: " << sstringType(this) << " str: " << stringValue << " type : "
    // <<typeToString(dataType)  <<std::endl;
    delete dataType;
    delete symtable;
}

/* get the child of a node */
Node* Node::child(int which) const {
    if (which < 0 || which >= numChildren) {
        return NULL;
    } else {
        return children[which];
    }
}

void Node::addChild(Node* child) {
    if (child) {
        children[numChildren] = child;
        numChildren++;
        child->parent = this;
    }
}

void Node::setDataType(DataType dataType) {
    DataType* temp = this->dataType;
    this->dataType = new DataType(dataType);
    delete temp;
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
        symtable = new std::map<String, Symbol>();
    }

    /* check if it's there first */
    if (symtable->count(sym.getName()) > 0) {
        throw Error("'" + sym.getName() + "' has already been declared", sym.getLine());
    }

    /* add it in */
    symtable->insert(std::pair<String, Symbol>(sym.getName(), sym));
}

/* lookup a symbol from a symbol table */
Symbol& Node::lookupSymbol(String name, int lineNumber) const {
    /* if there is no symbol table or the name is not a key, it's not found! */
    if (!symtable || !symtable->count(name)) {
        throw Error("Symbol '" + name + "' not found!", lineNumber);
    }

    /* return the record */
    return (*symtable)[name];
}

bool Node::hasSymbol(const String& name) const {
    if (!symtable) {
        return false;
    }
    return (symtable->count(name) > 0);
}

/* these are also here */
Symbol::Symbol(String name, DataType* type, int lineNumber, bool constant) {
    this->name = name;
    this->type = new DataType(*type);
    this->lineNumber = lineNumber;
    this->constant = constant;
}

Symbol::Symbol(String name, DataType type, int lineNumber, bool constant) {
    this->name = name;
    this->type = new DataType(type);
    this->lineNumber = lineNumber;
    this->constant = constant;
}

Symbol::Symbol(const Symbol& other) {
    this->name = String(other.name);
    this->type = new DataType(*other.getType());
    this->lineNumber = other.lineNumber;
    this->constant = other.constant;
}

Symbol::~Symbol() {
    delete type;
}

Symbol Symbol::operator=(const Symbol& other) {
    if (this == &other)
        return *this;

    delete this->type;
    this->name = other.name;
    this->type = new DataType(*other.getType());
    this->lineNumber = other.lineNumber;
    this->constant = other.constant;
    return *this;
}

bool Symbol::isConst() {
    return constant;
}

int Symbol::getLine() const {
    return lineNumber;
}
String Symbol::getName() const {
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
Node* cloneTree(Node* root) {
    /* base case */
    if (!root)
        return NULL;

    /* make this node */
    Node* newRoot = new Node(root);

    /* copy members */
    newRoot->setIntvalue(root->getIntvalue());
    newRoot->setStringvalue(root->getStringvalue());
    newRoot->setRealvalue(root->getRealvalue());
    newRoot->setBoolvalue(root->getBoolvalue());

    /* make and attach its children */
    for (int i = 0; i < root->getNumChildren(); i++) {
        /* make the child */
        Node* newChild = cloneTree(root->child(i));
        /* add it */
        newRoot->addChild(newChild);
    }

    return newRoot;
}

String Node::getStringvalue() const {
    return stringValue;
}

Int Node::getIntvalue() const {
    return intValue;
}

Real Node::getRealvalue() const {
    return realValue;
}

Bool Node::getBoolvalue() const {
    return boolValue;
}

void Node::setStringvalue(const String& value) {
    stringValue = value;
}

void Node::setIntvalue(const Int& value) {
    intValue = value;
}

void Node::setRealvalue(const Real& value) {
    realValue = value;
}

void Node::setBoolvalue(const Bool& value) {
    boolValue = value;
}
