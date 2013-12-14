#include "parsetree.h"

void print_node(TTR_Node *node)
{
    char err_msg[100];

    printf("<Line %d> ", N_LINE(node));

    switch(N_DTYPE(node)) {
    case INVALID_T: printf("INVALID: "); break;
    case UNDEFINED_T: printf("UNDEFINED: "); break;
    case VOID_T: printf("VOID: "); break;
    case INT_T: printf("INT: "); break;
    case FLOAT_T: printf("FLOAT: "); break;
    case BOOL_T: printf("BOOL: "); break;
    case STRING_T: printf("STRING: "); break;
    case UNTYPED_T: printf("UNTYPED: "); break;
    default: printf("What? "); break;
    }

    switch(N_TYPE(node)) {
    case N_TYPE: printf("TYPE: %d", N_INT(node)); break;
    case N_IDENTIFIER: printf("IDENTIFIER: %s", N_STR(node)); break;
    case N_EXPR: printf("EXPR:"); break;
    case N_BOOL: printf("BOOL: %d", N_INT(node)); break;
    case N_STRING: printf("STRING: %s", N_STR(node)); break;
    case N_FLOAT: printf("FLOAT: %f", N_FLOAT(node)); break;
    case N_INT: printf("INT: %d", N_INT(node)); break;
    case N_POW: printf("POW:"); break;
    case N_ADD: printf("ADD:"); break;
    case N_SUB: printf("SUB:"); break;
    case N_POS: printf("POS:"); break;
    case N_MOD: printf("MOD:"); break;
    case N_NEG: printf("NEG:"); break;
    case N_INV: printf("INV:"); break;
    case N_MULT: printf("MULT:"); break;
    case N_DIV: printf("DIV:"); break;
    case N_RSHIFT: printf("RSHIFT:"); break;
    case N_LSHIFT: printf("LSHIFT:"); break;
    case N_OR: printf("OR:"); break;
    case N_AND: printf("AND:"); break;
    case N_NOT: printf("NOT:"); break;
    case N_LT: printf("LT:"); break;
    case N_GT: printf("GT:"); break;
    case N_LTE: printf("LTE:"); break;
    case N_GTE: printf("GTE:"); break;
    case N_EQ: printf("EQ:"); break;
    case N_NEQ: printf("NEQ:"); break;
    case N_BOR: printf("BOR:"); break;
    case N_BXOR: printf("BXOR:"); break;
    case N_BAND: printf("BAND:"); break;
    case N_CONDITIONAL: printf("CONDITIONAL:"); break;
    case N_ASSIGN: printf("ASSIGN: %d", N_INT(node)); break;
    case N_STMT: printf("STMT:"); break;
    case N_SMALLSTMT: printf("SMALLSTMT:"); break;
    case N_PASS: printf("PASS:"); break;
    case N_IF: printf("IF:"); break;
    case N_ELIFLIST: printf("ELIFLIST:"); break;
    case N_ELIF: printf("ELIF:"); break;
    case N_ELSE: printf("ELSE:"); break;
    case N_WHILESTMT: printf("WHILESTMT:"); break;
    case N_WHILE: printf("WHILE:"); break;
    case N_FOR: printf("FOR:"); break;
    case N_FORSTMT: printf("FORSTMT:"); break;
    case N_BREAK: printf("BREAK:"); break;
    case N_CONTINUE: printf("CONTINUE:"); break;
    case N_FUNC: printf("FUNC:"); break;
    case N_FUNCDEF: printf("FUNCDEF:"); break;
    case N_PARAMLIST: printf("PARAMLIST:"); break;
    case N_POSARGS: printf("POSARGS:"); break;
    case N_CALL: printf("CALL:"); break;
    case N_GLOBAL: printf("GLOBAL:"); break;
    case N_IDENTLIST: printf("IDENTLIST:"); break;
    case N_RETURN: printf("RETURN:"); break;
    case N_TGTS: printf("TGTS:"); break;
    case N_PRINT: printf("PRINT: "); break;
    default:
        sprintf(err_msg, "Unknown node type: %d", N_TYPE(node));
        err_exit(err_msg);
    }
}

void print_tree(TTR_Node *root, int indents)
{
    int i;

    if (!root) return;

    for (i = 0; i < indents; i++) {
        printf("  ");
    }

    print_node(root);
    printf("\n");

    for (i = 0; i < N_NCH(root); i++) {
        print_tree(N_CHILD(root, i), indents + 1);
    }
}
