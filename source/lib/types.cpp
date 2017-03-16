/* types.cpp
 * this file contains functions for type handling and inference */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "tetra.h"

extern Node* root;

/* the symbol table for storing constants and globals */
std::map<String, Symbol> globals;

/* map for storing classes */
std::map<String, ClassContext> classes;

/* map for storing free functions */
FunctionMap functions;

/* prototypes */
void inferBlock(Node*, Node*);
void checkClassTypes(Node*);
DataType inferExpression(Node*, Node*);

/* return a string of a data type */
String typeToString(DataType t) {
    switch (t.getKind()) {
        case TYPE_NONE:
            return "none";
        case TYPE_INT:
            return "int";
        case TYPE_REAL:
            return "real";
        case TYPE_STRING:
            return "string";
        case TYPE_BOOL:
            return "bool";
        case TYPE_MUTEX:
            return "mutex";
        case TYPE_TASK:
            return "task";
        case TYPE_LIST:
            return "[" + typeToString(&((*(t.subtypes))[0])) + "]";
        case TYPE_DICT:
            return "{" + typeToString(&((*(t.subtypes))[0])) + ":" +
                   typeToString(&((*(t.subtypes))[1])) + "}";
        case TYPE_TUPLE: {
            String typeString = "(";
            for (unsigned long int i = 0; i < t.subtypes->size(); i++) {
                typeString += typeToString(&((*(t.subtypes))[i])) + ",";
            }
            /* if the tuple has elements ... */
            if (t.subtypes->size() > 0)
                /* then get rid of the trailing comma */
                typeString = typeString.substring(0, typeString.length() - 1);

            return typeString + ")";
        }
        case TYPE_CLASS:
            return *(t.className);
        case TYPE_FUNCTION: {
            return typeToString(&((*(t.subtypes))[0])) + "->" + typeToString(&((*(t.subtypes))[1]));
        }
        case TYPE_OVERLOAD: {
            String all = "Overloaded function: {";
            for (unsigned long int i = 0; i < t.subtypes->size(); i++) {
                all += typeToString(&((*(t.subtypes))[i])) + " ";
            }
            return all + "}";
        }
        default:
            throw Error("typeToString: Unknown data type");
    }
}

String typeToString(DataType* t) {
    return typeToString(*t);
}

/* class context functions */
ClassContext::ClassContext(String name) {
    this->name = name;
}

ClassContext::ClassContext() {
    this->name = "#ImBroken";
}

String ClassContext::getName() {
    return this->name;
}

bool ClassContext::hasMember(String varName) {
    return this->members.count(varName);
}

bool ClassContext::hasMethod(DataType* type, String name) {
    return this->methods.hasFunction(type, name);
}

void ClassContext::addMember(Symbol sym) {
    /* if this context already has the member... */
    if (members.count(sym.getName())) {
        /* COMPLAIN ! */
        throw Error("Class '" + this->name + "' already has member named '" + sym.getName() + "'.",
                    sym.getLine());
    }
    /* if this context already has a similarly named method... */
    if (methods.hasFuncNamed(sym.getName())) {
        /* COMPLAIN ! */
        throw Error("Class '" + this->name + "' already has method named '" + sym.getName() + "'.",
                    sym.getLine());
    }
    /* otherwise, add it */
    members[sym.getName()] = sym;
}

void ClassContext::addMethods(Node* root) {
    this->methods.build(root);
}

/* Given a class node, this function adds the member vars
 * into the class map (Cannot type check yet as this class
 * may have members that are dependent on
 * classes that have not been seen yet.)*/
void ClassContext::addMembers(Node* node) {
    /* add this part */
    if (node->kind() == NODE_IDENTIFIER) {
        this->addMember(Symbol(node->getStringvalue(), node->type(), node->getLine()));
    } else if (node->kind() == NODE_CLASS_PART) {
        /* recursively add other parts */
        this->addMembers(node->child(0));
        if (node->child(1)) {
            this->addMembers(node->child(1));
        }
    }
}

Symbol& ClassContext::getMember(String name) {
    return members[name];
}

const Node* ClassContext::getMethod(DataType* type, String name) {
    return methods.getFunctionNode(type, name);
}

DataType ClassContext::getMethods(String name) {
    return methods.getFunctionsNamed(name);
}

bool ClassContext::hasMethodNamed(String name) {
    return methods.hasFuncNamed(name);
}

std::map<String, Node*> ClassContext::removeInits() {
    /* remove any inits from the list of methods
     * and return them */
    return methods.remove("init");
}

/* data type functions */
DataType::DataType() {
    this->kind = TYPE_NONE;
    this->subtypes = new std::vector<DataType>;
    this->className = new String();
}

DataType::DataType(DataTypeKind kind) {
    this->kind = kind;
    this->subtypes = new std::vector<DataType>;
    this->className = new String();
}

DataType::DataType(const DataType& other) {
    this->kind = other.kind;
    this->subtypes = new std::vector<DataType>;
    for (unsigned long i = 0; i < other.subtypes->size(); i++) {
        this->subtypes->push_back((*(other.subtypes))[i]);
    }
    this->className = new String(*other.className);
}

DataType::~DataType() {
    delete className;
    delete subtypes;
}

/* return true if the type represents an empty
 * container (i.e. is a dict/list with no
 * subtype. */
bool DataType::isEmptyContainerType() const {
    return ((this->getKind() == TYPE_LIST || this->getKind() == TYPE_DICT) &&
            this->subtypes->size() == 0);
}

DataTypeKind DataType::getKind() const {
    return kind;
}

/* compare two data types for equality */
bool operator==(const DataType& lhs, const DataType& rhs) {
    /* check for an overload/function pairing */
    if (lhs.getKind() == TYPE_OVERLOAD && rhs.getKind() == TYPE_FUNCTION) {
        /* check each of them */
        for (long unsigned int i = 0; i < lhs.subtypes->size(); i++) {
            /* if one has the right params... */
            if ((*((*(lhs.subtypes))[i].subtypes))[0] == (*(rhs.subtypes))[0]) {
                return true;
            }
        }
        return false;
    }

    /* check for a function/overload pairing */
    if (rhs.getKind() == TYPE_OVERLOAD && lhs.getKind() == TYPE_FUNCTION) {
        /* check each of them */
        for (long unsigned int i = 0; i < rhs.subtypes->size(); i++) {
            /* if one has the right params... */
            if ((*((*(rhs.subtypes))[i].subtypes))[0] == (*(lhs.subtypes))[0]) {
                return true;
            }
        }
        return false;
    }

#if 0
    TODO FIXME EVENTUALLY THIS SHOULD WORK

    /* check for int/real things
     * if both are numerical, then the types match (due to auto promotion) */
    if ((lhs.getKind() == TYPE_INT || lhs.getKind() == TYPE_REAL) &&
        (rhs.getKind() == TYPE_INT || rhs.getKind() == TYPE_REAL)) {
        return true;
    }
#endif

    /* otherwise, if they're not the same kind, fail */
    if (lhs.getKind() != rhs.getKind()) {
        return false;
    }

    /* if they're lists or dicts and either is empty.. */
    if (lhs.getKind() == TYPE_LIST && (lhs.isEmptyContainerType() || rhs.isEmptyContainerType())) {
        /* then they match! */
        return true;
    }

    if (lhs.getKind() == TYPE_LIST) {
        /* recursively ensure the subtypes match */
        return (*(lhs.subtypes))[0] == (*(rhs.subtypes))[0];
    }

    /* for dictionaries, recursively check the key and value types */
    if (lhs.getKind() == TYPE_DICT) {
        return (((*(lhs.subtypes))[0] == (*(rhs.subtypes))[0]) &&
                ((*(lhs.subtypes))[1] == (*(rhs.subtypes))[1]));
    }

    /* for classes, just check the names */
    if (lhs.getKind() == TYPE_CLASS) {
        return *(lhs.className) == *(rhs.className);
    }

    /* for tuples and functions, check that they have the same
     * number of subtypes */
    if (lhs.subtypes->size() != rhs.subtypes->size()) {
        return false;
    }

    /* if they are both tuple or both functions and we made it
     * here, then they have the same number of subtypes,
     * so we need to compare the subtypes recursively. */
    if (lhs.getKind() == TYPE_TUPLE || lhs.getKind() == TYPE_FUNCTION) {
        bool same = true;
        unsigned long int i = 0;
        while (same && i < lhs.subtypes->size()) {
            same = ((*(lhs.subtypes))[i] == (*(rhs.subtypes))[i]);
            i++;
        }
        return same;
    }

    /* otherwise return true */
    return true;
}

