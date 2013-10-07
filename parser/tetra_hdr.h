/*
 * Header file for stuff necessary to construct a Tetra parse tree.
 */

#ifndef H_TETRA_HDR
#define H_TETRA_HDR

#include <inttypes.h>

typedef intmax_t tetra_int;
typedef long double tetra_float;

enum Comp_Type { LT, LTE, GT, GTE, EQ, NEQ };
enum Assign_Type { 
    BEC, MUL_BEC, DIV_BEC, PLU_BEC, MIN_BEC, MOD_BEC, EXP_BEC
};
enum Shift_Type { SHIFT_L, SHIFT_R };

#endif /* ifndef H_TETRA_HDR */
