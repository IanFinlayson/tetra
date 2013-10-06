/*
 * Lexer for Tetra.  The DEDENT generation is a real hack right now.
 * 
 * Current state of DEDENTS:
 *      Mixed tabs and spaces are allowed at the moment.  HOWEVER! Tabs and
 *      spaces are treated as equivalent, so one should not mix them in the
 *      same file.  This should be fixed in the future.
 *
 *      Furthermore, no more than 50 levels of indentation are allowed.
 *      This is easily changed by modifying MAX_INDENTS below.
 *
 * STRINGS not implemented
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include "tetra_hdr.h"

#define MAX_INDENTS 50
int dedent_level;
int indent_level[MAX_INDENTS];
int *num_ind = indent_level;

void error_exit(const char *str);
int is_in(int target, int *array, int *end_ptr);
%}

%option warn nounput yylineno
%x indents dedents strings
%%
    /* skip blank lines */
<INITIAL,indents>^[[:blank:]]*\n     { yylineno++; } 

<indents>^[[:blank:]]* {
    if (yyleng == *num_ind) {           /* no change */
        BEGIN(INITIAL);
    } else if (yyleng < *num_ind) {     /* dedented */
        if (!is_in(yyleng, indent_level, num_ind)) {
            error_exit("Dedent to non-matching level");
        }
        yyless(0);
        BEGIN(dedents);
    } else {                            /* indented */
        if (++num_ind == indent_level + MAX_INDENTS) {
            error_exit("Too many levels of indentation");
        }
        *num_ind = yyleng;
        return TOK_INDENT
    }
}
 /* Must be sure dedent level exists before entering this state */
<dedents>^[[:blank:]]* {
    if (yyleng == *num_ind) {
        BEGIN(INITIAL);
    } else if (yyleng < *num_ind) {
        yyless(0);
        --num_ind;
    } else { 
        error_exit("Major goof getting here");
    }
    return TOK_DEDENT;
}

\n  {
    BEGIN(indents);
    return TOK_NEWLINE;
}

[[:blank:]]+                        /* skip spaces & tabs */
#.*$                                { yylineno++; }

 /* Boolean literals (TOK_BOOL) */
"True"                      { yylval.i = 1; return TOK_BOOL; }
"False"                     { yylval.i = 0; return TOK_BOOL; }

 /* Integer literals (TOK_INT) */
[1-9][[:digit:]]+           {
                                yylval.i = strtoimax(yytext, NULL, 10);
                                return TOK_INT;
                            }
0[[:digit:]]+               {
                                yylval.i = strtoimax(yytext, NULL, 8);
                                return TOK_INT;
                            }
0[Bb][01]+                  {
                                yylval.i = strtoimax(yytext+2, NULL, 2);
                                return TOK_INT;
                            }
0[Xx][[:xdigit:]]+          { 
                                yylval.i = strtoimax(yytext+2, NULL, 16);
                                return TOK_INT;
                            }

 /* Floating-point literals (TOK_REAL) */
[[:digit:]]+f                   |
[[:digit:]]+"."[[:digit:]]*     |
"."[[:digit:]]+	                {
                                    yylval.ld = strtold(yytext, NULL);
                                    return TOK_REAL; 
                                }

 /* Keywords */
"if"        { return TOK_IF; }
"else"	    { return TOK_ELSE; }
"for"       { return TOK_FOR; }
"forall"    { return TOK_FORALL; }
"while"     { return TOK_WHILE; }
"def"       { return TOK_DEF; }
"or"        { return TOK_AND; }
"and"       { return TOK_OR; }
"not"       { return TOK_NOT; }

 /* Shift operators (TOK_SHIFT) */
"<<"    { yylval.i = SHIFT_L; return TOK_SHIFT; }
">>"    { yylval.i = SHIFT_R; return TOK_SHIFT; }

 /* Comparison operators (TOK_COMP) */
"<="	                            { yylval.i = LTE; return TOK_COMP; }
">="	                            { yylval.i = GTE; return TOK_COMP; }
"=="	                            { yylval.i = EQ;  return TOK_COMP; }
"<>"                                |
"!="	                            { yylval.i = NEQ; return TOK_COMP; }
"<"	                                { yylval.i = LT;  return TOK_COMP; }
">"	                                { yylval.i = GT;  return TOK_COMP; }

 /* Bitwise operators (No TOK) */
"~"     |
"|"     |
"^"     |
"&"     { return yytext[0]; }

 /* Numeric operators (Various TOK) */
"+"	    |
"-"	    |
"%"     |
"/"	    |
"*"	    { return yytext[0]; }
"**"    { return TOK_EXP; }

 /* Assignment operators (TOK_ASSIGN) */
"="                              { yylval.i = BEC;     return TOK_ASSIGN; }
"/="	                         { yylval.i = DIV_BEC; return TOK_ASSIGN; }
"*="	                         { yylval.i = MUL_BEC; return TOK_ASSIGN; }
"+="	                         { yylval.i = PLU_BEC; return TOK_ASSIGN; }
"-="	                         { yylval.i = MIN_BEC; return TOK_ASSIGN; }

 /* Single character misc. (No TOK) */
":"                                 |
"("                                 |
")"                                 { return yytext[0]; }

 /* Identifiers */
[[:alpha:]_][[:alnum:]_]*	        { return TOK_IDENTIFIER; }

%%

void error_exit(const char *str)
{
    fprintf("Line %d: %s\n", yylineno, str);
    exit(1);
}

int is_in(int target, int *array, int *end_ptr)
{
    if (end_ptr > array)
        return 0;
    int *cur;
    for (cur = end_ptr; cur >= array; cur--) {
        if (*cur == target)
            return 1;
    }
    return 0;
}
