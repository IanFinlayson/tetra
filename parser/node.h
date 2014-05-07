/*
 * Node for Tetra.
 */
#ifndef H_TETRA_NODE
#define H_TETRA_NODE

#include "tetra_hdr.h"

#define COMPARE_TYPES(node1, node2) \
        TTR_compare_trees((node1), (node2), TTR_compatible_types)

typedef struct node {
    int ntype;                  /* node type (if, while, etc.) */
    int dtype;                  /* data type (string, int, etc.) */
    int scope;                  /* scope in which the node occurs */
    char *str;                  /* identifier or string value */
    tetra_int d;                /* integer value */
    tetra_float f;              /* float value */
    int lineno;                 /* line number */
    int n_children;             /* number of children */
    int capacity;               /* capacity for child pointers */
    struct node **children;     /* list of pointers to children */
} TTR_Node;

/* A series of access macros.  
 * Hopefully will make any changes less painful. 
 */
#define N_TYPE(n) ((n)->ntype)
#define N_DTYPE(n) ((n)->dtype)
#define N_NCH(n) ((n)->n_children)
#define N_CHILDREN(n) ((n)->children)
#define N_CHILD(n, c) ((n)->children[(c)])
#define N_CAP(n) ((n)->capacity)
#define N_LINE(n) ((n)->lineno)
#define N_SCOPE(n) ((n)->scope)
#define N_INT(n) ((n)->d)
#define N_FLOAT(n) ((n)->f)
#define N_STR(n) ((n)->str)

enum Node_Types {
    /* Expression types */
    N_TYPE,
    N_IDENTIFIER,
    N_EXPR,
    N_BOOL,
    N_STRING,
    N_FLOAT,
    N_INT,
    N_POW,
    N_ADD,
    N_SUB,
    N_POS,
    N_MOD,
    N_NEG,
    N_INV,
    N_MULT,
    N_DIV,
    N_RSHIFT,
    N_LSHIFT,
    N_OR,
    N_AND,
    N_NOT,
    N_LT,
    N_GT,
    N_LTE,
    N_GTE,
    N_EQ,
    N_NEQ,
    N_BOR,
    N_BXOR,
    N_BAND,
    N_CONDITIONAL,
    N_ASSIGN,
    /* Statement types */
    N_STMT,
    N_SMALLSTMT,
    N_PASS,
    N_IF,
    N_ELIFLIST,
    N_ELIF,
    N_ELSE,
    N_WHILESTMT,
    N_WHILE,
    N_FOR,
    N_FORSTMT,
    N_BREAK,
    N_CONTINUE,
    N_PRINT,
    N_FUNC,
    N_FUNCDEF,
    N_PARAMLIST,
    N_POSARGS,
    N_CALL,
    N_GLOBAL,
    N_IDENTLIST,
    N_RETURN,
    N_TGTS 
};

/* 
 * Create a new node on the heap and return a pointer to it.  
 * Returns NULL on error
 */
TTR_Node *TTR_make_node(int type, const char *str, tetra_int d, 
        tetra_float f, int lineno);

/* 
 * Free a node and all its children.  Returns 0 on success, non-0 otherwise.
 */
void TTR_free_node(TTR_Node *node);

/*
 * Add a node as a child.  Returns 0 on success, non-0 otherwise.
 * Neither parent nor child may be null; too much relies on non-null 
 * children.
 */
int TTR_add_child(TTR_Node *parent, TTR_Node *child);

/*
 * Poll the children for their data types, and set the parent type
 * accordingly.  See discussion in tetra_hdr.h for data types and
 * their meaning.
 *
 * Note that this function is best suited to expressions, and not
 * to assignments, since the semantics differ.  (1 + 5.0 should be a 
 * float, but a = 5.0 should be an error when a is an int variable).
 *
 * Returns the type coerced to, which is INVALID_T when a legitimate
 * coercion could not be performed.
 */
int TTR_infer_data_type(TTR_Node *node);

/*
 * Take two types, and get the resulting type of that mixture.
 * type1 should be the type of the "left-hand side", and type2
 * should be the type of the "right-hand side".
 *
 * Note that this should be used with assignments and returns, but
 * not expressions (see TTR_infer_data_type).
 * 
 * Returns the type that type2 should be promoted to.
 */
int TTR_promote_type(int type1, int type2);

/*
 * Compare the types of two nodes.
 * Returns 0 if the two types are compatible (i.e., TTR_promote_type
 * returns anything other than INVALID_T when called with the types of
 * the nodes in order). 
 * Returns 1 otherwise.
 */
int TTR_compatible_types(TTR_Node *node1, TTR_Node *node2);

/*
 * Compare two binary node trees.  The first child of the root is compared,
 * then the first child of the root's second child, etc.
 *
 * comp is assumed to return 0 on a match.
 *
 * Returns 0 when both nodes run out of children to compare (trees match)
 * Returns -1 when node2 has children while node1 doesn't
 * Returns 1 when node1 has children and node2 doesn't
 * Returns whatever comp returns when two nodes don't match
 */
int TTR_compare_trees(TTR_Node *node1, TTR_Node *node2, 
        int (*comp)(TTR_Node *, TTR_Node *));

#endif /* H_TETRA_NODE */

