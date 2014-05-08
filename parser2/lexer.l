%{                                                                             
#include <iostream>
#include <string>
#include "tetra.hpp"
#include "parser.gen.hpp"

/* used for string literals */
std::string buffer;

/* used for whitespace indentation */
const int MAX_INDENTS = 50;
int indent_level[MAX_INDENTS];
int *num_ind = indent_level;
int is_in(int target, int *array, int *end_ptr);

%}
%option warn noyywrap yylineno
%x indents dedents strings end
%%

 /* skip blank lines */
<*>^[[:blank:]]*\n { } 

 /* Skip lines containing only blanks and comments */
<*>^[[:blank:]]*#.*\n { }

 /* but make sure that lines with code and comments end right */
<INITIAL>#.*\n  {BEGIN(indents); return TOK_NEWLINE;}

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
      fail("Dedent to non-matching level", yylineno);
    }
    yyless(0);
    BEGIN(dedents);
  } else {                            /* indented */
    BEGIN(INITIAL);
    if (++num_ind == indent_level + MAX_INDENTS) {
      fail("Too many levels of indentation", yylineno);
    }
    *num_ind = yyleng;
    return TOK_INDENT;
  }
}
    
<dedents>[[:blank:]]* { 
  if (yyleng == *num_ind) {
    BEGIN(INITIAL);
  } else if (yyleng < *num_ind) {
    for (int i = 0; i < yyleng; i++) {
      unput(' ');
    }
    --num_ind;
    return TOK_DEDENT;
  } else { 
    fail("Major goof getting here", yylineno);
  }
}

 /* must be sure dedent level exists before entering this state */
<dedents>[^[:space:]] {
  yyless(0);
  --num_ind;
  if (*num_ind == 0) {
    BEGIN(INITIAL);
  }
  return TOK_DEDENT;
}

\n  {
  BEGIN(indents);
  return TOK_NEWLINE;
}


 /* keywords */
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

 /* operators */
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
[a-zA-Z_][a-zA-Z0-9_]* {strcpy(yylval.stringval, yytext); return TOK_IDENT;}

 /* ignore whitespace and comments */
[ \t\n]+ {}

 /* anything else is an error */
. {printf("Error: invlaid lexeme '%s'.\n", yytext); return 0;} 

%%


int is_in(int target, int *array, int *end_ptr) {
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

