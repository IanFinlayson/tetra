/*This program sorts nodes such that we can determine what general actions should be taken given which nodes.
 * This is purely a convenience to the programmer of the interpreter, such that different nodes can be handled in different parts of the program. Theoretically, the interpreter could be made to work without this class qith just minor structual modifications
 */

#ifndef NODE_TABLE_H
#define NODE_TABLE_H

#include <iostream>
#include <map> //may change this to unordered_map
#include "frontend.hpp"

using std::string;

/*
 * About the classificaitons:
 * CONDITION denotes nodes which will always evaluate to a boolean expression regardless of its children
 * CONTROL denotes a node which may change control of the program to a non-adjacant node
 * IMMEDIATE represents a node which itself IS an immediate value or variable whose value can be returned
 * OPERATION refers to a node that requires evaluating and then operating on one or more of its children, eventually returning a value. Note that this differs from an immediate in that the returned value takes additional processing to calculate
 * STRUCTURE represents a node which is used to define the structure of the program.
 * ASSIGNMENT denotes an assignment operator. This turns out to be a somewhat special case, so we give it its own classification
 * FLAG Denotes a break, continue, or return. These statements notify the program that something exceptional has occurred, and that the program should undertake some aciton
 */

//The enumerations (as well as what enumeration goes with which node)  are subject to change until we wade further into the types of nodes we'll need
enum NodeClassification {

	CONDITION,
	CONTROL,
	IMMEDIATE,
	OPERATION,
	STRUCTURE,
	ASSIGNMENT,
	FLAG

};

class NodeTable {
private:

	std::map<NodeKind, NodeClassification> nodeChart;

	static NodeTable instance;

	//constructor populates the table
	NodeTable();	
	
	//Returns the node's classification by checking its NodeType
	const NodeClassification getClassification(const Node* node);

public:

	//There is only one NodeTable, and it never changes, This is all the interface we need
	static const NodeClassification classifyNode(const Node* node);

};

#endif
