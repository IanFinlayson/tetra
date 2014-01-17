/*
 * Parser for Tetra.
 *
 * Putting the line number in statements does not accurately reflect line
 * numbers, since they don't have a token to bite onto.
 *
 * "List" rules may be right-recursive for easier building of
 * a parse tree.  Once things are hammered out, this may change for the
 * better.
 *
 * Text values (for TOK_STRING and TOK_IDENT) are passed back
 * from the lexer as a pointer pointing to yytext.  Because of this,
 * text values must be extracted on this side of the parser in their own
 * rule.  For example, one must call identifier-atom or some similar
 * non-terminal to extract the value of yylval.text (which is the same as
 * yytext) before more text is read by the lexer.  Behavior otherwise will
 * certainly not be as intended.
 *
 * All lexemes that are always a single character (i.e., '+',
 * '-', etc., and not identifiers) are passed directly as that character.
 * All lexemes that can be multi-character ("==", "<<", identifiers, etc.)
 * are passed as tokens.  Is this awkward?  You betcha!  But the re-use
 * of many characters (notably *, +, and -), as well as the conflicts that
 * arise when a system of pure character passing for mult-character lexemes,
 * seem to make this a decent solution for now.
 *
 * Function calls do not allow for keyword arguments.
 */


%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tetra_hdr.h"
#include "node.h"
#include "symbol_table.h"
#include "dlinked_list.h"

#define FUNC_PARAM_CH 0

#define N_MAKE_INT(prnt, type, d) \
        (prnt) =  TTR_make_node((type), "", (d), 0.0, yylineno); \
        (prnt)->dtype = INT_T; \
        (prnt)->scope = symbol_table_current_scope(symbol_table)
#define N_MAKE_BOOL(prnt, type, b) \
        (prnt) = TTR_make_node((type), "", (b), 0.0, yylineno); \
        (prnt)->dtype = BOOL_T; \
        (prnt)->scope = symbol_table_current_scope(symbol_table)
#define N_MAKE_FLOAT(prnt, type, f) \
        (prnt) =  TTR_make_node((type), "", 0, (f), yylineno); \
        (prnt)->dtype = FLOAT_T; \
        (prnt)->scope = symbol_table_current_scope(symbol_table)
#define N_MAKE_STR(prnt, type, s) \
        (prnt) =  TTR_make_node((type), (s), 0, 0.0, yylineno); \
        (prnt)->dtype = STRING_T; \
        (prnt)->scope = symbol_table_current_scope(symbol_table)
#define N_MAKE_VOID(prnt, type) \
        (prnt) =  TTR_make_node((type), "", 0, 0.0, yylineno); \
        (prnt)->dtype = VOID_T; \
        (prnt)->scope = symbol_table_current_scope(symbol_table)
#define N_MAKE_NODE(prnt, type) \
        (prnt) = TTR_make_node((type), "", 0, 0.0, yylineno); \
        (prnt)->dtype = UNTYPED_T; \
        (prnt)->scope = symbol_table_current_scope(symbol_table)
#define N_ADD_CHILD(prnt, child) \
        TTR_add_child((prnt), (child))
#define INFER_DTYPE(prnt) \
        expr_type = TTR_infer_data_type((prnt)); \
        if (expr_type == INVALID_T) \
            yyerror("Type mismatch"); \
        else if (expr_type == UNDEFINED_T) \
            dll_push_tail(var_patch_list, (prnt))
#define SET_IDENT_DTYPE(ident, type) \
        TTR_set_ident_data_type(symbol_table, (ident), (type))
#define GET_IDENT_DTYPE(node, ident) \
        TTR_Node *n = symbol_table_lookup(symbol_table, (ident)); \
        (node)->dtype = (n == NULL) ? UNDEFINED_T : n->dtype
#define SET_FUNC_DTYPE(func, type) \
        TTR_set_ident_data_type(symbol_table, (func), (type))
