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

#define N_MAKE_INT(prnt, type, d) \
        (prnt) =  TTR_make_node((type), "", (d), 0.0, yylineno); \
        (prnt)->dtype = INT_T
#define N_MAKE_BOOL(prnt, type, b) \
        (prnt) = TTR_make_node((type), "", (b), 0.0, yylineno); \
        (prnt)->dtype = BOOL_T
#define N_MAKE_FLOAT(prnt, type, f) \
        (prnt) =  TTR_make_node((type), "", 0, (f), yylineno); \
        (prnt)->dtype = FLOAT_T
#define N_MAKE_STR(prnt, type, s) \
        (prnt) =  TTR_make_node((type), (s), 0, 0.0, yylineno); \
        (prnt)->dtype = STRING_T
#define N_MAKE_VOID(prnt, type) \
        (prnt) =  TTR_make_node((type), "", 0, 0.0, yylineno); \
        (prnt)->dtype = VOID_T
#define N_MAKE_INTSTR(prnt, type, d, s) \
        (prnt) = TTR_make_node((type), (str), (d), 0.0, yylineno)
#define N_MAKE_STMT(prnt, type) \
        (prnt) = TTR_make_node((type), "", 0, 0.0, yylineno); \
        (prnt)->dtype = UNTYPED_T
#define N_ADD_CHILD(prnt, child) \
        TTR_add_child((prnt), (child))
#define N_MAKE_UN(prnt, type, ch1) \
        (prnt) =  TTR_make_node((type), "", 0, 0.0, yylineno); \
        TTR_add_child((yyval.node), (ch1)); \
        if (TTR_infer_data_type((prnt)) == INVALID_T) \
            yyerror("Type mismatch")
#define N_MAKE_BIN(prnt, type, ch1, ch2) \
        (prnt) = TTR_make_node((type), "", 0, 0.0, yylineno); \
        TTR_add_child((yyval.node), (ch1)); \
        TTR_add_child((yyval.node), (ch2)); \
        if (TTR_infer_data_type((prnt)) == INVALID_T) \
            yyerror("Type mismatch")
#define N_MAKE_INT_BIN(prnt, type, d, ch1, ch2) \
        (prnt) =  TTR_make_node((type), "", (d), 0.0, yylineno); \
        TTR_add_child((yyval.node), (ch1)); \
        TTR_add_child((yyval.node), (ch2)); \
        if (TTR_infer_data_type((prnt)) == INVALID_T) \
            yyerror("Type mismatch")
#define INFER_DATA_TYPE(prnt) \
        if (TTR_infer_data_type((prnt)) == INVALID_T) \
            yyerror("Type mismatch")
#define SET_IDENT_TYPE(ident, type) \
        TTR_set_ident_data_type(symbol_table, (ident), (type))
#define GET_IDENT_TYPE(node, ident) \
        TTR_Node *n = symbol_table_lookup(symbol_table, (ident)); \
        (node)->dtype = (n == NULL) ? UNDEFINED_T : n->dtype
#define PUSH_SCOPE() \
        symbol_table_enter_next_scope(symbol_table)
#define POP_SCOPE() symbol_table_leave_scope(symbol_table)

int yywrap(void);
void yyerror(const char *msg);

extern int yylineno;
extern Symbol_Table *symbol_table;
extern TTR_Node *parse_tree;
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
%type <node> print-tok
%type <i> return-type type

%error-verbose

%%

program: stmt-list '$' TOK_NEWLINE { parse_tree = $1; }

stmt-list: stmt { $$ = $1; }
    | stmt stmt-list { 
        N_MAKE_STMT($$, N_STMT);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $2);
    }

stmt: simple-stmt { $$ = $1; }
    | top-level-stmt { $$ = $1; }

simple-stmt: small-stmt-list TOK_NEWLINE { $$ = $1; }

small-stmt-list: small-stmt { 
        N_MAKE_STMT($$, N_SMALLSTMT);
        N_ADD_CHILD($$, $1);
    }
    | small-stmt ';' small-stmt-list { 
        N_MAKE_STMT($$, N_SMALLSTMT);
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
    | global-stmt { DEBUG("ss"); $$ = $1; }

suite: simple-stmt { $$ = $1; }
    | TOK_NEWLINE TOK_INDENT stmt-list TOK_DEDENT { $$ = $3; }

if-stmt: if { $$ = $1; }
    | if else { N_MAKE_BIN($$, N_IF, $1, $2); }
    | if elif-list { N_MAKE_BIN($$, N_IF, $1, $2); }
    | if elif-list else { 
        N_MAKE_STMT($$, N_IF);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $2);
        N_ADD_CHILD($$, $3);
    }

if: if-tok expression ':' suite { 
    $$ = $1;
    N_ADD_CHILD($$, $2);
    N_ADD_CHILD($$, $4);
}

