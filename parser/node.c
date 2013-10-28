#include "node.h"
#include <stdlib.h>
#include <string.h>

TTR_Node *
TTR_make_node(int type, const char *str, tetra_int d, tetra_float f,
        int lineno)
{
    TTR_Node *node;
    node = malloc(sizeof(TTR_Node));
    if (node == NULL) /* malloc() error */
        return node;
    node->type = type;
    node->str = strdup(str);
    if (node->str == NULL) { /* strdup() error */
        TTR_free_node(node);
        return NULL;
    }
    node->d = d;
    node->f = f;
    node->lineno = lineno;
    node->n_children = 0;
    node->capacity = 1;
    node->children = malloc(sizeof(TTR_Node *));
    if (node->children == NULL) {
        TTR_free_node(node);
    }
    return node;
}

void
TTR_free_node(TTR_Node *node)
{
    int i;

    if (node == NULL)
        return;
    if (!(node->str == NULL))
        free(node->str);
    if (!(node->children == NULL)) {
        for (i = 0; i < node->n_children; i++)
            TTR_free_node(node->children[i]);
        free(node->children);
    }
}

int
TTR_add_child(TTR_Node *parent, TTR_Node *child)
{
    if (parent == NULL)
        return 1;
    if (child == NULL)
        return 1;
    if (parent->n_children >= parent->capacity) {
        parent->children = realloc(parent->children,
               parent->capacity * sizeof(TTR_Node *) * 2);
        if (parent->children == NULL)
            return 1;
        parent->capacity *= 2;
    }
    parent->children[parent->n_children] = child;
    parent->n_children++;
    return 0;
}