/* compare two data types for in-equality */
bool operator!=(const DataType& lhs, const DataType& rhs) {
    return !(lhs == rhs);
}

DataType DataType::operator=(const DataType& other) {
    std::vector<DataType>* newSubtypes = new std::vector<DataType>;
    for (unsigned long i = 0; i < other.subtypes->size(); i++) {
        newSubtypes->push_back((*(other.subtypes))[i]);
    }

    kind = other.kind;
    *className = *other.className;

    std::vector<DataType>* oldSubtypes = this->subtypes;
    this->subtypes = newSubtypes;
    delete oldSubtypes;
    return *this;
}

/* infer the param subtype from a function call */
void inferActualParams(Node* node, Node* function) {
    if (node && node->kind() == NODE_ACTUAL_PARAM_LIST) {
        inferActualParams(node->child(0), function);
        inferActualParams(node->child(1), function);
    } else if (node) {
        inferExpression(node, function);
    } else {
        return;
    }
}

void buildParamTupleType(DataType* type, const Node* node) {
    if (node && node->kind() == NODE_ACTUAL_PARAM_LIST) {
        buildParamTupleType(type, node->child(0));
        buildParamTupleType(type, node->child(1));
    } else if (node) {
        type->subtypes->push_back(*node->type());
    } else {
        return;
    }
}

/* infer the types of a print call */
DataType inferPrint(Node* pcall, Node* function) {
    /* just infer each expression, but we don't care what it is */

    /* get the first arg */
    Node* arg = pcall->child(1);

    /* while there are arguments */
    while (arg) {
        /* if this arg is a param, check the child */
        if ((arg->kind() == NODE_ACTUAL_PARAM_LIST) && (arg->getNumChildren() >= 1)) {
            inferExpression(arg->child(0), function);
        }
        /* else if it's some other child, check it itself */
        else {
            inferExpression(arg, function);
        }

        /* if the next arg is another arg, move on */
        if ((arg->getNumChildren() >= 2) && (arg->child(1)->kind() == NODE_ACTUAL_PARAM_LIST)) {
            arg = arg->child(1);
        }

        /* else just check it and we're done */
        else if (arg->getNumChildren() >= 2) {
            inferExpression(arg->child(1), function);
            arg = NULL;
        } else {
            arg = NULL;
        }
    }

    return DataType(TYPE_NONE);
}

DataType inferLen(Node* functionCall, Node* function) {
    /* check that there is one argument */
    if (functionCall->getNumChildren() != 2 || functionCall->child(1)->getNumChildren() != 1) {
        throw Error("len function expects one argument", functionCall->getLine());
    }

    /* infer the argument and capture its type */
    DataType t = inferExpression(functionCall->child(1)->child(0), function);

    /* check that it is a list or a string */
    if ((t.getKind() != TYPE_LIST) && (t.getKind() != TYPE_STRING) && (t.getKind() != TYPE_TUPLE) &&
        (t.getKind() != TYPE_DICT)) {
        throw Error("len function must be called on string, list, tuple, or dictionary",
                    functionCall->getLine());
    }

    /* should return an int */
    return DataType(TYPE_INT);
}

DataType inferSleep(Node* functionCall, Node* function) {
    /* check that there is one argument */
    if (functionCall->getNumChildren() != 2 || functionCall->child(1)->getNumChildren() != 1) {
        throw Error("sleep function expects one argument", functionCall->getLine());
    }

    /* infer the argument and capture its type */
    DataType t = inferExpression(functionCall->child(1)->child(0), function);

    /* check that it is a real value */
    if (t.getKind() != TYPE_REAL) {
        throw Error("sleep function must be called on a real value", functionCall->getLine());
    }

    /* returns none */
    return DataType(TYPE_NONE);
}

DataType inferInput(Node* functionCall, Node* function) {
    /* make sure there are 0 or 1 parameters */
    if (functionCall->getNumChildren() == 2 && functionCall->child(1)->getNumChildren() > 1) {
        throw Error("input should not have more than one parameter", functionCall->getLine());
    }

    /* infer the type of the argument, if any */
    if (functionCall->getNumChildren() == 2 && functionCall->child(1)->getNumChildren() == 1) {
        DataType t = inferExpression(functionCall->child(1)->child(0), function);
        if (t.getKind() != TYPE_STRING) {
            throw Error("input expects a string parameter", functionCall->getLine());
        }
    }

    /* should return an string */
    return DataType(TYPE_STRING);
}

DataType inferConversion(Node* functionCall, Node* function) {
    /* check that there is one argument */
    if (functionCall->getNumChildren() != 2 || functionCall->child(1)->getNumChildren() != 1) {
        throw Error("type conversion function expects one argument", functionCall->getLine());
    }

    /* infer the argument and capture its type */
    DataType t = inferExpression(functionCall->child(1)->child(0), function);

    /* do the check based on which convert it is */
    if (functionCall->child(0)->getStringvalue() == "int") {
        switch (t.getKind()) {
            case TYPE_REAL:
            case TYPE_STRING:
            case TYPE_BOOL:
                break;
            default:
                throw Error("Unexpected argument type in int conversion", functionCall->getLine());
        }
        return DataType(TYPE_INT);

    } else if (functionCall->child(0)->getStringvalue() == "real") {
        switch (t.getKind()) {
            case TYPE_INT:
            case TYPE_STRING:
                break;
            default:
                throw Error("Unexpected argument type in real conversion", functionCall->getLine());
        }
        return DataType(TYPE_REAL);

    } else if (functionCall->child(0)->getStringvalue() == "bool") {
        switch (t.getKind()) {
            case TYPE_INT:
            case TYPE_STRING:
                break;
            default:
                throw Error("Unexpected argument type in bool conversion", functionCall->getLine());
        }
        return DataType(TYPE_BOOL);

    } else if (functionCall->child(0)->getStringvalue() == "string") {
        switch (t.getKind()) {
            case TYPE_REAL:
            case TYPE_INT:
            case TYPE_BOOL:
                break;
            default:
                throw Error("Unexpected argument type in string conversion",
                            functionCall->getLine());
        }
        return DataType(TYPE_STRING);
    } else {
        throw Error("Unhandled conversion function", functionCall->getLine());
    }
}

/* this function checks if a function call is part of the standard library and
 * infers it */
