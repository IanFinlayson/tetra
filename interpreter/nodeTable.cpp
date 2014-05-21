/*This program hashes nodes such that we can determine what general actions should be taken given which nodes.
 */

#include <iostream>
#include <map> //may change this to unordered_map
#include "frontend.hpp"
#include <cstdlib>

using namespace std;

/*
 * About the classificaitons:
 * CONDITION denotes nodes which have as their first child some boolean expression which must be evaluated to true or false.
 * CONTROL denotes a node which may change control of the program to a non-adjacant node
 * IMMEDIATE represents a node which itself IS an immediate value or variable whose value can be returned
 * OPERATION refers to a node that requires evaluating and then operating on one or more of its children, eventually returning a value. Note that this differs from an immediate in that the returned value takes additional processing to calculate
 * STRUCTURE represents a node which is used to define the structure of the program. As a rule, their first child contains some value, and their second child contains either a similar STRUCTURE node, or (The last item present in the structure XOR nothing)
 */

enum NodeClassification {

	CONDITION,
	CONTROL,
	IMMEDIATE,
	OPERATION,
	STRUCTURE

};

using namespace std;

class NodeTable {
private:

	map<NodeKind, NodeClassification> nodeChart;

	static NodeTable* instance;
	NodeTable() {
		nodeChart[NODE_FUNCTION_LIST] = STRUCTURE;
		nodeChart[NODE_FUNCTION] = STRUCTURE;
		nodeChart[NODE_FORMAL_PARAM_LIST] = STRUCTURE;
		nodeChart[NODE_FORMAL_PARAM] = IMMEDIATE;
		nodeChart[NODE_STATEMENT] = STRUCTURE;
		nodeChart[NODE_PASS] = STRUCTURE;
		nodeChart[NODE_BREAK] = CONTROL;
		nodeChart[NODE_CONTINUE] = CONTROL;
		nodeChart[NODE_RETURN] = CONTROL;
		nodeChart[NODE_IF] = CONDITION;
		nodeChart[NODE_ELIF] = CONDITION;
		nodeChart[NODE_ELIF_CHAIN] = STRUCTURE;
		nodeChart[NODE_ELIF_CLAUSE] = CONDITION;
		nodeChart[NODE_WHILE] = CONDITION;
		nodeChart[NODE_FOR] = STRUCTURE;
		nodeChart[NODE_PARFOR] = STRUCTURE;
		nodeChart[NODE_PARALLEL] = STRUCTURE;
		nodeChart[NODE_BACKGROUND] = STRUCTURE;
		nodeChart[NODE_LOCK] = STRUCTURE;
		nodeChart[NODE_ASSIGN] = OPERATION;
		nodeChart[NODE_OR] = OPERATION;
		nodeChart[NODE_AND] = OPERATION;
		nodeChart[NODE_LT] = OPERATION;
		nodeChart[NODE_LTE] = OPERATION;
		nodeChart[NODE_GT] = OPERATION;
		nodeChart[NODE_GTE] = OPERATION;
		nodeChart[NODE_EQ] = OPERATION;
		nodeChart[NODE_NEQ] = OPERATION;
		nodeChart[NODE_BITXOR] = OPERATION;
		nodeChart[NODE_SHIFTL] = OPERATION;
		nodeChart[NODE_SHIFTR] = OPERATION;
		nodeChart[NODE_PLUS] = OPERATION;
		nodeChart[NODE_MINUS] = OPERATION;
		nodeChart[NODE_TIMES] = OPERATION;
		nodeChart[NODE_DIVIDE] = OPERATION;
		nodeChart[NODE_MODULUS] = OPERATION;
		nodeChart[NODE_EXP] = OPERATION;
		nodeChart[NODE_NOT] = OPERATION;
		nodeChart[NODE_BITNOT] = OPERATION;
		nodeChart[NODE_VECREF] = STRUCTURE;
		nodeChart[NODE_INDEX] = STRUCTURE;
		nodeChart[NODE_FUNCALL] = CONTROL;
		nodeChart[NODE_ACTUAL_PARAM_LIST] = STRUCTURE;
		nodeChart[NODE_INTVAL] = IMMEDIATE;
		nodeChart[NODE_REALVAL] = IMMEDIATE;
		nodeChart[NODE_BOOLVAL] = IMMEDIATE;
		nodeChart[NODE_STRINGVAL] = IMMEDIATE;
		nodeChart[NODE_IDENTIFIER] = IMMEDIATE;
		nodeChart[NODE_VECVAL] = STRUCTURE;
	}

	NodeClassification getClassification(Node* node) {
		//this is just present to check that every node is mapped
		if(nodeChart.find(node->kind()) == nodeChart.end()) {
			cout << "Node not found in classification table: " << node->kind() << "\nPlease look into this. Aborting." << endl;
			exit(EXIT_FAILURE);
		}
		return nodeChart[node->kind()];
	}

public:
	static void initialize() {
		instance = new NodeTable();
	}

	//assumes it has been initialized
	static NodeClassification classifyNode(Node* node) {
		return instance->getClassification(node);
	}

};



