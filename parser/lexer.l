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
#include "tetra_hdr.h"
#include "string_buffer.h"
/* Lest we forget: include definition files BEFORE parser.tab.h, as here */
#include "parser.tab.h"

#define MAX_INDENTS 50
#define INIT_STR_BUF_CAPACITY 100

int indent_level[MAX_INDENTS];
int *num_ind = indent_level;

char *endptr; /* Used to determine if valid non-base-10 literal is used */

Str_Buf *str_buf;

void error_exit(const char *str);
int is_in(int target, int *array, int *end_ptr);
%}

%option warn noyywrap yylineno nounput
%x indents dedents strings end
%%
 /* skip blank lines */
<*>^[[:blank:]]*\n  { } 

 /* Skip lines containing only blanks and comments */
<*>^[[:blank:]]*#.*\n    { }
 /* But make sure that lines with code and comments end right */
<INITIAL>#.*\n  {
                    BEGIN(indents);
                    return TOK_NEWLINE;
                }

 /* no whitespace to begin line */
<indents>^[^[:space:]] { 
    yyless(0);
    BEGIN(INITIAL);
    if (*num_ind > 0) {     /* dedented */
        BEGIN(dedents);
    }
}

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
    yyless(0);
    --num_ind;
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
    BEGIN(indents);
    return TOK_NEWLINE;
}

\"  { 
        if (str_buf == NULL)
            str_buf = make_str_buf(INIT_STR_BUF_CAPACITY);
        else
            reset_str_buf(str_buf, INIT_STR_BUF_CAPACITY);
        BEGIN(strings); 
    }

 /*
  * Borrowed from flex.sourceforge.net/manual/Start-Conditions.html
  */
<strings>\"             {
                            BEGIN(INITIAL);
                            yylval.text = str_buf->buffer;
                            return TOK_STRING;
                        }
<strings>\n             {
                            error_exit("Unterminated string literal");
                        }
<strings>\\[0-7]{1,3}   {
                            append_to_str_buf(str_buf, yytext);
                        }
<strings>\\[0-9]+       {
                            append_to_str_buf(str_buf, yytext);
                        }
<strings>\\n            |
<strings>\\t            |
<strings>\\r            |
<strings>\\b            |
<strings>\\f            { append_to_str_buf(str_buf, yytext); }
<strings>\\(.|\n)       { append_to_str_buf(str_buf, yytext+1); }
<strings>[^\\\n\"]+     { append_to_str_buf(str_buf, yytext); }

[[:blank:]]+    /* skip spaces & tabs */

 /* Boolean literals (TOK_BOOL) */
"True"  { yylval.i = 1; return TOK_BOOL; }
"False" { yylval.i = 0; return TOK_BOOL; }

 /* Integer literals (TOK_INT) */
0[[:digit:]]+       {
                        yylval.i = STR_TO_I(yytext, &endptr, 8);
                        if (*endptr != '\0') /* 8 or 9 captured */
                            error_exit("Invalid octal literal");
                        return TOK_INT;
                    }
[[:digit:]]         |
[1-9][[:digit:]]+   {   
                        yylval.i = STR_TO_I(yytext, NULL, 10);
                        return TOK_INT;
                    }
0[Bb][01]+          {
                        yylval.i = STR_TO_I(yytext+2, NULL, 2);
                        return TOK_INT;
                    }
0[Xx][[:xdigit:]]+  { 
                        yylval.i = STR_TO_I(yytext+2, NULL, 16);
                        return TOK_INT;
                    }

 /* Floating-point literals (TOK_REAL) */
[[:digit:]]+f                   |
[[:digit:]]+"."[[:digit:]]*     |
"."[[:digit:]]+	                {
                                    yylval.f = STR_TO_F(yytext);
                                    return TOK_REAL; 
                                }

 /* Keywords */
"if"        { return TOK_IF; }
"elif"      { return TOK_ELIF; }
"else"	    { return TOK_ELSE; }
"for"       { return TOK_FOR; }
"in"        { return TOK_IN; }
"forall"    { return TOK_FORALL; }
"parallel"  { return TOK_PARALLEL; }
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
"print"     { return TOK_PRINT; }
"int"       { yylval.i = INT_T; return TOK_TYPE; }
"float"      { yylval.i = FLOAT_T; return TOK_TYPE; }
"bool"      { yylval.i = BOOL_T; return TOK_TYPE; }
"string"    { yylval.i = STRING_T; return TOK_TYPE; }
"void"      { yylval.i = VOID_T; return TOK_TYPE; }

 /* Single-character lexemes */
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
"$" |
";" |
":" { return yytext[0]; }

 /* Shift operators (TOK_SHIFT) */
 
"<<"    { return TOK_LSHIFT; }
">>"    { return TOK_RSHIFT; }
 
 /* Comparison operators (TOK_COMP) */

"<="	{ yylval.i = LTE; return TOK_LTE; }
">="	{ yylval.i = GTE; return TOK_GTE; }
"=="	{ yylval.i = EQ;  return TOK_EQ; }
"!="	{ yylval.i = NEQ; return TOK_NEQ; }
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
"**"    { return TOK_POW; }


 /* Assignment operators (TOK_ASSIGN) */
 
 /*
"="     { yylval.i = BEC;     return TOK_ASSIGN; }
 */
"+="	{ yylval.i = PLU_BEC; return TOK_ASSIGN; }
"-="	{ yylval.i = MIN_BEC; return TOK_ASSIGN; }
"*="	{ yylval.i = MUL_BEC; return TOK_ASSIGN; }
"/="	{ yylval.i = DIV_BEC; return TOK_ASSIGN; }
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

 /* Finish up all dedents at end of file - SUPER KLUDGE */
 /* BAH!  More research needed */

%%

void error_exit(const char *str)
{
    fprintf(stderr, "Line %d: %s\n", yylineno, str);
    exit(1);
}

int is_in(int target, int *array, int *end_ptr)
{
    if (end_ptr < array) {
        return 0;
    }
    int *cur;
    for (cur = end_ptr; cur >= array; cur--) {
        if (*cur == target) {
            return 1;
        }
    }
    return 0;
}