DataType inferStdlib(Node* functionCall, Node* function, bool& is_stdlib) {
    is_stdlib = true;

    if (functionCall->child(0)->getStringvalue() == "print") {
        return inferPrint(functionCall, function);
    }

    if (functionCall->child(0)->getStringvalue() == "len") {
        return inferLen(functionCall, function);
    }

    if (functionCall->child(0)->getStringvalue() == "sleep") {
        return inferSleep(functionCall, function);
    }

    if (functionCall->child(0)->getStringvalue() == "input") {
        return inferInput(functionCall, function);
    }

    if ((functionCall->child(0)->getStringvalue() == "int") ||
        (functionCall->child(0)->getStringvalue() == "real") ||
        (functionCall->child(0)->getStringvalue() == "string") ||
        (functionCall->child(0)->getStringvalue() == "bool")) {
        return inferConversion(functionCall, function);
    }

    is_stdlib = false;
    return DataType(TYPE_NONE);
}

/* accepts a node as a parameter and returns true if that node
 * cannot be inside a lambda, false otherwise */
bool noLambdaParents(Node* node) {
    if (!node) {
        return false;
    }

    NodeKind k = node->kind();

    if (!k || k == NODE_ASSIGN || k == NODE_LOCK || k == NODE_BACKGROUND || k == NODE_PARALLEL ||
        k == NODE_WHILE || k == NODE_PARFOR || k == NODE_FOR || k == NODE_ELIF_CLAUSE ||
        k == NODE_IF) {
        return true;
    }
    return false;
}

/* takes a node as a parameter and returns the innermost lambda that
 * contains it, or null if it is not contained in a lambda */
Node* nextLambda(Node* startNode) {
    Node* curNode = startNode->getParent();

    /* keep going until you reach a node that can't be contained
     * in a lambda expression. */
    while (curNode && !noLambdaParents(curNode)) {
        /* if we reached a lambda, return it */
        if (curNode->kind() == NODE_LAMBDA) {
            return curNode;
        }
        /* otherwise, go up one */
        curNode = curNode->getParent();
    }
    /* if we got here, than we did not find
     * a containing lambda, and we reached a node
     * which cannot be contained in a lambda expression */
    return NULL;
}

/* check if this node is inside a parallel, parfor,
 * or background block */
bool inPar(Node* node) {
    Node* current = node;
    /* loop until we get to the containing function */
    while (current->kind() != NODE_FUNCTION) {
        /* check to see if we have found a containing parallel
         * block of any kind */
        if (current->kind() == NODE_PARFOR || current->kind() == NODE_PARALLEL ||
            current->kind() == NODE_BACKGROUND) {
            return true;
        }
        /* go up one node */
        current = current->getParent();
    }
    /* if we reached a function node and never found
     * a parallel node */
    return false;
}

/* check if this node is inside a loop */
bool inLoop(Node* node) {
    Node* current = node;
    /* loop until we get to the containing function */
    while (current->kind() != NODE_FUNCTION) {
        /* check to see if we have found a containing loop
         * of any kind */
        if (current->kind() == NODE_PARFOR || current->kind() == NODE_FOR ||
            current->kind() == NODE_WHILE) {
            return true;
        }
        /* go up one node */
        current = current->getParent();
    }
    /* if we reached a function node and never found
     * a parallel node */
    return false;
}

/* check that all contained blocks return */
bool checkReturns(Node* node) {
    switch (node->kind()) {
        case NODE_STATEMENT:
            /* a chain of statements returns on all paths
             * if it's statement returns on all paths or the
             * remaining chain does */
            return checkReturns(node->child(0)) || checkReturns(node->child(1));

        case NODE_IF:
            /* returns true if there is an else
             * and both the if and the else return true */
            return node->getNumChildren() == 3 && checkReturns(node->child(1)) &&
                   checkReturns(node->child(2));

        case NODE_ELIF:
            /* returns true if there is and else
             * and all the conditional paths return*/
            return node->getNumChildren() == 3 && checkReturns(node->child(0)) &&
                   checkReturns(node->child(1)) && checkReturns(node->child(2));

        case NODE_ELIF_CHAIN:
            /* returns true if both children return true */
            return checkReturns(node->child(0)) && checkReturns(node->child(1));

        case NODE_ELIF_CLAUSE:
            /* returns true if the block returns true */
            return checkReturns(node->child(1));

        case NODE_FUNCTION:
            /* returns what it's block returns */
            return checkReturns(node->child(node->getNumChildren() - 1));

        case NODE_RETURN:
            return true;

        default:
            return false;
    }
}

/* look up parent class */
Node* getClassNode(Node* node) {
    if (node->kind() == NODE_CLASS) {
        return node;
    } else if (node->kind() == NODE_TOPLEVEL_LIST) {
        return NULL;
    } else {
        return getClassNode(node->getParent());
    }
}

/* find identifier */
Symbol findIdSym(Node* expr, Node* function = NULL) {
    /* check if it's a lambda param first */
    /* look for lambdas first */
    Node* lambda = nextLambda(expr);
    Symbol sym = Symbol();
    bool found = false;
    while (lambda && !found) {
        /* if we found the identifier, get its symbol*/
        if (lambda->hasSymbol(expr->getStringvalue())) {
            sym = lambda->lookupSymbol(expr->getStringvalue(), lambda->getLine());
            found = true;
        } else {
            /* otherwise, go to the next lambda up */
            lambda = nextLambda(lambda);
        }
    }

    /* if not a lambda, see if it's local to the function */
    if (function && function->hasSymbol(expr->getStringvalue())) {
        /* look it up */
        sym = function->lookupSymbol(expr->getStringvalue(), expr->getLine());

        /* if it is in a class, check there for a member var*/
    } else if (getClassNode(expr) &&
               classes[getClassNode(expr)->getStringvalue()].hasMember(expr->getStringvalue())) {
        sym = classes[getClassNode(expr)->getStringvalue()].getMember(expr->getStringvalue());

        /* if it is in a class, check there for a method */
    } else if (getClassNode(expr) &&
               classes[getClassNode(expr)->getStringvalue()].hasMethodNamed(
                   expr->getStringvalue())) {
        /* get all the methods */
        sym =
            Symbol(expr->getStringvalue(),
                   classes[getClassNode(expr)->getStringvalue()].getMethods(expr->getStringvalue()),
                   expr->getLine(), true);

        /* next check for globals/constants */
    } else if (globals.count(expr->getStringvalue()) > 0) {
        /* look it up */
        sym = globals[expr->getStringvalue()];

        /* next check if it's the name of a free function */
    } else if (functions.hasFuncNamed(expr->getStringvalue())) {
        /* look it up */
        sym = Symbol(expr->getStringvalue(), functions.getFunctionsNamed(expr->getStringvalue()),
                     expr->getLine(), true);
    }
    /* return the thing we found (or NULL) */
    return sym;
}

