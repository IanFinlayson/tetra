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
    N_TYPE(node) = type;
    N_STR(node) = strdup(str);
    if (N_STR(node) == NULL) { /* strdup() error */
        TTR_free_node(node);
        return NULL;
    }
    N_INT(node) = d;
    N_FLOAT(node) = f;
    N_LINE(node) = lineno;
    N_NCH(node) = 0;
    N_CAP(node) = 1;
    N_CHILDREN(node) = malloc(sizeof(TTR_Node *));
    if (N_CHILDREN(node) == NULL) {
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
    if (!(N_STR(node) == NULL))
        free(N_STR(node));
    if (!(N_CHILDREN(node) == NULL)) {
        for (i = 0; i < N_NCH(node); i++)
            TTR_free_node(N_CHILD(node, i));
        free(N_CHILDREN(node));
    }
    free(node);
}

int
TTR_add_child(TTR_Node *parent, TTR_Node *child)
{
    if (parent == NULL)
        return 1;
    if (child == NULL)
        return 1;
    if (N_NCH(parent) >= N_CAP(parent)) {
        N_CHILDREN(parent) = realloc(N_CHILDREN(parent),
               N_CAP(parent) * sizeof(TTR_Node *) * 2);
        if (N_CHILDREN(parent) == NULL)
            return 1;
        N_CAP(parent) *= 2;
    }
    N_CHILD(parent, N_NCH(parent)) = child;
    N_NCH(parent)++;
    return 0;
}
