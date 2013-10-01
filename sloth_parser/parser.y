/*
 * Daniel Easterling
 * Parser for Sloth
 * CPSC 401 UMW
 * 1 October 2013
 *
 * Grammar "inspired" by the description of the C grammar in K&R
 */
%{
#include <stdio.h>
#include <string.h>
#include "node.h"

int yywrap(void);
void yyerror(const char* str);
%}

%token IDENTIFIER
%token VALUE
%token PLUS MINUS DIVIDE MULTIPLY
%token LT GT LTE GTE EQ NEQ AND OR NOT
%token SEMICOLON
%token BECOMES
%token LEFT_PAR RIGHT_PAR
%token BEG END
%token IF THEN ELSE WHILE DO PRINT INPUT

%error-verbose

%%

program: statement-list {}


statement-list: statement
              | statement-list statement {}
              

statement: assignment-statement
         | print-statement
         | selection-statement
         | iteration-statement
         | compound-statement {}
                  

assignment-statement: IDENTIFIER BECOMES expression SEMICOLON {}


print-statement: PRINT expression SEMICOLON {}


selection-statement: IF expression THEN statement
                   | IF expression THEN statement ELSE statement {}
                   

iteration-statement: WHILE expression DO statement {}


compound-statement: BEG statement-list END {}


    /* Expressions */

primary-expression: IDENTIFIER
                  | INPUT
                  | VALUE
                  | LEFT_PAR expression RIGHT_PAR {}
                  

unary-expression: primary-expression
                | NOT unary-expression {}
                

multiplicative-expression: unary-expression
    | multiplicative-expression MULTIPLY unary-expression
    | multiplicative-expression DIVIDE unary-expression
    

additive-expression: multiplicative-expression
    | additive-expression PLUS multiplicative-expression
    | additive-expression MINUS multiplicative-expression {}
    
    
relational-expression: additive-expression
    | relational-expression LT additive-expression
    | relational-expression LTE additive-expression
    | relational-expression GT additive-expression
    | relational-expression GTE additive-expression
    | relational-expression EQ additive-expression
    | relational-expression NEQ additive-expression {}
    

and-expression: relational-expression
    | and-expression AND relational-expression {}
    

or-expression: and-expression
    | or-expression OR and-expression {}
    

expression: or-expression {}

%%

int yywrap()
{
    return 1;
}

void yyerror(const char *str)
{
    fprintf(stderr, "Compiler error: '%s'.\n", str);
}

int main(int argc, char **argv)
{
    yyparse();
    return 0;
}