/* infer the types of an expression, and also return the type */
DataType inferExpressionPrime(Node* expr, Node* function) {
    /* the left hand side, right hand side, and result used below */
    DataType lhs, rhs;

    /* switch on the type of expression */
    switch (expr->kind()) {
        case NODE_ASSIGN: {
            /* get the type of the right hand side */
            rhs = inferExpression(expr->child(1), function);

            /* if the left hand side is an identifier... */
            if (expr->child(0)->kind() == NODE_IDENTIFIER) {
                /* try to find the id */
                Symbol sym = findIdSym(expr->child(0), function);
                /* if it doesn't exist  and it IS inferable...*/
                if (sym.getName() == "" && !rhs.isEmptyContainerType()) {
                    /* infer it! */
                    lhs = rhs;
                    function->insertSymbol(
                        Symbol(expr->child(0)->getStringvalue(), lhs, expr->child(0)->getLine()));

                    /* if it doesn't exist and it IS NOT inferable... */
                } else if (sym.getName() == "") {
                    /* complain so much */
                    throw Error("Cannot infer subtype of empty list/dictionary.", expr->getLine());

                    /* if it exists and is a unassignable ... */
                } else if (sym.isConst()) {
                    throw Error(
                        "Cannot assign to constant, class, method, or "
                        "free function.",
                        expr->getLine());
                } else {
                    /* if we end up here, all is well - update lhs */
                    lhs = *sym.getType();
                }

                /* if it's not directly and identifier.. */
            } else {
                /* get the type of the left hand side */
                lhs = inferExpression(expr->child(0), function);

                /* if there is an index on the left... */
                if (expr->child(0)->kind() == NODE_INDEX) {
                    /* then check for immutable types */
                    DataTypeKind assignKind = expr->child(0)->child(0)->type()->getKind();
                    if (assignKind == TYPE_TUPLE || assignKind == TYPE_STRING) {
                        throw Error("Cannot assign to immutable types (string or tuple).",
                                    expr->getLine());
                    }
                }
            }

            /* make sure both sides are the same type */
            if (rhs != lhs) {
                throw Error("Assignment of incompatible types.", expr->getLine());
            }

            /* return the type of the rhs */
            expr->child(0)->setDataType(lhs);
            return rhs;
        }

        case NODE_OR:
        case NODE_AND:
            /* check that both children are bools */
            lhs = inferExpression(expr->child(0), function);
            rhs = inferExpression(expr->child(1), function);
            if ((lhs.getKind() != TYPE_BOOL) || (rhs.getKind() != TYPE_BOOL)) {
                throw Error("Only bool values may be used with and/or", expr->getLine());
            }
            /* the result is a bool as well */
            return DataType(TYPE_BOOL);
        case NODE_LT:
        case NODE_LTE:
        case NODE_GT:
        case NODE_GTE:
            /* check that both sides have the same type
             * at some point add in int->real promotion */
            lhs = inferExpression(expr->child(0), function);
            rhs = inferExpression(expr->child(1), function);
            if (lhs != rhs) {
                throw Error("Only matching types can be compared", expr->getLine());
            }

            /* check that they are strings, ints, or reals */
            if (lhs.getKind() != TYPE_STRING && lhs.getKind() != TYPE_REAL &&
                lhs.getKind() != TYPE_INT) {
                throw Error("This comparison invalid on given type.", expr->getLine());
            }

            /* the result is a bool */
            return DataType(TYPE_BOOL);

        case NODE_EQ:
        case NODE_NEQ:
            /* check that both sides have the same type
             * at some point add in int->real promotion */
            lhs = inferExpression(expr->child(0), function);
            rhs = inferExpression(expr->child(1), function);
            if (lhs != rhs) {
                throw Error("Only matching types can be compared", expr->getLine());
            }

            /* the result is a bool */
            return DataType(TYPE_BOOL);

        case NODE_NOT:
            /* check that the operand is bool */
            lhs = inferExpression(expr->child(0), function);
            if (lhs.getKind() != TYPE_BOOL) {
                throw Error("Operand of not must be a bool", expr->getLine());
            }
            return DataType(TYPE_BOOL);

        case NODE_IN: {
            /* get the types of both sides */
            lhs = inferExpression(expr->child(0), function);
            rhs = inferExpression(expr->child(1), function);

            /* if the container on the right is a dictionary or a list... */
            if (rhs.getKind() == TYPE_LIST || rhs.getKind() == TYPE_DICT) {
                /* make sure that the list subtype/ dictionary key type
                 * matches the
                 * type of the left operand */
                if (lhs != ((*(rhs.subtypes))[0])) {
                    /* otherwise, complain */
                    throw Error("Mismatched operands to 'in' operator.", expr->getLine());
                    /* if we got here then it matched */
                } else {
                    return DataType(TYPE_BOOL);
                }

                /* if the container on the right is a tuple... */
            } else if (rhs.getKind() == TYPE_TUPLE) {
                /* look through the types of each of its elements */
                for (long unsigned i = 0; i < rhs.subtypes->size(); i++) {
                    /* if one type matches the left side... */
                    if (lhs == (*(rhs.subtypes))[i]) {
                        /* then it passes type checking */
                        return DataType(TYPE_BOOL);
                    }
                    /* if we get here, then the tuple doesn't have any
                     * element that is the same type as the left operand */
                }
                throw Error("Mismatched operands to 'in' operator.", expr->getLine());

                /* if the left side is a string */
            } else if (rhs.getKind() == TYPE_STRING) {
                /* make sure the right side is too */
                if (lhs.getKind() == TYPE_STRING) {
                    return DataType(TYPE_BOOL);
                    /* otherwise, COMPLAIN */
                } else {
                    throw Error("Mismatched operands to 'in' operator.", expr->getLine());
                }
                /* if we get here, then the right side does not evaluate to a
                 * container
                 */
            } else {
                Error(
                    "Invalid operand to 'in' operator. Right operand must be "
                    "of type "
                    "list, dictionary, tuple, or string.",
                    expr->getLine());
            }
        }
        case NODE_BITXOR:
        case NODE_BITAND:
        case NODE_BITOR:
        case NODE_SHIFTL:
        case NODE_SHIFTR:
            /* check that both operands are integers */
            lhs = inferExpression(expr->child(0), function);
            rhs = inferExpression(expr->child(1), function);

            if ((lhs.getKind() != TYPE_INT) || (rhs.getKind() != TYPE_INT)) {
                throw Error("Operands to bitwise operator must be integer", expr->getLine());
            }

            /* returns an integer back */
            return DataType(TYPE_INT);

        case NODE_BITNOT:
            /* check that the operand is an int */
            lhs = inferExpression(expr->child(0), function);
            if (lhs.getKind() != TYPE_INT) {
                throw Error("Operand to bitwise not must be an integer", expr->getLine());
            }
            return DataType(TYPE_INT);

        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_TIMES:
        case NODE_DIVIDE:
        case NODE_MODULUS:
        case NODE_EXP:
            /* check that both operands match and that they are int/real
             * at some point add in int->real promotion... */
            lhs = inferExpression(expr->child(0), function);
            rhs = inferExpression(expr->child(1), function);

            if (lhs != rhs) {
                throw Error("In binary operator, the types " + typeToString(lhs) + " and " +
                                typeToString(rhs) + " are not compatible",
                            expr->getLine());
            }

            if ((lhs.getKind() != TYPE_INT) && (lhs.getKind() != TYPE_REAL)) {
                /* special case: adding strings and list is OK */
                if ((expr->kind() == NODE_PLUS) &&
                    ((lhs.getKind() == TYPE_STRING) || (lhs.getKind() == TYPE_LIST))) {
                    /* s'alright */
                } else {
                    throw Error("Numeric type required", expr->getLine());
                }
            }

            /* return the same type back */
            return lhs;

        case NODE_UMINUS:
            /* check the argument */
            lhs = inferExpression(expr->child(0), function);

            /* only int or real */
            if ((lhs.getKind() != TYPE_INT) && (lhs.getKind() != TYPE_REAL)) {
                throw Error("Numeric type required for unary minus", expr->getLine());
            }

            /* return the same type back */
            return lhs;

        case NODE_INDEX: {
            /* check children */
            lhs = inferExpression(expr->child(0), function);
            rhs = inferExpression(expr->child(1), function);

            /* get the kind of lhs*/
            DataTypeKind kind = lhs.getKind();

            /* return the type of the container's values */
            /* dictionaries */
            if (kind == TYPE_DICT) {
                /* check the key type */
                if ((*(lhs.subtypes))[0] != rhs) {
                    throw Error("Key has incompatible type.", expr->getLine());
                }
                return (*(lhs.subtypes))[1];

                /* tuples */
            } else if (kind == TYPE_TUPLE) {
                /* check the index type */
                if (rhs.getKind() != TYPE_INT) {
                    throw Error("Tuple index must be an integer.", expr->getLine());
                }
                /* make sure the index is in range */
                if ((unsigned long) expr->child(1)->getIntvalue().toInt() < lhs.subtypes->size()) {
                    /* get return the type of the index */
                    return (*(lhs.subtypes))[expr->child(1)->getIntvalue().toInt()];
                    /* if it isn't in range */
                } else {
                    /* complain! */
                    throw Error("Tuple index out of range.", expr->getLine());
                }

                /* lists */
            } else if (kind == TYPE_LIST) {
                /* check the index type */
                if (rhs.getKind() != TYPE_INT) {
                    throw Error("List index must be an integer.", expr->getLine());
                }
                return ((*(lhs.subtypes))[0]);

                /* strings */
            } else if (kind == TYPE_STRING) {
                /* check the index type */
                if (rhs.getKind() != TYPE_INT) {
                    throw Error("String index must be an integer.", expr->getLine());
                }
                return lhs;

                /* otherwise it isn't an indexable type */
            } else {
                throw Error("Index performed on unindexable type.", expr->getLine());
            }
        }

        case NODE_LISTRANGE: {
            lhs = inferExpression(expr->child(0), function);
            rhs = inferExpression(expr->child(1), function);

            /* make sure the types are both ints */
            if (lhs.getKind() != TYPE_INT || lhs.getKind() != TYPE_INT) {
                throw Error("Cannot create range with types other than INT.", expr->getLine());
            }

            /* a listrange can only possibly be a list of ints */
            DataType t = DataType(TYPE_LIST);
            t.subtypes->push_back(DataType(TYPE_INT));
            return t;
        }

        case NODE_FUNCALL: {
            /* check for stl functions */
            bool is_stdlib;
            lhs = inferStdlib(expr, function, is_stdlib);
            if (is_stdlib) {
                return lhs;
            }

            /* if it's not an stl function */
            /* infer the identifier */
            lhs = inferExpression(expr->child(0), function);
            /* make an empty tuple type for the params */
            DataType* rhsParams = new DataType(TYPE_TUPLE);
            /* if there are arguments... */
            if (expr->getNumChildren() > 1) {
                /* infer them */
                inferActualParams(expr->child(1), function);
                /* add them to the tuple */
                buildParamTupleType(rhsParams, expr->child(1));
            }

            /* make sure that we found a matching function */
            /* if we have a single function... */
            if (lhs.getKind() == TYPE_FUNCTION) {
                /* if it has the right params... */
                if ((*(lhs.subtypes))[0] == *rhsParams) {
                    /* return the return type */
                    delete rhsParams;
                    return (*(lhs.subtypes))[1];
                }
                /* if we have multiple possibilities */
            } else if (lhs.getKind() == TYPE_OVERLOAD) {
                /* check each of them */
                for (long unsigned int i = 0; i < lhs.subtypes->size(); i++) {
                    /* if one has the right params... */
                    if ((*((*(lhs.subtypes))[i].subtypes))[0] == *rhsParams) {
                        /* return its return type */
                        delete rhsParams;
                        return (*((*(lhs.subtypes))[i].subtypes))[1];
                    }
                }
            }
            /* if we get here, then we either haven't found any matches,
             * or the ones that we have found don't accept the correct arguments
             */
            throw Error("No matching function for: '" + expr->child(0)->getStringvalue() +
                            typeToString(rhsParams) + "'.",
                        expr->getLine());
        }

        case NODE_ACTUAL_PARAM_LIST:
            throw Error("inferExpression: should not be a param list here");
            break;

        case NODE_IDENTIFIER: { /* first check if it already has a type (this
                                   happens for declarations) */
            if (expr->type()) {
                /* if it is a classType, make sure the class exists */
                if (expr->type()->getKind() == TYPE_CLASS &&
                    !classes.count(*(expr->type()->className))) {
                    throw Error("Class '" + *(expr->type()->className) + "' does not exist.",
                                expr->getLine());
                }

                /* if the identifier already exists in this function... */
                if (function->hasSymbol(expr->getStringvalue())) {
                    /* complain! */
                    throw Error("The identifier '" + expr->getStringvalue() + "' already exists.",
                                expr->getLine());
                }

                /* if we make it here, we need to add this declaration to this
                 * function's sym table */
                function->insertSymbol(
                    Symbol(expr->getStringvalue(), expr->type(), expr->getLine()));
                /* then just return the type it already has */
                return *expr->type();
            }
            /* otherwise, if it doesn't already have a type... */
            /* if the id already exists, get its type */
            Symbol sym = findIdSym(expr, function);

            /* if we didn't find it... */
            if (sym.getName() == "") {
                /* complain! */
                throw Error(
                    "Reference to non-existent identifier '" + expr->getStringvalue() + "'.",
                    expr->getLine());
            }

            /* otherwise, return the type */
            return *sym.getType();
        }

        /* return these types */
        case NODE_INTVAL:
            return DataType(TYPE_INT);
        case NODE_REALVAL:
            return DataType(TYPE_REAL);
        case NODE_BOOLVAL:
            return DataType(TYPE_BOOL);
        case NODE_STRINGVAL:
            return DataType(TYPE_STRING);
        case NODE_NONEVAL:
            return DataType(TYPE_NONE);
        case NODE_LISTVAL: {
            DataType dt = DataType(TYPE_LIST);
            Node* currNode = expr;
            /* traverse the subtree of list values */
            while (currNode && currNode->getNumChildren() > 0) {
                DataType elemType = inferExpression(currNode->child(0), function);
                /* if this is the first element, add the subtype */
                if (dt.subtypes->size() == 0) {
                    (dt.subtypes)->push_back(elemType);
                    /* if there is a previous subtype, make sure they match */
                } else if (dt.subtypes->size() == 1 && ((*(dt.subtypes))[0]) != elemType) {
                    throw Error("Mismatched list types", expr->getLine());
                }
                /* set current node to the next one */
                currNode = currNode->child(1);
            }
            return dt;
        }

        case NODE_DICTVAL: {
            DataType dt = DataType(TYPE_DICT);
            Node* currNode = expr;
            /* traverse the subtree of dictvals */
            while (currNode && currNode->getNumChildren() > 0) {
                DataType keyType = inferExpression(currNode->child(0), function);
                DataType valType = inferExpression(currNode->child(1), function);
                /* if this is the first element, add the subtypes */
                if (dt.subtypes->size() == 0) {
                    dt.subtypes->push_back(keyType);
                    dt.subtypes->push_back(valType);
                    /* if there are previous subtypes, make sure they match */
                } else if (dt.subtypes->size() == 2 &&
                           (((*(dt.subtypes))[0] != keyType) || ((*(dt.subtypes))[1] != valType))) {
                    throw Error("Mismatched key/value types", expr->getLine());
                }
                /* set current node to the next one */
                currNode = currNode->child(2);
            }

            return dt;
        }

        case NODE_TUPVAL: {
            DataType dt = DataType(TYPE_TUPLE);
            Node* currNode = expr;
            /* traverse the subtree of tuple values */
            while (currNode && currNode->getNumChildren() > 0) {
                DataType elemType = inferExpression(currNode->child(0), function);

                /* add the subtype */
                dt.subtypes->push_back(elemType);

                /* set current node to the next one */
                currNode = currNode->child(1);
            }

            return dt;
        }

        case NODE_LAMBDA: {
            /* make sure that any classes that are referred to
             * actually exist */
            checkClassTypes(expr->child(0));
            /* add the params to the symtable and datatype */
            inferParams(expr);

            /* infer the the return type */
            if (expr->getNumChildren() > 1) {
                expr->type()->subtypes->push_back(inferExpression(expr->child(1), function));
            } else {
                expr->type()->subtypes->push_back(inferExpression(expr->child(0), function));
            }

            return *expr->type();
        }

        case NODE_DOT: {
            lhs = inferExpression(expr->child(0), function);
            /* check that class exists */
            if (!lhs.className || !classes.count(*lhs.className)) {
                throw Error("Class does not exist.", expr->getLine());
                /* check that class has member var */
            } else if (!classes[*lhs.className].hasMember(expr->child(1)->getStringvalue())) {
                throw Error("Class does not contain specified member variable.", expr->getLine());
            }

            /* return the type of the member variable */
            return *classes[*lhs.className].getMember(expr->child(1)->getStringvalue()).getType();
        }

        case NODE_SELF: {
            /* return parent class type*/
            Node* classNode = getClassNode(expr);
            /* throw error if no parent class found */
            if (!classNode) {
                throw Error("Reference to 'self' outside of class.", expr->getLine());
            }

            /* return the class' type */
            DataType type = DataType(TYPE_CLASS);
            *(type.className) = classNode->getStringvalue();
            return type;
        }

        case NODE_METHOD_CALL: {
            lhs = inferExpression(expr->child(0), function);
            /* infer the tuple_type of the actual params */
            DataType* rhsParams = new DataType(TYPE_TUPLE);

            /* if there are actual params... */
            if (expr->child(1)->getNumChildren() > 1) {
                /* infer them */
                inferActualParams(expr->child(1)->child(1), function);
                /* add them to the tuple_type */
                buildParamTupleType(rhsParams, expr->child(1)->child(1));
            }

            /* check that class exists */
            if (!lhs.className || !classes.count(*lhs.className)) {
                throw Error("Class '" + *(lhs.className) + "' does not exist.", expr->getLine());

                /* check that class has method */
            } else if (!classes[*lhs.className].hasMethod(
                           rhsParams, expr->child(1)->child(0)->getStringvalue())) {
                throw Error("Class '" + *(lhs.className) + "' does not contain method '" +
                                expr->child(1)->child(0)->getStringvalue() + "'. " +
                                expr->child(1)->getStringvalue() + typeToString(rhsParams),
                            expr->getLine());
            }

            /* return the return type of the method */
            return *classes[*lhs.className]
                        .getMethod(rhsParams, expr->child(1)->child(0)->getStringvalue())
                        ->type();
        }

        case NODE_DECLARATION: {
        }

        default:
            throw Error("inferExpression: unknown node type");
            break;
    }

    throw Error("inferExpression: unhandled type");
}

