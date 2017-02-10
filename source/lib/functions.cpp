/* functions.cpp
 * this file builds a function lookup table so that when the interpreter
 * encounters a function call, it can easily find the address of the
 * appropriate node where the called function code resides since there is only
 * one funciton table per program (even if using multiple files, the further
 * functions should be addable by calling buildtree for each file's syntax
 * tree) this uses a single object design.
 */

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "tetra.h"

/* given a function signature, returns the adress of a node containing the
   function definition for that signature */
Node* FunctionMap::getFunctionNode(const String functionSignature) {
    std::cout << "Looking for function '" << functionSignature << "' in:\n";
    for (auto it = lookup.cbegin(); it != lookup.cend(); ++it) {
            std::cout << "\t" << it->first << " : " << it->second << "\n";
    }
    std::cout << "\n\n";

    /* if function is not there, will return default Node* (i.e. NULL) */
    return lookup[functionSignature];
}

Node* FunctionMap::getFunctionNode(DataType* params, String name) {
    return lookup[name + typeToString(params)];
}

/* lookup function node from functionCall node */
Node* FunctionMap::getFunctionNode(const Node* callNode) {
    return lookup[getFunctionSignature(callNode)];
}

FunctionMap::FunctionMap() {}

/* Fills the function map given the specified base node TODO: this function is
   comprised of old functionality followed by new changes to accomadate constant
   time lookup. This function can potentially bwe rewritten to both search for
   functions and assign them values at the same time */
void FunctionMap::build(Node* tree) {
    if (tree->kind() == NODE_TOPLEVEL_LIST || tree->kind() == NODE_CLASS_PART) {
        /* by frontend specifications, there MUST be a child to add */
        Node* candidate = tree->child(0);
        if (candidate->kind() == NODE_FUNCTION) {
            /* populate the symtable and update the datatype */
            inferParams(candidate);

            /* if this function is already in the table */
            if (lookup.count(getFunctionSignature(candidate)) > 0) {
                throw Error("Duplicate function. ", candidate->getLine());
            }

            /* otherwise, it's not in the table, so add it! */
            lookup[getFunctionSignature(candidate)] = candidate;
        }
        /* checks if there are further functions to add */
        if (tree->child(1) != NULL) {
            build(tree->child(1));
        }
    }
}

/* wrapper around std::map's insert function */
void FunctionMap::insert(std::pair<String, Node*> pair) {
    lookup.insert(pair);
}

/* clear all functions from the map */
void FunctionMap::clearAll() {
    lookup.clear();
}

std::map<String, Node*> FunctionMap::remove(String name) {
    /* make a map to store the pairs to return */
    std::map<String, Node*> pairs;

    /* make a vector to store keys to remove */
    std::vector<String> keys;

    /* find the functions */
    for (std::map<String, Node*>::iterator it = lookup.begin(); it != lookup.end(); it++) {
        /* check for a name match */
        if (name == it->first.substring(0, (it->first).indexOf("("))) {
            /* add it to the return list */
            pairs.insert(*it);

            /* add the old key to a list for removal */
            keys.push_back(it->first);
        }
    }

    /* remove all the old keys */
    for (unsigned long i = 0; i < keys.size(); i++) {
        lookup.erase(keys[i]);
    }

    return pairs;
}

bool FunctionMap::hasFuncNamed(String name) {
    /* loop through all elements in the map */
    for (std::map<String, Node*>::iterator it = lookup.begin(); it != lookup.end(); it++) {
        /* check for a name match */
        if (name == it->first.substring(0, (it->first).indexOf("("))) {
            return true;
        }
    }
    /* if we got through the list and didn't find a name match */
    return false;
}

DataType FunctionMap::getFunctionsNamed(String name) {
    /* create a dataType to return */
    DataType retType = DataType(TYPE_OVERLOAD);

    /* loop through all elements in the map */
    for (std::map<String, Node*>::iterator it = lookup.begin(); it != lookup.end(); it++) {
        /* check for a name match */
        if (name == it->first.substring(0, (it->first).indexOf("("))) {
            /* if one was found, add it to the subtypes */
            retType.subtypes->push_back(DataType(*(it->second->type())));
        }
    }
    /* If there are no subtypes, return null */
    if (retType.subtypes->size() == 0) {
        retType = DataType(TYPE_NONE);
        /* If there is one, just return that one */
    } else if (retType.subtypes->size() == 1) {
        retType = (*(retType.subtypes))[0];
    }
    return retType;
}

bool FunctionMap::hasFunction(Node* node) {
    return lookup.count(getFunctionSignature(node));
}

bool FunctionMap::hasFunction(DataType* type, String name) {
    return lookup.count(name + typeToString(type));
}

/* Given a NODE_FUNCTION (seen by the build method) or NODE_FUNCALL (seen at
 * runtime) Assembles the function signature for the function */
String FunctionMap::getFunctionSignature(const Node* node) {
    if (node->kind() == NODE_FUNCTION) {
        return getFunctionSignature(node->getStringvalue(),node->type());
    } else if (node->kind() == NODE_FUNCALL) {
        /* get the params */
        DataType* params = new DataType(TYPE_TUPLE);
        if (node->getNumChildren() > 1) {
            buildParamTupleType(params, node->child(1));
        }

        String paramStr = typeToString(params);
        delete params;
        return node->child(0)->getStringvalue() + paramStr;
    }
    throw Error("Cannot create Function signature.");
}

/* given a name and a functiontype, assembles a function signature */
String FunctionMap::getFunctionSignature(const String name, const DataType* type) {
    /* this returns the whole type including return */
    String signature = name + typeToString(&((*(type->subtypes))[0]));

    /* if there is an arrow in it, remove it */
    int idx = signature.indexOf("->");
    if (idx != -1) {
        signature = signature.substring(0, idx);
    }

    return signature;
}
