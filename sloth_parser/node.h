#ifndef H_NODE
#define H_NODE

#define ID_SIZE 100
#define MAX_CHILDREN 3

struct Node {
    int type;
    double val;
    char id[ID_SIZE];
    int num_children;
    struct Node *children[MAX_CHILDREN];
};

/*
 * Creates a new node with the values given and returns it
 */
struct Node *make_node(int type, double val, char *id);

/*
 * Attaches an existing Node onto a parent
 */
void attach_node(struct Node *parent, struct Node *child);

#endif