/* infer an expression and assign it to the node */
DataType inferExpression(Node* expr, Node* function) {
    /* do the inference */
    DataType t = inferExpressionPrime(expr, function);

    /* assign it into this node */
    expr->setDataType(t);

    /* return it */
    return *expr->type();
}

/* infer/ type check function for tasks and locks */
void checkMuTasks(Node* block, Node* function) {
    /* is it a task or a mutex? */
    DataTypeKind kind;
    switch (block->kind()) {
        /* task */
        case NODE_BACKGROUND:
        case NODE_WAIT:
            kind = TYPE_TASK;
            break;
        /* mutex */
        case NODE_LOCK:
            kind = TYPE_MUTEX;
            break;
        /* some other type (this should never happen) */
        default:
            throw Error("Something's wrotten in the state of Denmark!", block->getLine());
    }

    /* if there are two children or it's a wait block, it's named */
    if (block->child(1) || block->kind() == NODE_WAIT) {
        /* check if the identifier exists */
        Symbol sym = findIdSym(block->child(0), function);

        /* if the identifier doesn't exist yet
         * and it's not a wait node*/
        if (sym.getName() == "" && block->kind() != NODE_WAIT) {
            /* make a symbol for it */
            sym = Symbol(block->child(0)->getStringvalue(), DataType(kind),
                         block->child(0)->getLine());
            /* add to this function's symtable */
            function->insertSymbol(sym);

            /* if the identifier doesn't exist and it
             * is a wait node*/
        } else if (sym.getName() == "") {
            throw Error("Cannot wait for task that has not been created",
                        block->child(0)->getLine());
        }

        /* if the type is wrong... */
        if (sym.getType()->getKind() != kind) {
            throw Error("Task or mutex identifier has improper type.", block->child(0)->getLine());
        }

        /* set the type */
        block->child(0)->setDataType(*sym.getType());

        /* if there is a block ... */
        if (block->child(1)) {
            /* check the block */
            inferBlock(block->child(1), function);
        }

        /* otherwise, it's an unnamed task/lock */
    } else {
        /* check the sub-block */
        inferBlock(block->child(0), function);
    }
}

