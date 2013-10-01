#include <string.h>
#include <stdlib.h>
#include "node.h"

struct Node *make_node(int type, double val, char *id)
{
    int i;
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL) {
        perror("Error allocating memory for node");
        exit(1);
    }
    node->type = type;
    node->val = val;
    strcpy(node->id, id);
    node->num_children = 0;
    for (i = 0; i < MAX_CHILDREN; i++) {
        node->children[i] = NULL;
    }

    return node;
}

void attach_node(struct Node *parent, struct Node *child)
{
    parent->children[parent->num_children++] = child;
    assert(parent->num_children <= MAX_CHILDREN);
}
