/*
 * Lexer for Tetra.
 * 
 * Current state of DEDENTS:
 *      The DEDENT generation is a real hack right now.
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
#include <string.h>
/* Lest we forget: include definition files BEFORE parser.tab.h, as here */
#include "tetra_hdr.h"
#include "parser.tab.h"

#define MAX_INDENTS 50
int dedent_level;
int indent_level[MAX_INDENTS];
int *num_ind = indent_level;

void error_exit(const char *str);
int is_in(int target, int *array, int *end_ptr);
%}

%option warn nounput noyywrap yylineno
%x indents dedents strings
%%
 /* skip blank lines */
<*>^[[:blank:]]*\n  {
//                        printf("Line %d is blank\n", yylineno-1); //DEBUG
                    } 

 /* Skip lines containing only blanks and comments */
<*>^[[:blank:]]*#.*\n    {
                printf("Comment!\n");
//                printf("Line %d has a comment\n", yylineno-1); //DEBUG
            }
 /* But make sure that lines with code and comments end right */
<INITIAL>#.*\n  {
                    BEGIN(indents);
                    return TOK_NEWLINE;
                }

 /* no whitespace to begin line */
<indents>^[^[:space:]] { 
//    printf("In state %d: Cur ind_lvl: %d\n", YY_START, *num_ind); //DEBUG
//    printf("Put back %c\n", yytext[0]); //DEBUG
    yyless(0);
    BEGIN(INITIAL);
    if (*num_ind > 0) {     /* dedented */
//        printf("Should be dedenting\n"); //DEBUG
        BEGIN(dedents);
    }
}

<indents>^[[:blank:]]* {
//    printf("%d: Leading whitespace length: %d\n", yylineno, yyleng);//DEBUG
    if (yyleng == *num_ind) {           /* no change */
        BEGIN(INITIAL);
    } else if (yyleng < *num_ind) {     /* dedented */
//        printf("%d: Should be dedenting 2\n", yylineno); //DEBUG
        if (!is_in(yyleng, indent_level, num_ind)) {
            error_exit("Dedent to non-matching level");
        }
        yyless(0);
        BEGIN(dedents);
    } else {                            /* indented */
//        printf("%d Indented to %d\n", yylineno, yyleng); //DEBUG
        BEGIN(INITIAL);
        if (++num_ind == indent_level + MAX_INDENTS) {
            error_exit("Too many levels of indentation");
        }
        *num_ind = yyleng;
        return TOK_INDENT;
    }
}
 /* Must be sure dedent level exists before entering this state */
<dedents>[^[:space:]] {
//    printf("Hooray I'm here! "); //DEBUG
//    printf("State: %d\n", YY_START); //DEBUG
//    printf("%d: Dedent length: 0\n", yylineno); //DEBUG
//    printf("Putting back: %c\n", yytext[0]); //DEBUG
    yyless(0);
    --num_ind;
//    printf("%d: *num_ind = %d\n", yylineno, *num_ind); //DEBUG
    if (*num_ind == 0) {
        BEGIN(INITIAL);
    }
    return TOK_DEDENT;
}
    
<dedents>[[:blank:]]* {
//    printf("%d: Dedent length %d\n", yylineno, yyleng); //DEBUG
    if (yyleng == *num_ind) {
        BEGIN(INITIAL);
    } else if (yyleng < *num_ind) {
        yyless(*num_ind - yyleng);
        --num_ind;
    } else { 
        error_exit("Major goof getting here");
    }
    return TOK_DEDENT;
}

\n  {
//    printf("Line: %d\n", yylineno); //DEBUG
    BEGIN(indents);
    return TOK_NEWLINE;
}

[[:blank:]]+    /* skip spaces & tabs */

 /* Boolean literals (TOK_BOOL) */
"True"  { yylval.i = 1; return TOK_BOOL; }
"False" { yylval.i = 0; return TOK_BOOL; }

 /* Integer literals (TOK_INT) */
0[[:digit:]]+       {
                        yylval.i = strtoimax(yytext, NULL, 8);
                        return TOK_INT;
                    }
[[:digit:]]         |
[1-9][[:digit:]]+   {   
                        yylval.i = strtoimax(yytext, NULL, 10);
                        return TOK_INT;
                    }
0[Bb][01]+          {
                        yylval.i = strtoimax(yytext+2, NULL, 2);
                        return TOK_INT;
                    }
0[Xx][[:xdigit:]]+  { 
                        yylval.i = strtoimax(yytext+2, NULL, 16);
                        return TOK_INT;
                    }

 /* Floating-point literals (TOK_REAL) */
[[:digit:]]+f                   |
[[:digit:]]+"."[[:digit:]]*     |
"."[[:digit:]]+	                {
                                    yylval.f = strtold(yytext, NULL);
                                    return TOK_REAL; 
                                }

 /* Keywords */