/* infer types in a block and add them into the symbol table */
void inferBlock(Node* block, Node* function) {
    if (!block) {
        return;
    }

    /* switch on the different types */
    switch (block->kind()) {
        case NODE_STATEMENT:
            /* handle both children */
            inferBlock(block->child(0), function);
            inferBlock(block->child(1), function);
            break;
        case NODE_RETURN: {
            /* make sure that it isn't in a parallel block of any kind */
            if (inPar(block)) {
                throw Error("Cannot place return statement in parallel block.", block->getLine());
            }

            /* infer the expression */
            DataType ret;
            if (block->child(0)) {
                ret = inferExpression(block->child(0), function);
            } else {
                ret = DataType(TYPE_NONE);
            }

            /* check that it matches the return type */
            if (ret != function->type()->subtypes->back()) {
                throw Error("Return value type '" + typeToString(ret) +
                                "' does not match function's declared type '" +
                                typeToString(&function->type()->subtypes->back()) + "'.",
                            block->getLine());
            }
            break;
        }
        case NODE_IF: {
            /* infer the type of the expression */
            DataType cond = inferExpression(block->child(0), function);

            /* check that it is a BOOL */
            if (cond.getKind() != TYPE_BOOL) {
                throw Error("if condition must be a bool", block->getLine());
            }

            /* infer both the then and else blocks */
            inferBlock(block->child(1), function);
            inferBlock(block->child(2), function);
            break;
        }
        case NODE_ELIF:
            /* check the first child which is the first elf clause */
            inferBlock(block->child(0), function);

            /* check the second child which is the chain of the rest of the
             * elifs */
            inferBlock(block->child(1), function);

            /* check the third child which is the else clause (maybe NULL) */
            inferBlock(block->child(2), function);
            break;
        case NODE_ELIF_CHAIN:
            /* check the clause on the left and the chain on the right */
            inferBlock(block->child(0), function);
            inferBlock(block->child(1), function);
            break;
        case NODE_ELIF_CLAUSE: {
            /* check the expression on the left */
            DataType cond = inferExpression(block->child(0), function);

            /* check that it is a BOOL */
            if (cond.getKind() != TYPE_BOOL) {
                throw Error("elif condition must be a bool", block->getLine());
            }

            /* check the statements on the right */
            inferBlock(block->child(1), function);
            break;
        }
        case NODE_WHILE: {
            /* infer the type of the expression */
            DataType cond = inferExpression(block->child(0), function);

            /* check that it is a BOOL */
            if (cond.getKind() != TYPE_BOOL) {
                throw Error("while condition must be a bool", block->getLine());
            }

            /* infer the body */
            inferBlock(block->child(1), function);
            break;
        }
        case NODE_PARFOR:
        case NODE_FOR: {
            /* infer the type of the expression */
            DataType expr_type = inferExpression(block->child(1), function);

            /* make sure it is some type of list */
            if (expr_type.getKind() != TYPE_LIST && expr_type.getKind() != TYPE_DICT &&
                expr_type.getKind() != TYPE_STRING) {
                throw Error(
                    "for expression must be of type list, "
                    "dictionary, or string",
                    block->getLine());
            }

            /* see if the identifier for indexing already exists */
            Symbol idxSym = findIdSym(block->child(0), function);

            /* if it does, make sure it is the right type */
            if (idxSym.getName() != "") {
                /* if it's a string, the symbol must be too */
                if (expr_type.getKind() == TYPE_STRING) {
                    if (idxSym.getType()->getKind() != TYPE_STRING) {
                        throw Error("Type of index variable '" + block->child(0)->getStringvalue() +
                                        "' is incompatible with container elements.",
                                    block->getLine());
                    }
                } else {
                    /* otherwise, the subtype should match the symbol type */
                    if ((*(idxSym.getType())) != (*(expr_type.subtypes))[0]) {
                        throw Error("Type of index variable '" + block->child(0)->getStringvalue() +
                                        "' is incompatible with container elements.",
                                    block->getLine());
                    }
                }

                /* set the type of the block to match */
                block->child(0)->setDataType(*idxSym.getType());

                /* otherwise, if it doesn't exist, add it */
            } else if (idxSym.getName() == "") {
                if (expr_type.getKind() == TYPE_STRING) {
                    /* make it a string */
                    function->insertSymbol(Symbol(block->child(0)->getStringvalue(),
                                                  DataType(TYPE_STRING), block->getLine()));
                    block->child(0)->setDataType(DataType(TYPE_STRING));
                } else {
                    /* make it whatever the subtype is */
                    function->insertSymbol(Symbol(block->child(0)->getStringvalue(),
                                                  &(*(expr_type.subtypes))[0], block->getLine()));
                    block->child(0)->setDataType((*(expr_type.subtypes))[0]);
                }
            }

            /* check the block under this */
            inferBlock(block->child(2), function);
            break;
        }
        case NODE_PARALLEL:
            inferBlock(block->child(0), function);
            break;
        case NODE_BACKGROUND:
        case NODE_LOCK:
        case NODE_WAIT:
            checkMuTasks(block, function);
            break;
        /* these require no work... */
        case NODE_PASS:
            break;
        case NODE_BREAK:
        case NODE_CONTINUE:
            /* if not contained in a loop... */
            if (!inLoop(block)) {
                throw Error("Break and continue are only permitted within a loop.",
                            block->getLine());
            }
            break;

        default:
            /* handle it as an expression */
            inferExpression(block, function);
    }
}

