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
    MUL_BEC, DIV_BEC, IDV_BEC, PLU_BEC, MIN_BEC, MOD_BEC, POW_BEC,
    RSH_BEC, LSH_BEC, AND_BEC, XOR_BEC, OR_BEC
};
enum Shift_Type { SHIFT_L, SHIFT_R };

#endif /* ifndef H_TETRA_HDR */