if-tok: TOK_IF { N_MAKE_STMT($$, N_IF); }

elif-list: elif { 
        N_MAKE_STMT($$, N_ELIFLIST);
        N_ADD_CHILD($$, $1);
    }
    | elif elif-list { 
        N_MAKE_STMT($$, N_ELIFLIST);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $2);
    }

elif: elif-tok expression ':' suite { 
    $$ = $1;
    N_ADD_CHILD($$, $2);
    N_ADD_CHILD($$, $4);
}

elif-tok: TOK_ELIF { N_MAKE_STMT($$, N_ELIF); }

else: else-tok ':' suite { 
    $$ = $1;
    N_ADD_CHILD($$, $3);
}

else-tok: TOK_ELSE { N_MAKE_STMT( $$, N_ELSE); }

/*
while-stmt: while { $$ = $1; }

while: TOK_WHILE expression ':' suite { N_MAKE_BIN($$, N_WHILE, $2, $4); }
*/
while-stmt: while-tok expression ':' suite  {
    $$ = $1;
    N_ADD_CHILD($$, $2);
    N_ADD_CHILD($$, $4);
}

while-tok: TOK_WHILE { N_MAKE_STMT($$, N_WHILE); }

for-stmt: for { $$ = $1; }
    /* | for else { N_MAKE_BIN($$, N_FORSTMT, $1, $2); } */

for: for-tok target-list TOK_IN expression-list ':' suite { 
    $$ = $1;
    N_ADD_CHILD($$, $2);
    N_ADD_CHILD($$, $4);
    N_ADD_CHILD($$, $6);
}

for-tok: TOK_FOR { N_MAKE_STMT($$, N_FOR); }

target-list: target { 
        N_MAKE_STMT($$, N_TGTS);
        N_ADD_CHILD($$, $1);
    }
    | target-list ',' target { 
        N_MAKE_STMT($$, N_TGTS);
        N_ADD_CHILD($$,$1);
        N_ADD_CHILD($$, $3);
    }

target: identifier { $$ = $1; } 

func-def: def funcname '(' parameter-list ')' return-type 
    ':' { PUSH_SCOPE(); } suite { 
    POP_SCOPE();
    $$ = $1;     
    N_ADD_CHILD($$, $2); 
    N_ADD_CHILD($$, $4); 
    N_ADD_CHILD($$, $9); 
    SET_IDENT_TYPE($2, $6);
}

def: TOK_DEF { N_MAKE_STMT($$, N_FUNCDEF); }

funcname: identifier { $$ = $1; }

parameter-list: /* empty */ { N_MAKE_VOID($$, N_PARAMLIST); }
    | identifier { 
        N_MAKE_STMT($$, N_PARAMLIST);
        N_ADD_CHILD($$, $1);
    }
    | parameter-list ',' identifier { 
        N_MAKE_STMT($$, N_PARAMLIST);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
    }

return-type: /* empty */ { $$ = VOID_T; }
    | type { $$ = $1; }

type: TOK_TYPE { $$ = yylval.i; }

expression-stmt: expression-list

expression-list: expression { 
        N_MAKE_STMT($$, N_EXPR);
        N_ADD_CHILD($$, $1);
    }
    | expression ',' expression-list
    | expression ',' expression-list ',' 
        { N_MAKE_BIN($$, N_EXPR, $1, $3); }

pass-stmt: TOK_PASS { N_MAKE_VOID($$, N_PASS); }

return-stmt: TOK_RETURN expression { 
    N_MAKE_STMT($$, N_RETURN);
    N_ADD_CHILD($$, $2);
}

break-stmt: TOK_BREAK { N_MAKE_VOID($$, N_BREAK); }

continue-stmt: TOK_CONTINUE { N_MAKE_VOID($$, N_CONTINUE); }

print-stmt: print-tok expression { 
    $$ = $1;
    N_ADD_CHILD($$, $2);
}

print-tok: TOK_PRINT { N_MAKE_STMT($$, N_PRINT); }

global-stmt: TOK_GLOBAL identifier-list { N_MAKE_UN($$, N_GLOBAL, $2); }

identifier-list: identifier { N_MAKE_UN($$, N_IDENTLIST, $1); }
    | identifier-list ',' identifier 
        { N_MAKE_BIN($$, N_IDENTLIST, $1, $3); }

expression: assignment-expr { $$ = $1; }

assignment-expr: conditional-expr { $$ = $1; }
    | target '=' assignment-expr 
        { 
            SET_IDENT_TYPE($1, N_DTYPE($3));
            N_MAKE_INT_BIN($$, N_ASSIGN, BEC_BEC, $1, $3);
        }
    | target TOK_ASSIGN assignment-expr
        {
            SET_IDENT_TYPE($1, N_DTYPE($3));
            N_MAKE_INT_BIN($$, N_ASSIGN, $2, $1, $3);
        }