/* infer the function's params and add them to the
 * symtable/update the datatype */
void inferParams(Node* node, Node* function) {
    /* base case */
    if (!node) {
        return;
    }

    /* if we are in the function node */
    if (node->kind() == NODE_FUNCTION || node->kind() == NODE_LAMBDA) {
        /* make a new function datatype */
        DataType type = DataType(TYPE_FUNCTION);
        /* add an empty param tuple */
        type.subtypes->push_back(DataType(TYPE_TUPLE));
        /* add the return type (if it has one)*/
        if (node->kind() == NODE_FUNCTION) {
            type.subtypes->push_back(*(node->type()));
        }
        /* replace the existing datatype */
        node->setDataType(type);

        /* if there are params */
        if (node->getNumChildren() > 1) {
            /* add the params*/
            inferParams(node->child(0), node);
        }
    }

    /* infer any list elements */
    if (node->kind() == NODE_FORMAL_PARAM_LIST) {
        inferParams(node->child(0), function);
        /* if there are more... */
        if (node->child(1)) {
            inferParams(node->child(1), function);
        }
    }

    /* else if it's just one param, handle it */
    else if (node->kind() == NODE_DECLARATION) {
        /* add the param to the symbol table */
        function->insertSymbol(Symbol(node->getStringvalue(), node->type(), node->getLine()));
        /* add the param to the datatype */
        (*(function->type()->subtypes))[0].subtypes->push_back(*node->type());
    }
}

/* infer a global/const definition */
void inferGlobal(Node* node, bool isConst = false) {
    /* name for errors */
    String varType;
    isConst ? varType = "Constant" : varType = "Global";

    /* check if this symbol exists already, (it shouldn't)*/
    if (globals.count(node->child(0)->getStringvalue()) > 0 ||
        functions.hasFuncNamed(node->child(0)->getStringvalue()) ||
        classes.count(node->child(0)->getStringvalue()) > 0) {
        throw Error(varType + " '" + node->child(0)->getStringvalue() + "' has been defined.");
    }

    /* if there is an assignment */
    if (node->child(1)) {
        /* get the type of the right hand side */
        DataType rhs = inferExpression(node->child(1), NULL);

        /* if the left hand side has a declared type.. */
        if (node->child(0)->type()) {
            /* check that the types match */
            if (*(node->child(0)->type()) != rhs) {
                throw Error("Assignment of unmatched types.", node->getLine());
            }
            /* if there is no declared type */
        } else {
            /* infer the type from the right side */
            node->setDataType(rhs);
        }

        /* set the node type */
        node->child(0)->setDataType(rhs);
    }
    /* add it in */
    globals.insert(std::pair<String, Symbol>(
        node->child(0)->getStringvalue(),
        Symbol(node->child(0)->getStringvalue(), node->type(), node->getLine())));
}

void inferGlobals(Node* node) {
    /* infer each function */
    if (node && (node->kind() == NODE_TOPLEVEL_LIST)) {
        /* infer child 0 based on its kind */
        if (node->child(0)->kind() == NODE_CONST) {
            inferGlobal(node->child(0), true);
        } else if (node->child(0)->kind() == NODE_GLOBAL) {
            inferGlobal(node->child(0));
        }

        /* recursively infer any other functions */
        inferGlobals(node->child(1));
    }
}

/* add stl functions to the list of globals */
void addStls() {
    String stls[] = {"len", "input", "int", "string", "bool", "real", "print"};

    for (unsigned long i = 0; i < sizeof(stls) / sizeof(stls[0]); i++) {
        /* add them to the globals */
        DataType type(TYPE_FUNCTION);

        globals.insert(std::pair<String, Symbol>("len", Symbol(stls[i], &type, 0, true)));
    }
}

/* Given a class node, this function adds the member vars
 * into the class map (Cannot type check yet as this class
 * may have members that are dependent on
 * classes that have not been seen yet.)*/
void addMembers(ClassContext* context, Node* node) {
    /* if this node is for a member var... */
    if (node->kind() == NODE_IDENTIFIER) {
        /* add it */
        context->addMember(Symbol(node->getStringvalue(), node->type(), node->getLine()));
    } else if (node->kind() == NODE_CLASS_PART) {
        /* recursively add other parts */
        addMembers(context, node->child(0));
        if (node->child(1)) {
            addMembers(context, node->child(1));
        }
    }
}