#define CHECK_TYPES(type1, type2) \
        if (TTR_promote_type((type1), (type2)) == INVALID_T) \
            yyerror("Type mismatch")

int yywrap(void);
void yyerror(const char *msg);

extern int yylineno;
extern Symbol_Table *symbol_table;
extern TTR_Node *parse_tree;
extern TTR_List *call_patch_list;
extern TTR_List *var_patch_list;

int func_type; /* Used when checking validity of return statements */
TTR_Node *function; /* Used in "call" to check param types */
int expr_type; /* Used when inferring types */
%}

%union {
    tetra_float f;
    tetra_int i;
    char *text;
    struct node *node;
}

%token <text> TOK_IDENTIFIER TOK_STRING
%token <i> TOK_BOOL TOK_INT TOK_ASSIGN TOK_TYPE
%token <f> TOK_REAL
%token TOK_OR TOK_AND TOK_NOT TOK_RSHIFT TOK_LSHIFT
%token TOK_LTE TOK_GTE TOK_EQ TOK_NEQ
%token TOK_INDENT TOK_DEDENT TOK_NEWLINE
%token TOK_INT_DIV TOK_POW
%token TOK_IF TOK_ELIF TOK_ELSE
%token TOK_WHILE  TOK_FOR TOK_IN TOK_BREAK TOK_CONTINUE
%token TOK_FORALL TOK_PARALLEL
%token TOK_DEF TOK_GLOBAL
%token TOK_PASS TOK_RETURN TOK_PRINT

/* You can call nonterminals.awk here to generate a type list for all
 * non-terminals (with ":r !awk -f nonterminals.awk parser.y").
 * I suggest re-generating after any changes to the rules section.
 * This assumes that all non-terminals are of the type of node.
 * Types return-type and type must be hand-done, though, for they are ints.
 */
%type <node> program stmt-list stmt simple-stmt small-stmt-list
%type <node> compound-stmt suite if-stmt if elif-list elif else
%type <node> while-stmt for-stmt for target-list target func-def
%type <node> funcname parameter-list small-stmt
%type <node> expression-stmt expression-list pass-stmt return-stmt
%type <node> break-stmt continue-stmt global-stmt identifier-list
%type <node> expression assignment-expr identifier conditional-expr
%type <node> or-test and-test not-test comparison or-expr xor-expr
%type <node> and-expr shift-expr add-expr mult-expr unary-expr power-expr
%type <node> primary atom literal enclosure call argument-list
%type <node> positional-arguments print-stmt def while-tok for-tok
%type <node> if-tok else-tok elif-tok top-level-stmt callable
%type <node> return-tok parameters
%type <node> print-tok global-tok
%type <i> return-type type

%error-verbose

%%

program: stmt-list '$' TOK_NEWLINE { parse_tree = $1; }

stmt-list: stmt { $$ = $1; }
    | stmt stmt-list {
        N_MAKE_NODE($$, N_STMT);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $2);
    }

stmt: simple-stmt { $$ = $1; }
    | top-level-stmt { $$ = $1; }

simple-stmt: small-stmt-list TOK_NEWLINE { $$ = $1; }

small-stmt-list: small-stmt {
        N_MAKE_NODE($$, N_SMALLSTMT);
        N_ADD_CHILD($$, $1);
    }
    | small-stmt ';' small-stmt-list {
        N_MAKE_NODE($$, N_SMALLSTMT);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }


top-level-stmt: func-def
    | compound-stmt { $$ = $1; }

compound-stmt: if-stmt
    | while-stmt
    | for-stmt { $$ = $1; }

small-stmt: expression-stmt
    | pass-stmt
    | return-stmt
    | break-stmt
    | continue-stmt
    | print-stmt
    | global-stmt { $$ = $1; }

suite: simple-stmt { $$ = $1; }
    | TOK_NEWLINE TOK_INDENT stmt-list TOK_DEDENT { $$ = $3; }

