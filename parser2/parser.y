%{                                                                             
#include <iostream>
#include <string>

#include "tetra.hpp"

extern int yylineno;

int yylex( );
int yywrap( );
void yyerror(const char* str);
%}

/* each non-terminal is represented with a node literlas are doubles */
%union {
  struct Node* node;
  int intval;
  double realval;
  bool boolval;
  char stringval[256];
}

/* typless tokens */
%token TOK_IF
%token TOK_ELIF
%token TOK_ELSE
%token TOK_FOR
%token TOK_IN
%token TOK_FORALL
%token TOK_PARALLEL
%token TOK_WHILE
%token TOK_CONTINUE
%token TOK_BREAK
%token TOK_DEF
%token TOK_GLOBAL
%token TOK_OR
%token TOK_AND
%token TOK_NOT
%token TOK_PASS
%token TOK_RETURN
%token TOK_INT
%token TOK_FLOAT
%token TOK_BOOL
%token TOK_STRING
%token TOK_ASSIGN
%token TOK_PLUS
%token TOK_MINUS
%token TOK_TIMES
%token TOK_DIVIDE
%token TOK_MODULUS
%token TOK_BITXOR
%token TOK_BITAND
%token TOK_BITOR
%token TOK_BITNOT
%token TOK_LEFTPARENS
%token TOK_RIGHTPARENS
%token TOK_COMMA
%token TOK_DOLLAR
%token TOK_SEMICOLON
%token TOK_COLON
%token TOK_LSHIFT
%token TOK_RSHIFT
%token TOK_LTE
%token TOK_GTE
%token TOK_EQ
%token TOK_NEQ
%token TOK_LT
%token TOK_GT
%token TOK_PLUSEQ
%token TOK_MINUSEQ
%token TOK_TIMESEQ
%token TOK_DIVIDEEQ
%token TOK_MODULUSEQ
%token TOK_EXPEQ
%token TOK_RSHIFTEQ
%token TOK_LSHIFTEQ
%token TOK_ANDEQ
%token TOK_XOREQ
%token TOK_OREQ

/* typed tokens */
%token <intval> TOK_INTVAL
%token <realval> TOK_REALVAL
%token <boolval> TOK_BOOLVAL
%token <stringval> TOK_STRINGVAL
%token <id> TOK_IDENT

%error-verbose

%%


program: TOK_INTVAL {
  std::cout << "YES!" << std::endl;
}

%%

int yywrap( ) { 
  return 1;
}

void yyerror(const char* str) {
  fail(str, yylineno);
}



