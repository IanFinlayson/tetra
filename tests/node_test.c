#include <stdio.h>
#include "../parser/tetra_hdr.h"
#include "../parser/node.h"

enum { BLUE, ALPHA, NUM };
void print_node_test(TTR_Node *node, int indents);

int main(int argc, char **argv)
{
    TTR_Node *root;
    root = TTR_make_node(BLUE, "", 0, 0.0, 1);
    TTR_add_child(root, TTR_make_node(ALPHA, "Word", 0, 0.0, 2));
    TTR_add_child(root, TTR_make_node(NUM, "", 34, 0.0, 3));
    TTR_add_child(root->children[0], TTR_make_node(NUM, "", 0, 1.523, 3));

    print_node_test(root, 0);
}

void print_node_test(TTR_Node *node, int indents)
{
    int i;
    for (i = 0; i < indents; i++)
        printf("  "); 
    printf("Type %d | Str %s | Int %d | Real %Lf | Line %d | Ch %d | ",
            node->type, node->str, node->d, node->f, node->lineno,
            node->n_children);
    printf("Cap %d\n", node->capacity);
    for (i = 0; i < node->n_children; i++)
        print_node_test(node->children[i], indents + 1);
}
