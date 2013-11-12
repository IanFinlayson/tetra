/*
 * Header file for stuff necessary to construct a Tetra parse tree.
 */

#ifndef H_TETRA_HDR
#define H_TETRA_HDR

#include <inttypes.h>
#include "exit_funcs.h"

typedef int32_t tetra_int;
typedef double tetra_float;

#define STR_TO_I(str, end, base) (int32_t) strtol((str), (end), (base))
/* does not take endptr because flex should filter out invalid literals */
#define STR_TO_F(str) strtod((str), NULL)

enum Comp_Type { LT, LTE, GT, GTE, EQ, NEQ };
enum Assign_Type { 
    MUL_BEC, DIV_BEC, PLU_BEC, MIN_BEC, MOD_BEC, POW_BEC,
    RSH_BEC, LSH_BEC, AND_BEC, XOR_BEC, OR_BEC, BEC_BEC
};
enum Shift_Type { SHIFT_L, SHIFT_R };
enum TYPES { INT_T, FLOAT_T, STRING_T, BOOL_T, VOID_T };

#endif /* ifndef H_TETRA_HDR */
