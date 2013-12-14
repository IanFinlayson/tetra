#include "node.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

TTR_Node * TTR_make_node(int type, const char *str, tetra_int d, 
        tetra_float f, int lineno)
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

void TTR_free_node(TTR_Node *node)
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

int TTR_add_child(TTR_Node *parent, TTR_Node *child)
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

int TTR_infer_data_type(TTR_Node *node)
{
    int p_type, ch_type, i;
    if (N_DTYPE(node) == UNTYPED_T) {
        N_DTYPE(node) = UNDEFINED_T;
    }
    if (N_NCH(node) == 0) {
        return N_DTYPE(node);
    }
    p_type = N_DTYPE(N_CHILD(node, 0));
    for (i = 1; i < N_NCH(node); i++) {
        ch_type = N_DTYPE(N_CHILD(node, i));
        if (ch_type == UNTYPED_T) {
            N_DTYPE(node) = UNTYPED_T;
            return UNTYPED_T;
        }
        if (ch_type == UNDEFINED_T)
            continue;
        if (p_type == UNDEFINED_T) {
            p_type = ch_type;
            continue;
        }
        if (p_type == ch_type)
            continue;
        if ((p_type == FLOAT_T || p_type == INT_T) &&
                 (ch_type == FLOAT_T || ch_type == INT_T))
            p_type = FLOAT_T;
        else
            p_type = INVALID_T;
    }
    N_DTYPE(node) = p_type;
    return p_type;
}

int TTR_promote_type(int type1, int type2)
{
    if (type1 == FLOAT_T && type2 == INT_T)
        return FLOAT_T;
    if (type1 == type2)
        return type1;
    return INVALID_T;
}

int TTR_compatible_types(TTR_Node *node1, TTR_Node *node2)
{
    if (TTR_promote_type(N_DTYPE(node1), N_DTYPE(node2)) == INVALID_T)
        return 1;
    return 0;
}

int TTR_compare_trees(TTR_Node *node1, TTR_Node *node2, 
        int (*comp)(TTR_Node *, TTR_Node *))
{
    int result, nch1, nch2;
    TTR_Node *ch1, *ch2;

    if (node1 == NULL) {
        if (node2 == NULL)
            return 0;
        else
            return -1;
    }
    if (node2 == NULL) {
        return 1;
    }

    nch1 = N_NCH(node1);
    nch2 = N_NCH(node2);

    if (nch1 < nch2)
        return -1;
    if (nch1 > nch2)
        return 1;
    if (nch1 == 0)
        return 0;

    ch1 = N_CHILD(node1, 0);
    ch2 = N_CHILD(node2, 0);

    if ((result = comp(ch1, ch2)))
        return result;
    if (nch1 == 1)
        return result;
    return TTR_compare_trees(N_CHILD(node1, 1), N_CHILD(node2, 1), comp);
}
