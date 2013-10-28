/*
 * Node for Tetra.
 */
#ifndef H_TETRA_NODE
#define H_TETRA_NODE

#include "tetra_hdr.h"

typedef struct node {
    int type;
    char *str;
    tetra_int d;
    tetra_float f;
    int lineno;
    int n_children;
    int capacity;
    struct node **children;
} TTR_Node;

/* 
 * Create a new node on the heap and return a pointer to it.  Returns NULL on
 * error
 */
TTR_Node *
TTR_make_node(int type, const char *str, tetra_int d, tetra_float f, 
        int lineno);

/* 
 * Free a node and all its children.  Returns 0 on success, non-0 otherwise.
 */
void
TTR_free_node(TTR_Node *node);

/*
 * Add a node as a child.  Returns 0 on success, non-0 otherwise.
 * Neither parent nor child may be null; too much relies on non-null children.
 */
int
TTR_add_child(TTR_Node *parent, TTR_Node *child);

#endif /* H_TETRA_NODE */
