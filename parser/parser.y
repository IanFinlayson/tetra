/*
 * Parser for Tetra.
 *
 * Currently, text values (for TOK_STRING and TOK_IDENT) are passed back
 * from the lexer as a pointer pointing to yytext.  Because of this,
 * text values must be extracted on this side of the parser in their own
 * rule.  For example, one must call identifier-atom or some similar
 * non-terminal to extract the value of yylval.text (which is the same as
 * yytext) before more text is read by the lexer.  Behavior otherwise will
 * certainly not be as intended.
 * In the future, this can be fixed by allocating space for yylval.text
 * and using strcpy() on the lexer side.  Research into the max length of
 * yytext and its behavior would be good before doing this, however.
 */


%{
#include <stdio.h>
#include <stdlib.h>
#include "tetra_hdr.h"

int yywrap(void);
void yyerror(const char *msg);

extern int yylineno;
%}

%union {
    tetra_float f;
    tetra_int i;
    char *text;
}

%token <text> TOK_IDENTIFIER TOK_STRING
%token <i> TOK_BOOL TOK_INT
%token <f> TOK_REAL
%token TOK_IF TOK_ELSE TOK_OR TOK_AND TOK_NOT TOK_COMP TOK_SHIFT TOK_ADD
%token TOK_EXP TOK_ASSIGN TOK_INDENT TOK_DEDENT TOK_NEWLINE TOK_FOR
%token TOK_FORALL TOK_WHILE TOK_DEF

%error-verbose

%%

program: expression {
    printf("Program parsed\n"); //DEBUG
}

expression: conditional-expr

conditional-expr: or-test
    | or-test TOK_IF or-test TOK_ELSE expression

or-test: and-test
    | or-test TOK_OR and-test

and-test: not-test
    | and-test TOK_AND not-test

not-test: comparison
    | TOK_NOT not-test

comparison: or-expr
    | comparison '<' or-expr
    | comparison '>' or-expr
    | comparison '<' '=' or-expr
    | comparison '>' '=' or-expr
    | comparison '=' '=' or-expr
    | comparison '!' '=' or-expr
    | comparison '<' '>' or-expr

or-expr: xor-expr
    | or-expr '|' xor-expr

xor-expr: and-expr
    | xor-expr '^' and-expr

and-expr: shift-expr
    | and-expr '&' shift-expr

shift-expr: add-expr
    | shift-expr '>' '>' add-expr
    | shift-expr '<' '<' add-expr

add-expr: mult-expr
    | add-expr '+' mult-expr
    | add-expr '-' mult-expr
    | add-expr '%' mult-expr

mult-expr: unary-expr
    | mult-expr '*' unary-expr
    | mult-expr '/' unary-expr {
        printf("Reg div"); //DEBUG
    }
    | mult-expr '%' unary-expr
    | mult-expr '/' '/' unary-expr { 
        printf("Int div"); //DEBUG
    }

unary-expr: power-expr {
    printf("Power\n"); //DEBUG
}
    | '-' unary-expr
    | '+' unary-expr
    | '~' unary-expr

power-expr: primary {
    printf("Primary\n"); //DEBUG
}
    | primary '*' '*' unary-expr

primary: atom {
    printf("Atom\n"); //DEBUG
}

atom: TOK_IDENTIFIER {
    printf("Identifier\n"); //DEBUG
}
    | literal {
        printf("Literal\n"); //DEBUG
    }
    | enclosure

literal: TOK_BOOL
    | TOK_INT {
        printf("Int\n"); //DEBUG
    }
    | TOK_REAL
    | TOK_STRING

enclosure: '(' expression ')'

%%

int yywrap()
{
    return 1;
}

void yyerror(const char *msg)
{
    fprintf(stderr, "<Line %d> Parser error: '%s'.\n", yylineno, msg);
}

int main(int argc, char **argv)
{
    yyparse();
    return 0;
}
