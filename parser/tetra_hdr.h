/*
 * Header file for stuff necessary to construct a Tetra parse tree.
 */
#ifndef H_TETRA_HDR
#define H_TETRA_HDR

#include <inttypes.h>
#include <stdio.h>
#include "exit_funcs.hpp"

#define DEBUG(msg) fprintf(stderr, "%s\n", (msg))

typedef int32_t tetra_int;
typedef double tetra_float;

#define STR_TO_I(str, end, base) (tetra_int) strtol((str), (end), (base))
/* does not take endptr because flex should filter out invalid literals */
#define STR_TO_F(str) strtod((str), NULL)

#define PUSH_SCOPE() \
        symbol_table_enter_next_scope(symbol_table)
#define POP_SCOPE() symbol_table_leave_scope(symbol_table)
#define ENTER_SCOPE(scope) \
        symbol_table_enter_scope(symbol_table, (scope))
#define SET_GLOBAL_SCOPE() \
        while (symbol_table_current_scope(symbol_table) > 1) \
            symbol_table_leave_scope(symbol_table)
#define GET_SYMBOL(ident) \
        symbol_table_lookup(symbol_table, (ident))
#define ADD_SYMBOL(node) \
        symbol_table_add(symbol_table, (node)->str, (node))
#define CURRENT_SCOPE() (symbol_table->scope_stack[symbol_table->sp-1])

enum Comp_Type { LT, LTE, GT, GTE, EQ, NEQ };
enum Assign_Type { 
    MUL_BEC, DIV_BEC, PLU_BEC, MIN_BEC, MOD_BEC, POW_BEC,
    RSH_BEC, LSH_BEC, AND_BEC, XOR_BEC, OR_BEC, BEC_BEC
};
enum Shift_Type { SHIFT_L, SHIFT_R };

/* 
 * INVALID_T is the result of an attempt to combine incompatible types,
 * such as BOOL_T and INT_T or STRING_T and FLOAT_T.
 * UNTYPED_T is for nodes without types, such as statements.
 * UNDEFINED_T is for nodes that should be typed, but that lack the
 * information for type to be inferred.
 */
enum Data_Type { 
    INVALID_T, UNTYPED_T, UNDEFINED_T, VOID_T, INT_T,
    FLOAT_T, STRING_T, BOOL_T
};

#endif /* ifndef H_TETRA_HDR */
