/*This program hashes nodes such that we can determine what general actions should be taken given which nodes.
*/

#include <iostream>
#include <map> //may change this to unordered_map
#include "frontend.hpp"
#include "nodeTable.h"
#include <cstdlib> //used for exit
//#define NDEBUG
#include <assert.h>
using std::string;

/*
 * About the classificaitons:
 * CONDITION denotes nodes which will always evaluate to a boolean expression regardless of its children
 * CONTROL denotes a node which may change control of the program to a non-adjacant node
 * IMMEDIATE represents a node which itself IS an immediate value or variable whose value can be returned
 * OPERATION refers to a node that requires evaluating and then operating on one or more of its children, eventually returning a value. Note that this differs from an immediate in that the returned value takes additional processing to calculate
 * STRUCTURE represents a node which is used to define the structure of the program.
 * ASSIGNMENT denotes an assignment operator. This turns out to be a somewhat special case, so we give it its own classification
 */

//Constructor gives each NodeKind an appropriate classification in the table
NodeTable::NodeTable() {
	nodeChart[NODE_FUNCTION_LIST] = CLASS_STRUCTURE;
	nodeChart[NODE_FUNCTION] = CLASS_STRUCTURE;
	nodeChart[NODE_FORMAL_PARAM_LIST] = CLASS_STRUCTURE;
	nodeChart[NODE_FORMAL_PARAM] = CLASS_IMMEDIATE;
	nodeChart[NODE_STATEMENT] = CLASS_STRUCTURE;
	nodeChart[NODE_PASS] = CLASS_STRUCTURE;
	nodeChart[NODE_BREAK] = CLASS_FLAG;
	nodeChart[NODE_CONTINUE] = CLASS_FLAG;
	nodeChart[NODE_RETURN] = CLASS_FLAG;
	nodeChart[NODE_IF] = CLASS_STRUCTURE;
	nodeChart[NODE_ELIF] = CLASS_STRUCTURE;
	nodeChart[NODE_ELIF_CHAIN] = CLASS_STRUCTURE;
	nodeChart[NODE_ELIF_CLAUSE] = CLASS_STRUCTURE;
	nodeChart[NODE_WHILE] = CLASS_STRUCTURE;
	nodeChart[NODE_FOR] = CLASS_STRUCTURE;
	nodeChart[NODE_PARFOR] = CLASS_PARALLEL;
	nodeChart[NODE_PARALLEL] = CLASS_PARALLEL;
	nodeChart[NODE_BACKGROUND] = CLASS_PARALLEL;
	nodeChart[NODE_LOCK] = CLASS_PARALLEL;
	nodeChart[NODE_ASSIGN] = CLASS_ASSIGNMENT;
	nodeChart[NODE_OR] = CLASS_OPERATION;
	nodeChart[NODE_AND] = CLASS_OPERATION;
	nodeChart[NODE_LT] = CLASS_CONDITION;
	nodeChart[NODE_LTE] = CLASS_CONDITION;
	nodeChart[NODE_GT] = CLASS_CONDITION;
	nodeChart[NODE_GTE] = CLASS_CONDITION;
	nodeChart[NODE_EQ] = CLASS_CONDITION;
	nodeChart[NODE_NEQ] = CLASS_CONDITION;
	nodeChart[NODE_BITAND] = CLASS_OPERATION;
	nodeChart[NODE_BITOR] = CLASS_OPERATION;
	nodeChart[NODE_BITXOR] = CLASS_OPERATION;
	nodeChart[NODE_SHIFTL] = CLASS_OPERATION;
	nodeChart[NODE_SHIFTR] = CLASS_OPERATION;
	nodeChart[NODE_PLUS] = CLASS_OPERATION;
	nodeChart[NODE_MINUS] = CLASS_OPERATION;
	nodeChart[NODE_TIMES] = CLASS_OPERATION;
	nodeChart[NODE_DIVIDE] = CLASS_OPERATION;
	nodeChart[NODE_MODULUS] = CLASS_OPERATION;
	nodeChart[NODE_EXP] = CLASS_OPERATION;
	nodeChart[NODE_NOT] = CLASS_CONDITION;
	nodeChart[NODE_BITNOT] = CLASS_OPERATION;
	nodeChart[NODE_VECREF] = CLASS_STRUCTURE;
	nodeChart[NODE_INDEX] = CLASS_STRUCTURE;
	nodeChart[NODE_FUNCALL] = CLASS_CONTROL;
	nodeChart[NODE_ACTUAL_PARAM_LIST] = CLASS_STRUCTURE;
	nodeChart[NODE_INTVAL] = CLASS_IMMEDIATE;
	nodeChart[NODE_REALVAL] = CLASS_IMMEDIATE;
	nodeChart[NODE_BOOLVAL] = CLASS_IMMEDIATE;
	nodeChart[NODE_STRINGVAL] = CLASS_IMMEDIATE;
	nodeChart[NODE_IDENTIFIER] = CLASS_IMMEDIATE;
	nodeChart[NODE_VECVAL] = CLASS_IMMEDIATE;
}


const NodeClassification NodeTable::getClassification(const Node* node) {
	//this is just present to check that every node is mapped
	assert(nodeChart.find(node->kind()) != nodeChart.end());
/*	if(nodeChart.find(node->kind()) == nodeChart.end()) {
		cout << "Node not found in classification table: " << node->kind() << "\nPlease look into this. Aborting." << endl;
		exit(EXIT_FAILURE);
	}*/
	return nodeChart[node->kind()];
}

//Static function rewturns the classification of the given node
const NodeClassification NodeTable::classifyNode(const Node* node) {
	return instance.getClassification(node);
}
//initializes the static table
NodeTable NodeTable::instance;