/* Given a class context, removes any init functions from its list of methods
 * and adds them as constructors to the lists of free functions (also adds a
 * default constructor if one does not exist) */
void initSquared(ClassContext context) {
    /* remove the init functions */
    std::map<String, Node*> inits = context.removeInits();

    DataType* type = new DataType(TYPE_CLASS);
    *(type->className) = context.getName();

    /* loop through any inits*/
    for (std::map<String, Node*>::iterator it = inits.begin(); it != inits.end(); it++) {
        /* update the return types to this class's type*/
        (*(it->second->type()->subtypes))[it->second->type()->subtypes->size() - 1] = *type;

        /* rename the functions and insert them */
        functions.insert(std::pair<String, Node*>(
            context.getName() + it->first.substring((it->first).indexOf("(")), it->second));
    }

    /* if there were no inits... */
    if (!inits.size()) {
        /* make a default one and add it! */
        Node* node = new Node(NODE_FUNCTION);
        node->setStringvalue(String(context.getName()));
        node->setDataType(DataType(TYPE_FUNCTION));
        /* add the empty param type */
        node->type()->subtypes->push_back(DataType(TYPE_TUPLE));
        /* add the return type */
        node->type()->subtypes->push_back(*type);
        String key = context.getName() + "()";
        functions.insert(std::pair<String, Node*>(key, node));
    }
}

/* Makes initial pass through top levels to populate
 * class map*/
void initClass(Node* node) {
    /* First, we need to read in the the Classes. */
    if (node && (node->kind() == NODE_TOPLEVEL_LIST)) {
        if (node->child(0)->kind() == NODE_CLASS) {
            /* check for duplicate class name */
            if (classes.count(node->child(0)->getStringvalue())) {
                throw Error("Duplicate class.", node->child(0)->getLine());
            }
            /* check for stl names*/
            if (globals.count(node->child(0)->getStringvalue())) {
                throw Error("Cannot use stl function name for class name.",
                            node->child(0)->getLine());
            }

            /* create new ClassContext */
            ClassContext context(node->child(0)->getStringvalue());

            /* add any class parts */
            context.addMethods(node->child(0)->child(0));
            context.addMembers(node->child(0)->child(0));

            /* remove the init functions from the class' methods
             * and add them as globally available class constructors */
            initSquared(context);

            /* add context to map of class contexts */
            classes[context.getName()] = context;
        }

        /* recursively infer any other classes */
        initClass(node->child(1));
    }
}

/* checks for the existence of any classes which are referred to */
void checkClassTypes(Node* node) {
    /* check return types */
    if (node->kind() == NODE_FUNCTION) {
        /* if the return type is a class, make sure that it exists */
        if ((*(node->type()->subtypes)).back().getKind() == TYPE_CLASS &&
            !classes.count(*((*(node->type()->subtypes)).back().className))) {
            throw Error("Return type does not exist.", node->getLine());
        }

        /* if it has params... */
        if (node->getNumChildren() > 1) {
            /* check those too */
            checkClassTypes(node->child(0));
        }
    } else if (node->kind() == NODE_FORMAL_PARAM_LIST) {
        checkClassTypes(node->child(0));
        /* if there are more params... */
        if (node->child(1)) {
            /* check 'em */
            checkClassTypes(node->child(1));
        }
    } else if (node->kind() == NODE_DECLARATION) {
        if (node->type()->getKind() == TYPE_CLASS && !classes.count(*(node->type()->className))) {
            throw Error("Class '" + *(node->type()->className) + "' does not exist.",
                        node->getLine());
        }
    }
}

void inferFunction(Node* node) {
    /* make sure that this function does not share a name
     * with a global or a class */
    if (globals.count(node->getStringvalue()) || classes.count(node->getStringvalue())) {
        throw Error("Free function cannot share name with global, constant, or class.",
                    node->getLine());
    }

    /* if there is a return (not none) and all paths don't return */
    if ((*(node->type()->subtypes))[1].getKind() != TYPE_NONE && !checkReturns(node)) {
        /* complain */
        throw Error("Function '" + node->getStringvalue() +
                        "' has declared return type, but all paths do not return.",
                    node->getLine());
    }

    /* check that any classes in params/return type exist */
    checkClassTypes(node->child(0));

    /* if there are params...*/
    if (node->getNumChildren() > 1) {
        inferBlock(node->child(1), node);
    } else {
        inferBlock(node->child(0), node);
    }
}

/* infer types for class declaration */
void inferClass(Node* node) {
    if (node->kind() == NODE_CLASS) {
        inferClass(node->child(0));
    } else if (node->kind() == NODE_CLASS_PART) {
        inferClass(node->child(0));
        if (node->child(1)) {
            inferClass(node->child(1));
        }
    } else if (node->kind() == NODE_IDENTIFIER) {
        checkClassTypes(node);
        /* make sure that the identifier doesn't already exist */
        if (globals.count(node->getStringvalue()) || classes.count(node->getStringvalue()) ||
            functions.hasFuncNamed(node->getStringvalue())) {
            throw Error("Member identifier '" + node->getStringvalue() +
                            "' already exists in current scope.",
                        node->getLine());
        }
    } else if (node->kind() == NODE_FUNCTION) {
        /* check that any classes in params/return type exist */
        checkClassTypes(node->child(0));

        /* if there is a return (not none) and all paths don't return */
        if ((*(node->type()->subtypes))[1].getKind() != TYPE_NONE &&
            node->getStringvalue() != String("init") && !checkReturns(node)) {
            /* complain */
            throw Error("Function '" + node->getStringvalue() +
                            "' has declared return type, but all paths do not return.",
                        node->getLine());
        }

        /* if there are params...*/
        if (node->getNumChildren() > 1) {
            inferBlock(node->child(1), node);
        } else {
            inferBlock(node->child(0), node);
        }
    }
}

/* Called after functions are populated to verify that there is
 * a function called main that takes no arguments and returns none*/
/* populates classes, free functions, and globals */
void verifyMain() {
    /* get the main with no arguments */
    const Node* mainNode = functions.getFunctionNode("main()");

    /* If we didn' find it... */
    if (!mainNode) {
        /* complain! */
        throw Error(
            "Program must contain a function called 'main' which takes no "
            "arguments and returns 'none'.");
    }

    /* otherwise, if it returns something...*/
    if ((*(mainNode->type()->subtypes))[1].getKind() != TYPE_NONE) {
        /* complain about that */
        throw Error("Function 'main()' cannot have a return type other than 'none'.",
                    mainNode->getLine());
    }
}

void initTypes(Node* node) {
    /* add all the stl functions to the global list
     * to reserve their names */
    addStls();

    /* add all the classes */
    initClass(node);

    /* add all the free functions */
    functions.build(node);

    /* check for a main() with no args and no return */
    verifyMain();

    /* add and type check/infer all the globals/constants */
    inferGlobals(node);
}

/* this function does type checking/type inference on a parse tree */
void inferTypes(Node* node) {
    /* infer each function */
    if (node && (node->kind() == NODE_TOPLEVEL_LIST)) {
        /* infer child 0 based on its kind */
        if (node->child(0)->kind() == NODE_FUNCTION) {
            inferFunction(node->child(0));
        } else if (node->child(0)->kind() == NODE_CLASS) {
            inferClass(node->child(0));
        }

        /* recursively infer any other functions */
        inferTypes(node->child(1));
    }
}
