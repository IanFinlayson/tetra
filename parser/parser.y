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
 *
 * Currently, all lexemes that are always a single character (i.e., '+',
 * '-', etc., and not identifiers) are passed directly as that character.
 * All lexemes that can be multi-character ("==", "<<", identifiers, etc.)
 * are passed as tokens.  Is this awkward?  You betcha!  But the re-use
 * of many characters (notably *, +, and -), as well as the conflicts that
 * arise when a system of pure character passing for mult-character lexemes,
 * seem to make this a decent solution for now.
 *
 * Function calls do not currently allow for keyword arguments.
 *
 * Strings are not yet implemented.
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
%token <i> TOK_BOOL TOK_INT TOK_ASSIGN TOK_SHIFT TOK_COMP 
%token <f> TOK_REAL
%token TOK_OR TOK_AND TOK_NOT 
%token TOK_INDENT TOK_DEDENT TOK_NEWLINE
%token TOK_INT_DIV TOK_POW
%token TOK_IF TOK_ELIF TOK_ELSE 
%token TOK_WHILE TOK_FORALL TOK_FOR TOK_IN TOK_BREAK TOK_CONTINUE
%token TOK_DEF TOK_GLOBAL
%token TOK_PASS TOK_RETURN
%token TOK_INT_T TOK_REAL_T TOK_BOOL_T TOK_STRING_T

/* You can call nonterminals.awk here to generate a type list for all
 * non-terminals (with ":r !awk -f nonterminals.awk parser.y").
 * I suggest re-generating after any changes to the rules section.
 * This assumes that all non-terminals are of the type of node.
 */

%error-verbose

%%

program: stmt-list

stmt-list: stmt
    | stmt-list stmt

stmt: simple-stmt
    | compound-stmt

simple-stmt: small-stmt TOK_NEWLINE
    | simple-stmt ';' small-stmt TOK_NEWLINE

compound-stmt: if-stmt
    | while-stmt
    | for-stmt
    | func-def

suite: simple-stmt
    | TOK_NEWLINE TOK_INDENT stmt-list TOK_DEDENT

if-stmt: if
    | if else
    | if elif-list
    | if elif-list else

if: TOK_IF expression ':' suite

elif-list: elif
    | elif-list elif

elif: TOK_ELIF expression ':' suite

else: TOK_ELSE ':' suite

while-stmt: while
    | while else

while: TOK_WHILE expression ':' suite

for-stmt: for
    | for else

for: TOK_FOR target-list TOK_IN expression-list ':' suite

func-def: TOK_DEF funcname '(' parameter-list ')' return-type ':'

funcname: identifier

parameter-list: /* empty */
    | parameter-list ',' identifier

return-type: /* empty */
    | type

type: TOK_INT_T
    | TOK_REAL_T
    | TOK_BOOL_T
    | TOK_STRING_T

small-stmt: expression-stmt
    | assignment-stmt
    | augmented-assignment-stmt
    | pass-stmt
    | return-stmt
    | break-stmt
    | continue-stmt
    | global-stmt

expression-stmt: expression-list

expression-list: expression
    | expression-list ',' expression
    | expression-list ',' expression ','

expression: conditional-expr

assignment-stmt: l-side-eq expression-list

l-side-eq: target-list '='
    | l-side-eq target-list '='

target-list: target
    | target-list ',' target

target: identifier

identifier: TOK_IDENTIFIER

augmented-assignment-stmt: augtarget TOK_ASSIGN expression-list

augtarget: identifier

pass-stmt: TOK_PASS

return-stmt: TOK_RETURN

break-stmt: TOK_BREAK

continue-stmt: TOK_CONTINUE

global-stmt: TOK_GLOBAL identifier-list

identifier-list: identifier
    | identifier-list ',' identifier

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
    | add-expr '+' mult-expr
    | add-expr '-' mult-expr

mult-expr: unary-expr
    | mult-expr '*' unary-expr
    | mult-expr '/' unary-expr
    | mult-expr '%' unary-expr
    | mult-expr TOK_INT_DIV unary-expr

unary-expr: power-expr
    | '+' unary-expr
    | '-' unary-expr
    | '~' unary-expr

power-expr: primary
    | primary TOK_POW unary-expr

primary: atom
    | call

atom: identifier 
    | literal 
    | enclosure

literal: TOK_BOOL
    | TOK_INT
    | TOK_REAL
    | TOK_STRING

enclosure: '(' expression ')'

call: primary '(' ')'
    | primary '(' argument-list ')'

argument-list: positional-arguments

positional-arguments: expression
    | positional-arguments ',' expression

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