conditional-expr: or-test { $$ = $1; }
    | or-test TOK_IF or-test TOK_ELSE expression { 
        N_MAKE_STMT($$, N_CONDITIONAL);
        N_ADD_CHILD($$, $1);
        N_ADD_CHILD($$, $3);
        N_ADD_CHILD($$, $5);
    }

or-test: and-test { $$ = $1; }
    | or-test TOK_OR and-test { N_MAKE_BIN($$, N_OR, $1, $3); }

and-test: not-test { $$ = $1; }
    | and-test TOK_AND not-test { N_MAKE_BIN($$, N_AND, $1, $3); }

not-test: comparison { $$ = $1; }
    | TOK_NOT not-test { N_MAKE_UN($$, N_NOT, $2); }

comparison: or-expr { $$ = $1; }
    | comparison '<' or-expr { N_MAKE_BIN($$, N_LT, $1, $3); }
    | comparison '>' or-expr { N_MAKE_BIN($$, N_GT, $1, $3); }
    | comparison TOK_LTE or-expr { N_MAKE_BIN($$, N_LTE, $1, $3); }
    | comparison TOK_GTE or-expr { N_MAKE_BIN($$, N_GTE, $1, $3); }
    | comparison TOK_EQ or-expr { N_MAKE_BIN($$, N_EQ, $1, $3); }
    | comparison TOK_NEQ or-expr { N_MAKE_BIN($$, N_NEQ, $1, $3); }

or-expr: xor-expr { $$ = $1; }
    | or-expr '|' xor-expr { N_MAKE_BIN($$, N_BOR, $1, $3); }

xor-expr: and-expr { $$ = $1; }
    | xor-expr '^' and-expr { N_MAKE_BIN($$, N_BXOR, $1, $3); }

and-expr: shift-expr { $$ = $1; }
    | and-expr '&' shift-expr { N_MAKE_BIN($$, N_BAND, $1, $3); }

shift-expr: add-expr { $$ = $1; }
    | shift-expr TOK_LSHIFT add-expr { N_MAKE_BIN($$, N_LSHIFT, $1, $3); }
    | shift-expr TOK_RSHIFT add-expr { N_MAKE_BIN($$, N_RSHIFT, $1, $3); }

add-expr: mult-expr { $$ = $1; }
    | add-expr '+' mult-expr { N_MAKE_BIN($$, N_ADD, $1, $3); }
    | add-expr '-' mult-expr { N_MAKE_BIN($$, N_SUB, $1, $3); }

mult-expr: unary-expr { $$ = $1; }
    | mult-expr '*' unary-expr { N_MAKE_BIN($$, N_MULT, $1, $3); }
    | mult-expr '/' unary-expr { N_MAKE_BIN($$, N_DIV, $1, $3); }
    | mult-expr '%' unary-expr { N_MAKE_BIN($$, N_MOD, $1, $3); }

unary-expr: power-expr { $$ = $1; }
    | '+' unary-expr { N_MAKE_UN($$, N_POS, $2); }
    | '-' unary-expr { N_MAKE_UN($$, N_NEG, $2); }
    | '~' unary-expr { N_MAKE_UN($$, N_INV, $2); }

power-expr: primary { $$ = $1; }
    | unary-expr TOK_POW power-expr { N_MAKE_BIN($$, N_POW, $1, $3); }

primary: atom
    | call { $$ = $1; }

atom: callable
    | literal { $$ = $1; }

callable: identifier
    | enclosure { $$ = $1; }

identifier: TOK_IDENTIFIER { 
    N_MAKE_STR($$, N_IDENTIFIER, yylval.text);
    GET_IDENT_TYPE($$, yylval.text);
}

literal: TOK_BOOL { N_MAKE_BOOL($$, N_BOOL, yylval.i); }
    | TOK_INT { N_MAKE_INT($$, N_INT, yylval.i); }
    | TOK_REAL { N_MAKE_FLOAT($$, N_FLOAT, yylval.f); }
    | TOK_STRING { N_MAKE_STR($$, N_STRING, yylval.text); }

enclosure: '(' expression ')' { $$ = $2; }

call: callable '(' ')' { 
    N_MAKE_UN($$, N_CALL, $1); 
    GET_IDENT_TYPE($$, N_STR($1));
}
    | callable '(' argument-list ')' { N_MAKE_BIN($$, N_CALL, $1, $3); }

argument-list: positional-arguments { $$ = $1; }

positional-arguments: expression { N_MAKE_UN($$, N_POSARGS, $1); }
    | expression ',' positional-arguments
        { N_MAKE_BIN($$, N_POSARGS, $1, $3); }

%%

int yywrap()
{
    return 1;
}

void yyerror(const char *msg)
{
    fprintf(stderr, "<Line %d> Parser error: '%s'.\n", yylineno, msg);
}

/*
int main(int argc, char **argv)
{
    yyparse();
    return 0;
}
*/