"if"        { return TOK_IF; }
"elif"      { return TOK_ELIF; }
"else"	    { return TOK_ELSE; }
"for"       { return TOK_FOR; }
"in"        { return TOK_IN; }
"forall"    { return TOK_FORALL; }
"while"     { return TOK_WHILE; }
"continue"  { return TOK_CONTINUE; }
"break"     { return TOK_BREAK; }
"def"       { return TOK_DEF; }
"global"    { return TOK_GLOBAL; }
"or"        { return TOK_AND; }
"and"       { return TOK_OR; }
"not"       { return TOK_NOT; }
"pass"      { return TOK_PASS; }
"return"    { return TOK_RETURN; }
"int"       { return TOK_INT_T; }
"real"      { return TOK_REAL_T; }
"bool"      { return TOK_BOOL_T; }
"string"    { return TOK_STRING_T; }

 /* In order to shift between tokenizing most individual characters and
  * just passing them through, invert the commenting of this section 
  * and the following sections.
  */
"<" |
">" |
"=" |
"+" |
"-" |
"*" |
"/" |
"%" |
"^" |
"&" |
"|" |
"!" |
"~" |
"(" |
")" |
"," |
":" { return yytext[0]; }

 /* Shift operators (TOK_SHIFT) */
 
"<<"    { yylval.i = SHIFT_L; return TOK_SHIFT; }
">>"    { yylval.i = SHIFT_R; return TOK_SHIFT; }
 
 /* Comparison operators (TOK_COMP) */

"<="	{ yylval.i = LTE; return TOK_COMP; }
">="	{ yylval.i = GTE; return TOK_COMP; }
"=="	{ yylval.i = EQ;  return TOK_COMP; }
"<>"    |
"!="	{ yylval.i = NEQ; return TOK_COMP; }
 /*
"<"	    { yylval.i = LT;  return TOK_COMP; }
">"	    { yylval.i = GT;  return TOK_COMP; }
 */

 /* Bitwise operators (No TOK) */
 /* 
"~"     |
"|"     |
"^"     |
"&"     { return yytext[0]; }
 */
 /* Numeric operators (Various TOK) */
 /*
"+"	    |
"-"	    { yylval.i = yytext[0]; return TOK_ADD; }
"%"     |
"/"	    |
"*"	    { yylval.i = yytext[0]; return TOK_MULT; }
 */
"//"    { return TOK_INT_DIV; } 
"**"    { return TOK_POW; }


 /* Assignment operators (TOK_ASSIGN) */
 
 /*
"="     { yylval.i = BEC;     return TOK_ASSIGN; }
 */
"+="	{ yylval.i = PLU_BEC; return TOK_ASSIGN; }
"-="	{ yylval.i = MIN_BEC; return TOK_ASSIGN; }
"*="	{ yylval.i = MUL_BEC; return TOK_ASSIGN; }
"/="	{ yylval.i = DIV_BEC; return TOK_ASSIGN; }
"//="   { yylval.i = IDV_BEC; return TOK_ASSIGN; }
"%="    { yylval.i = MOD_BEC; return TOK_ASSIGN; }
"**="   { yylval.i = POW_BEC; return TOK_ASSIGN; }
">>="   { yylval.i = RSH_BEC; return TOK_ASSIGN; }
"<<="   { yylval.i = LSH_BEC; return TOK_ASSIGN; }
"&="    { yylval.i = AND_BEC; return TOK_ASSIGN; }
"^="    { yylval.i = XOR_BEC; return TOK_ASSIGN; }
"|="    { yylval.i = OR_BEC;  return TOK_ASSIGN; }

 /* Single character misc. (No TOK) */
 /*
":"     |
"("     |
")"     { return yytext[0]; }
 */

 /* Identifiers */
[[:alpha:]_][[:alnum:]_]*   {
                                yylval.text = yytext;
                                return TOK_IDENTIFIER;
                            }
 /* Catch unexpected characters */
<*>.    { 
            fprintf(stderr, "Error in state %d\n", YY_START);
            fprintf(stderr, "%d\n", yytext[0]);
            error_exit("Unexpected character");
        }

%%

void error_exit(const char *str)
{
    fprintf(stderr, "Line %d: %s\n", yylineno, str);
    exit(1);
}

int is_in(int target, int *array, int *end_ptr)
{
//    printf("In function is_in()\n"); //DEBUG
    if (end_ptr < array) {
//        printf("end_ptr > array\n"); //DEBUG
        return 0;
    }
    int *cur;
    for (cur = end_ptr; cur >= array; cur--) {
//        printf("Checking indent level: %d\n", *cur); //DEBUG
        if (*cur == target) {
//            printf("Match found: %d\n", *cur); //DEBUG
            return 1;
        }
    }
    return 0;
}
