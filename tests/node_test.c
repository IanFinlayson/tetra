#include <stdio.h>
#include "../parser/tetra_hdr.h"
#include "../parser/node.h"

enum { BLUE, ALPHA, INT, REAL };
void print_node_test(TTR_Node *node, int indents);

int main(int argc, char **argv)
{
    printf("Don't mind me, just checking: %d %d\n", N_INT, N_FLOAT);

    TTR_Node *root;
    root = TTR_make_node(BLUE, "", 0, 0.0, 1);
    TTR_add_child(root, TTR_make_node(ALPHA, "Word", 0, 0.0, 2));
    TTR_add_child(root, TTR_make_node(INT, "", 34, 0.0, 3));
    TTR_add_child(N_CHILD(root, 0), TTR_make_node(REAL, "", 0, 1.523, 3));
    TTR_add_child(root, TTR_make_node(REAL, "", 0, 1.523, 3));

    print_node_test(root, 0);

    TTR_free_node(root);

    return 0;
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
    for (i = 0; i < N_NCH(node); i++) {
        print_node_test(N_CHILD(node, i), indents + 1);
    }
}