if-stmt: if { $$ = $1; }
    | if else {
        N_MAKE_NODE($$, N_IF);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $2);
    }
    | if elif-list {
        N_MAKE_NODE($$, N_IF);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $2);
    }
    | if elif-list else {
        N_MAKE_NODE($$, N_IF);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $2);
        N_ADD_CHILD($$, $3);
    }

if: if-tok expression ':' suite {
        $$ = $1;
        N_ADD_CHILD($$, $2);
        N_ADD_CHILD($$, $4);
    }

if-tok: TOK_IF { N_MAKE_NODE($$, N_IF); }

elif-list: elif {
        N_MAKE_NODE($$, N_ELIFLIST);
        N_ADD_CHILD($$, $1);
    }
    | elif elif-list {
        N_MAKE_NODE($$, N_ELIFLIST);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $2);
    }

elif: elif-tok expression ':' suite {
        $$ = $1;
        N_ADD_CHILD($$, $2);
        N_ADD_CHILD($$, $4);
    }

elif-tok: TOK_ELIF { N_MAKE_NODE($$, N_ELIF); }

else: else-tok ':' suite {
        $$ = $1;
        N_ADD_CHILD($$, $3);
    }

else-tok: TOK_ELSE { N_MAKE_NODE( $$, N_ELSE); }

while-stmt: while-tok expression ':' suite  {
        $$ = $1;
        N_ADD_CHILD($$, $2);
        N_ADD_CHILD($$, $4);
    }

while-tok: TOK_WHILE { N_MAKE_NODE($$, N_WHILE); }

for-stmt: for { $$ = $1; }

for: for-tok target-list TOK_IN expression-list ':' suite {
        $$ = $1;
        N_ADD_CHILD($$, $2);
        N_ADD_CHILD($$, $4);
        N_ADD_CHILD($$, $6);
    }

for-tok: TOK_FOR { N_MAKE_NODE($$, N_FOR); }

target-list: target {
        N_MAKE_NODE($$, N_TGTS);
        N_ADD_CHILD($$, $1);
    }
    | target-list ',' target {
        N_MAKE_NODE($$, N_TGTS);
        N_ADD_CHILD($$,$1);
        N_ADD_CHILD($$, $3);
    }

target: identifier { $$ = $1; }

func-def: def funcname { PUSH_SCOPE(); } parameters return-type[ret]
        ':' { func_type = $ret; } suite {
        POP_SCOPE();
        $$ = $def;
        N_STR($$) = N_STR($funcname);
        //N_ADD_CHILD($$, $funcname);
        N_ADD_CHILD($$, $parameters);
        N_ADD_CHILD($$, $suite);
        SET_FUNC_DTYPE($$, $ret);
    }

def: TOK_DEF { N_MAKE_NODE($$, N_FUNCDEF); }

funcname: identifier { $$ = $1; }

parameters: '(' ')' { N_MAKE_NODE($$, N_PARAMLIST); }
    | '(' parameter-list[params] ')' { $$ = $params; }

parameter-list: identifier type {
        N_MAKE_NODE($$, N_PARAMLIST);
        N_ADD_CHILD($$, $identifier);
        SET_IDENT_DTYPE($identifier, $type);
    }
    | identifier type ',' parameter-list[plist] {
        N_MAKE_NODE($$, N_PARAMLIST);
        N_ADD_CHILD($$, $identifier);
        N_ADD_CHILD($$, $plist);
        SET_IDENT_DTYPE($identifier, $type);
    }

return-type: /* empty */ { $$ = VOID_T; }
    | type { $$ = $1; }

type: TOK_TYPE { $$ = yylval.i; }

expression-stmt: expression-list { $$ = $1; }

expression-list: expression {
        N_MAKE_NODE($$, N_EXPR);
        N_ADD_CHILD($$, $1);
    }
    | expression ',' expression-list
    | expression ',' expression-list ',' {
        N_MAKE_NODE($$, N_EXPR);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }

pass-stmt: TOK_PASS { N_MAKE_VOID($$, N_PASS); }

return-stmt: return-tok expression {
        CHECK_TYPES(N_DTYPE($expression), func_type);
        $$ = $[return-tok];
        N_ADD_CHILD($$, $expression);
    }

return-tok: TOK_RETURN { N_MAKE_NODE($$, N_RETURN); }

break-stmt: TOK_BREAK { N_MAKE_VOID($$, N_BREAK); }

continue-stmt: TOK_CONTINUE { N_MAKE_VOID($$, N_CONTINUE); }

print-stmt: print-tok expression {
        $$ = $1;
        N_ADD_CHILD($$, $2);
    }

print-tok: TOK_PRINT { N_MAKE_NODE($$, N_PRINT); }

global-stmt: global-tok identifier-list { $$ = $1; N_ADD_CHILD($$, $2); }

global-tok: TOK_GLOBAL { N_MAKE_NODE($$, N_GLOBAL); }

identifier-list: identifier {
        N_MAKE_NODE($$, N_IDENTLIST);
        N_ADD_CHILD($$, $1);
    }
    | identifier-list ',' identifier {
        N_MAKE_NODE($$, N_IDENTLIST);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }

expression: assignment-expr { $$ = $1; }

assignment-expr: conditional-expr { $$ = $1; }
    | target '=' assignment-expr {
        SET_IDENT_DTYPE($1, N_DTYPE($3));
        N_MAKE_INT($$, N_ASSIGN, BEC_BEC);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }
    | target TOK_ASSIGN assignment-expr {
        SET_IDENT_DTYPE($1, N_DTYPE($3));
        N_MAKE_INT($$, N_ASSIGN, $2);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }

conditional-expr: or-test { $$ = $1; }
    | or-test TOK_IF or-test TOK_ELSE expression {
        N_MAKE_NODE($$, N_CONDITIONAL);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        N_ADD_CHILD($$, $5);
        INFER_DTYPE($$);
    }

or-test: and-test { $$ = $1; }
    | or-test TOK_OR and-test {
        N_MAKE_NODE($$, N_OR);
        N_DTYPE($$) = BOOL_T;
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }

and-test: not-test { $$ = $1; }
    | and-test TOK_AND not-test {
        N_MAKE_NODE($$, N_AND);
        N_DTYPE($$) = BOOL_T;
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }

not-test: comparison { $$ = $1; }
    | TOK_NOT not-test {
        N_MAKE_NODE($$, N_NOT);
        N_DTYPE($$) = BOOL_T;
        N_ADD_CHILD($$, $2);
    }

comparison: or-expr { $$ = $1; }
    | comparison '<' or-expr {
        N_MAKE_NODE($$, N_LT);
        N_DTYPE($$) = BOOL_T;
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }
    | comparison '>' or-expr {
        N_MAKE_NODE($$, N_GT);
        N_DTYPE($$) = BOOL_T;
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }
    | comparison TOK_LTE or-expr {
        N_MAKE_NODE($$, N_LTE);
        N_DTYPE($$) = BOOL_T;
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }
    | comparison TOK_GTE or-expr {
        N_MAKE_NODE($$, N_GTE);
        N_DTYPE($$) = BOOL_T;
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }
    | comparison TOK_EQ or-expr {
        N_MAKE_NODE($$, N_EQ);
        N_DTYPE($$) = BOOL_T;
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }
    | comparison TOK_NEQ or-expr {
        N_MAKE_NODE($$, N_NEQ);
        N_DTYPE($$) = BOOL_T;
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }

or-expr: xor-expr { $$ = $1; }
    | or-expr '|' xor-expr {
        N_MAKE_NODE($$, N_BOR);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }

xor-expr: and-expr { $$ = $1; }
    | xor-expr '^' and-expr {
        N_MAKE_NODE($$, N_BXOR);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }

and-expr: shift-expr { $$ = $1; }
    | and-expr '&' shift-expr {
        N_MAKE_NODE($$, N_BAND);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }

