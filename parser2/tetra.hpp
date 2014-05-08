#ifndef TETRA_HPP
#define TETRA_HPP

#include <string>
#include <vector>

/* types of nodes */
const int NODE_STMT = 0;


struct Node {
    int type;
    std::vector<Node*> children;
};


/* node functions */
Node* make_node(int type);

void fail(const std::string& mesg, int lineno = 0);

#endif



