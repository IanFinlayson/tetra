%{
#include <stdio.h>
#include <stdlib.h>
#include "tetra_hdr.h"

extern int yylineno;

void yywrap(void);
void yyerror(const char *msg);
%}

%union {
    intmax_t i;
    long double ld;
    char *text;
    Comp_Type comp;
}

%error-verbose

%%

program: statement-block

expression: conditional-expr

conditional-expr: or-test
    | or-test TOK_IF or-test TOK_ELSE expr

or-test: and-test
    | or-test TOK_OR and-test

and-test: not-test
    | and-test TOK_AND not-test

not-test: comparison
    | TOK_NOT not-test

comparison: or-expr
    | comparison TOK_COMP or-expr

or-expr: xor-expr
    | or-expr '|' xor-expr

xor-expr: and-expr
    | xor-expr '^' and-expr

and-expr: shift-expr
    | and-expr '&' shift-expr

shift-expr: add-expr
    | shift-expr TOK_SHIFT add-expr

add-expr: mult-expr
    | add-expr TOK_ADD mult-expr

mult-expr: unary-expr
    | mult-expr '*' unary-expr
    | mult-expr '/' unary-expr
    | mult-expr '%' unary-expr
    | mult-expr '//' unary-expr

unary-expr: power-expr
    | '-' unary-expr
    | '+' unary-expr
    | '~' unary-expr

power-expr: primary-expr
    | primary-expr TOK_EXP unary-expr

primary: atom

atom: TOK_IDENTIFIER
    | literal
    | enclosure

literal: TOK_BOOL
    | TOK_INT
    | TOK_REAL
    | TOK_STRING

enclosure: '(' expression ')'

%%

void yywrap()
{
    return 1;
}

void yyerror(const char *msg)
{
    fprintf(stderr, "<Line %d> Compiler error: '%s'.\n", yylineno, msg);
}

int main(int argc, char **argv)
{
    return 0;
}