shift-expr: add-expr { $$ = $1; }
    | shift-expr TOK_LSHIFT add-expr {
        N_MAKE_NODE($$, N_LSHIFT);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }
    | shift-expr TOK_RSHIFT add-expr {
        N_MAKE_NODE($$, N_RSHIFT);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }

add-expr: mult-expr { $$ = $1; }
    | add-expr '+' mult-expr {
        N_MAKE_NODE($$, N_ADD);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }
    | add-expr '-' mult-expr {
        N_MAKE_NODE($$, N_SUB);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }

mult-expr: unary-expr { $$ = $1; }
    | mult-expr '*' unary-expr {
        N_MAKE_NODE($$, N_MULT);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }
    | mult-expr '/' unary-expr {
        N_MAKE_NODE($$, N_DIV);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }
    | mult-expr '%' unary-expr {
        N_MAKE_NODE($$, N_MOD);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }

unary-expr: power-expr { $$ = $1; }
    | '+' unary-expr {
        N_MAKE_NODE($$, N_POS);
        N_ADD_CHILD($$, $2);
        INFER_DTYPE($$);
    }
    | '-' unary-expr {
        N_MAKE_NODE($$, N_NEG);
        N_ADD_CHILD($$, $2);
        INFER_DTYPE($$);
    }
    | '~' unary-expr {
        N_MAKE_NODE($$, N_INV);
        N_ADD_CHILD($$, $2);
        INFER_DTYPE($$);
    }

power-expr: primary { $$ = $1; }
    | unary-expr TOK_POW power-expr {
        N_MAKE_NODE($$, N_POW);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        INFER_DTYPE($$);
    }

primary: atom
    | call { $$ = $1; }

atom: callable
    | literal
    | enclosure { $$ = $1; }

callable: identifier { $$ = $1; }

identifier: TOK_IDENTIFIER {
        N_MAKE_STR($$, N_IDENTIFIER, yylval.text);
        GET_IDENT_DTYPE($$, yylval.text);
    }

literal: TOK_BOOL { N_MAKE_BOOL($$, N_BOOL, yylval.i); }
    | TOK_INT { N_MAKE_INT($$, N_INT, yylval.i); }
    | TOK_REAL { N_MAKE_FLOAT($$, N_FLOAT, yylval.f); }
    | TOK_STRING { N_MAKE_STR($$, N_STRING, yylval.text); }

enclosure: '(' expression ')' { $$ = $expression; }

call: callable argument-list[args] {
        N_MAKE_NODE($$, N_CALL);
        //N_ADD_CHILD($$, $callable);
        N_STR($$) = N_STR($callable);
        N_ADD_CHILD($$, $args);
        function = GET_SYMBOL(N_STR($callable));
        if (function != NULL) {
            N_DTYPE($$) = N_DTYPE(function);
            if(COMPARE_TYPES(N_CHILD(function, FUNC_PARAM_CH), $args))
                yyerror("Argument types do not match parameter types");
        } else {
            N_DTYPE($$) = UNDEFINED_T;
            dll_push_tail(call_patch_list, $$);
        }
    }

argument-list: '(' ')' { $$ = N_MAKE_NODE($$, N_POSARGS); }
    | '(' positional-arguments[args] ')' { $$ = $args; }

positional-arguments: expression {
        N_MAKE_NODE($$, N_POSARGS);
        N_ADD_CHILD($$, $expression);
        INFER_DTYPE($$);
    }
    | expression ',' positional-arguments[args] {
        N_MAKE_NODE($$, N_POSARGS);
        N_ADD_CHILD($$, $expression);
        N_ADD_CHILD($$, $args);
        INFER_DTYPE($$);
    }

%%

int yywrap()
{
    return 1;
}

void yyerror(const char *msg)
{
    fprintf(stderr, "<Line %d> Parser error: '%s'.\n", yylineno, msg);
}


