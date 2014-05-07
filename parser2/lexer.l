%{                                                                             
#include <iostream>
#include <string>
#include "tetra.hpp"
#include "parser.tab.h"

std::string buffer;

%}
%option warn noyywrap yylineno
%x indents dedents strings end
%%

 /* Keywords */
"if"       {return TOK_IF;}
"elif"     {return TOK_ELIF;}
"else"     {return TOK_ELSE;}
"for"      {return TOK_FOR;}
"in"       {return TOK_IN;}
"forall"   {return TOK_FORALL;}
"parallel" {return TOK_PARALLEL;}
"while"    {return TOK_WHILE;}
"continue" {return TOK_CONTINUE;}
"break"    {return TOK_BREAK;}
"def"      {return TOK_DEF;}
"global"   {return TOK_GLOBAL;}
"or"       {return TOK_OR;}
"and"      {return TOK_AND;}
"not"      {return TOK_NOT;}
"pass"     {return TOK_PASS;}
"return"   {return TOK_RETURN;}
"int"      {return TOK_INT;}
"float"    {return TOK_FLOAT;}
"bool"     {return TOK_BOOL;}
"string"   {return TOK_STRING;}




 /* TODO whitespace */



"="     {return TOK_ASSIGN;}
"+"     {return TOK_PLUS;}
"-"     {return TOK_MINUS;}
"*"     {return TOK_TIMES;}
"/"     {return TOK_DIVIDE;}
"%"     {return TOK_MODULUS;}
"^"     {return TOK_BITXOR;}
"&"     {return TOK_BITAND;}
"|"     {return TOK_BITOR;}
"~"     {return TOK_BITNOT;}
"("     {return TOK_LEFTPARENS;}
")"     {return TOK_RIGHTPARENS;}
","     {return TOK_COMMA;}
"$"     {return TOK_DOLLAR;}
";"     {return TOK_SEMICOLON;}
":"     {return TOK_COLON;}
"<<"    {return TOK_LSHIFT;}
">>"    {return TOK_RSHIFT;}
"<="    {return TOK_LTE;}
">="    {return TOK_GTE;}
"=="    {return TOK_EQ;}
"!="    {return TOK_NEQ;}
"<"     {return TOK_LT;}
">"     {return TOK_GT;}
"+="    {return TOK_PLUSEQ;}
"-="    {return TOK_MINUSEQ;}
"*="    {return TOK_TIMESEQ;}
"/="    {return TOK_DIVIDEEQ;}
"%="    {return TOK_MODULUSEQ;}
"**="   {return TOK_EXPEQ;}
">>="   {return TOK_RSHIFTEQ;}
"<<="   {return TOK_LSHIFTEQ;}
"&="    {return TOK_ANDEQ;}
"^="    {return TOK_XOREQ;}
"|="    {return TOK_OREQ;}

 /* integer literals TODO more bases */
[1-9][0-9]* {yylval.intval = atoi(yytext); return TOK_INTVAL;}

 /* real literals */
[[:digit:]]+f               |
[[:digit:]]+"."[[:digit:]]* |
"."[[:digit:]]+digit        {yylval.realval = atof(yytext); return TOK_REALVAL;}

 /* string literals
  * Borrowed from flex.sourceforge.net/manual/Start-Conditions.html */
\"                      {buffer = ""; BEGIN(strings);}
<strings>\"             {BEGIN(INITIAL); strcpy(yylval.stringval, buffer.c_str( )); return TOK_STRINGVAL;}
<strings>\n             {fail("Unterminated string literal");}
<strings>\\[0-7]{1,3}   |
<strings>\\[0-9]+       |
<strings>\\n            |
<strings>\\t            |
<strings>\\r            |
<strings>\\b            |
<strings>\\f            |
<strings>[^\\\n\"]+     {buffer += yytext;}
<strings>\\(.|\n)       {buffer += yytext + 1;}

 /* boolean literals */
"true" {yylval.boolval = true; return TOK_BOOLVAL;}
"false" {yylval.boolval = false; return TOK_BOOLVAL;}

 /* identifiers */
[a-zA-Z_][a-zA-Z0-9_]* {strcpy(yylval.id, yytext); return TOK_IDENT;}

 /* ignore whitespace and comments */
"#".*$   {}
[ \t\n]+ {}

 /* anything else is an error */
. {printf("Error: invlaid lexeme '%s'.\n", yytext); return 0;} 

%%



