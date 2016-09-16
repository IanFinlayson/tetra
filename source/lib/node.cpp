/* functions for dealing with node creation, management etc. */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "tetra.h"

extern int yylineNumber;
extern int numNodes;

/* returns a string representation of a node type */
Tstring stringType(Node* node) {
    Tstring ss;

    switch (node->kind()) {
        /* statements and groups */
        case NODE_FUNCTION:
            return "FUNC " + node->getStringvalue();
        case NODE_TOPLEVEL_LIST:
            return "TOP LEVELS";
        case NODE_STATEMENT:
            return "STMTS";
        case NODE_FORMAL_PARAM_LIST:
            return "PARAMS";
        case NODE_IDENTIFIERS:
            return "IDENTIFIERS";
        case NODE_OPEN:
            return "OPEN";
        case NODE_IMPORT:
            return "IMPORT";
        case NODE_PASS:
            return "PASS";
        case NODE_WAIT:
            return "WAIT";
        case NODE_RETURN:
            return "RETURN";
        case NODE_BREAK:
            return "BREAK";
        case NODE_CONTINUE:
            return "CONTINUE";
        case NODE_IF:
            return "IF";
        case NODE_WHILE:
            return "WHILE";
        case NODE_FOR:
            return "FOR";
        case NODE_ELIF:
            return "ELIF";
        case NODE_ELIF_CHAIN:
            return "ELIF CHAIN";
        case NODE_ELIF_CLAUSE:
            return "ELIF CLAUSE";
        case NODE_PARALLEL:
            return "PARALLEL";
        case NODE_PARFOR:
            return "PARFOR";
        case NODE_BACKGROUND:
            return "BACKGROUND";
        case NODE_LOCK:
            return "LOCK";
        case NODE_CONST:
            return "CONST";
        case NODE_GLOBAL:
            return "GLOBAL";
        case NODE_DECLARATION:
            return "DECLARATION: " + node->getStringvalue();
        case NODE_LAMBDA:
            return "LAMBDA";
        case NODE_CLASS:
            return "CLASS " + node->getStringvalue();
        case NODE_CLASS_PART:
            return "CLASS PART";
        case NODE_DOT:
            return "DOT";
        case NODE_METHOD_CALL:
            return "METHOD CALL";
        case NODE_SELF:
            return "SELF";

        /* operators */
        case NODE_ASSIGN:
            return "=";
        case NODE_OR:
            return "or";
        case NODE_AND:
            return "and";
        case NODE_LT:
            return "<";
        case NODE_LTE:
            return "<=";
        case NODE_GT:
            return ">";
        case NODE_GTE:
            return ">=";
        case NODE_EQ:
            return "==";
        case NODE_NEQ:
            return "!=";
        case NODE_NOT:
            return "not";
        case NODE_BITXOR:
            return "^";
        case NODE_BITAND:
            return "&";
        case NODE_BITOR:
            return "|";
        case NODE_BITNOT:
            return "~";
        case NODE_SHIFTL:
            return "<<";
        case NODE_SHIFTR:
            return ">>";
        case NODE_PLUS:
            return "+";
        case NODE_MINUS:
            return "-";
        case NODE_TIMES:
            return "*";
        case NODE_DIVIDE:
            return "/";
        case NODE_MODULUS:
            return "%";
        case NODE_EXP:
            return "EXP";
        case NODE_IN:
            return "in";

        /* functions */
        case NODE_FUNCALL:
            return "CALL: " + node->getStringvalue();
        case NODE_ACTUAL_PARAM_LIST:
            return "ARGS";

        /* lists */
        case NODE_INDEX:
            return "INDEX";
        case NODE_LISTVAL:
            return "LISTVAL";
        case NODE_TUPVAL:
            return "TUPVAL";
        case NODE_DICTVAL:
            return "DICTVAL";
        case NODE_LISTRANGE:
            return "LISTRANGE";

        /* leafs */
        case NODE_INTVAL:
            return "INT:" + node->getStringvalue();
        case NODE_REALVAL:
            return "REAL: " + node->getStringvalue();
        case NODE_STRINGVAL:
            return "\"" + node->getStringvalue() + "\"";
        case NODE_IDENTIFIER:
            return "ID " + node->getStringvalue();
        case NODE_BOOLVAL:
            return "BOOL: " + node->getStringvalue();
        case NODE_NONEVAL:
            return "NONE";
        default:
            throw Error("Unsupported node type!");
    }
}
/* node member functions */
Node::Node(NodeKind nodeType) {
    this->nodeType = nodeType;
    this->dataType = NULL;
    parent = NULL;
    symtable = NULL;
    lineNumber = yylineNumber; /* this is often inaccurate! */
    numChildren = 0;
    numNodes++;
    nodeNum = numNodes; 
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
    numNodes++;
    nodeNum = numNodes; 
}

Node::~Node() {
    for (int i = 0; i < this->numChildren; i++) {
        delete this->child(i);
    }

    std::cout << "deleting node #" << nodeNum<< std::endl;
    std::cout << "stringtype = " << stringType(this) << std::endl;
    delete dataType;
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
