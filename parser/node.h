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

/* A series of access macros.  
 * Hopefully will make any changes less painful. 
 */
#define N_TYPE(n) ((n)->type)
#define N_NCH(n) ((n)->n_children)
#define N_CHILDREN(n) ((n)->children)
#define N_CHILD(n, c) ((n)->children[(c)])
#define N_CAP(n) ((n)->capacity)
#define N_LINE(n) ((n)->lineno)
#define N_INT(n) ((n)->d)
#define N_FLOAT(n) ((n)->f)
#define N_STR(n) ((n)->str)

/* 
 * Create a new node on the heap and return a pointer to it.  
 * Returns NULL on error
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
 * Neither parent nor child may be null; too much relies on non-null 
 * children.
 */
int
TTR_add_child(TTR_Node *parent, TTR_Node *child);

#endif /* H_TETRA_NODE */
