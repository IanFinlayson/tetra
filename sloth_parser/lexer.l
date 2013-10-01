/* Scanner for Sloth language
 * Author: Dan Easterling
 * CPSC 401 UMW Fall 2013
 * 18 September 2013
 * mod: 1 October 2013
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "parser.tab.h"

/*
enum {
    TOK_IDENT = 100, TOK_VALUE, TOK_PLUS, TOK_MINUS, TOK_DIV, TOK_MULT, 
    TOK_LT, TOK_GT, TOK_LTE, TOK_GTE, TOK_EQ, TOK_NEQ, TOK_AND, TOK_OR, 
    TOK_NOT, TOK_SEMI, TOK_BECOMES, TOK_LEFTPAR, TOK_RIGHTPAR, TOK_BEG, 
    TOK_END, TOK_IF, TOK_THEN, TOK_ELSE, TOK_WHILE, TOK_DO, TOK_PRINT, 
    TOK_INPUT 
}; 
*/
%}

%option noyywrap

%%

"%".*$                              /* skip comments */
[[:space:]]+                        /* skip whitespace */
"begin" 							{ return BEG; }
"end"  							    { return END; }
"if"    							{ return IF; }
"then"  							{ return THEN; }
"else"  							{ return ELSE; }
"while" 							{ return WHILE; }
"do"    							{ return DO; }
"print" 							{ return PRINT; }
"input" 							{ return INPUT; }
"+"     							{ return PLUS; }
"-"     							{ return MINUS; }
"/"     							{ return DIVIDE; }
"*"     							{ return MULTIPLY; }
"<"     							{ return LT; }
">"     							{ return GT;}
"<="    							{ return LTE; }
">="    							{ return GTE; }
"=="    							{ return EQ; }
"!="    							{ return NEQ; }
"&&"    							{ return AND; }
"||"    							{ return OR; }
"!"     							{ return NOT; }
";"     							{ return SEMICOLON; }
":="    							{ return BECOMES; }
"("     							{ return LEFT_PAR; }
")"     							{ return RIGHT_PAR; }
[[:alpha:]_][[:alnum:]_]*           { return IDENTIFIER; }
[[:digit:]]*"."?[[:digit:]]+        { return VALUE; }
. {
    fprintf(stderr, "Error: invalid lexeme '%s'.\n", yytext);
    return 0;
  }

%%
/*
int main(int argc, char **argv)
{
    int token;

    if (argc < 2) {
        yyin = stdin;
    } else {
        if ((yyin = fopen(argv[1], "r")) == NULL) {
            perror("Error opening file");
            exit(1);
        }
    }

    while (token = yylex()) {
        printf("%d\n", token);
    }

    return 0;
}
*/
