%{                                                                             
#include <iostream>
#include <string>

#include "tetra.hpp"

extern int yylineno;
int yylex( );
int yywrap( );
void yyerror(const char* str);

/* the root of the parse tree - this can be used after calling yyparse */
Node* root;

%}

/* each non-terminal is represented with a node literlas are doubles */
%union {
  Node* node;
  int intval;
  double realval;
  bool boolval;
  char stringval[256];
  DataType data_type;
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
%token TOK_REAL
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
%token TOK_EXP
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
%token <stringval> TOK_IDENTIFIER

/* dummy tokens */
%token TOK_INDENT
%token TOK_DEDENT
%token TOK_NEWLINE

/* types */
%type <node> functions function param_list statements statement block params param simple_statements
%type <node> compound_statement simple_statement pass_statement return_statement break_statement
%type <node> continue_statement expression if_statement while_statement else_option
%type <data_type> return_type type

%error-verbose

%%

/* a program is a list of functions */
program: functions {
  root = $1;
}

/* a list of functions */
functions: function functions {
  $$ = new Node(NODE_FUNCTION_LIST);
  $$->addChild($1);
  $$->addChild($2);
} | {
  $$ = NULL;
}

/* a single function */
function: TOK_DEF TOK_IDENTIFIER param_list return_type TOK_COLON block {
  $$ = new Node(NODE_FUNCTION);
  $$->setDataType($4);
  $$->addChild($3);
  $$->addChild($6);
}

/* a parameter list (with bannanas) */
param_list: TOK_LEFTPARENS params TOK_RIGHTPARENS {
  $$ = $2;
} | TOK_LEFTPARENS TOK_RIGHTPARENS {
  $$ = NULL;
}

/* a list of at least one parameter */
params: param TOK_COMMA params {
  $$ = new Node(NODE_PARAM_LIST);
  $$->addChild($1);
  $$->addChild($3);
} | param {
  $$ = NULL;
}

/* a single parameter */
param: TOK_IDENTIFIER type {
  $$ = new Node(NODE_PARAM);
  $$->setIdentifier(std::string($1));
  $$->setDataType($2);
}

/* types (keeping it simple for now...) */
type: TOK_INT {
  $$ = TYPE_INT;
} | TOK_REAL {
  $$ = TYPE_REAL;
} | TOK_STRING {
  $$ = TYPE_STRING;
} | TOK_BOOL {
  $$ = TYPE_BOOL;
}

/* a return type is either a simple type or none which means void */
return_type: type {
  $$ = $1;
} | {
  $$ = TYPE_VOID;
}

/* a block is a set of statements, indented over */
block: TOK_NEWLINE TOK_INDENT statements TOK_DEDENT {
  $$ = $3;
}

/* a list of at least one statement */
statements: statement statements {
  $$ = new Node(NODE_STATEMENT);
  $$->addChild($1);
  $$->addChild($2);
} | statement {
  $$ = $1;
}

/* a single statement */
statement: simple_statements
  | compound_statement {
  $$ = $1;
}

/* simple statements are a list of simple statements separated by semi-conlons on one line */
simple_statements: simple_statement TOK_SEMICOLON simple_statements TOK_NEWLINE {
  $$ = new Node(NODE_STATEMENT);
  $$->addChild($1);
  $$->addChild($3);
} | simple_statement TOK_NEWLINE {
  $$ = $1;
}

/* a simple statement is a one-liner that can't be broken down */
simple_statement: pass_statement
  | return_statement
  | break_statement
  | continue_statement
  | expression {
  $$ = $1;
}

/* a compound statement is one which has a block of code under it */
compound_statement: if_statement
/*  | elif_statement
  | for_statement */
  | while_statement {
  $$ = $1;
}

/* simple statements */
pass_statement: TOK_PASS {
  $$ = new Node(NODE_PASS);
}
return_statement: TOK_RETURN {
  $$ = new Node(NODE_RETURN);
}
break_statement: TOK_BREAK {
  $$ = new Node(NODE_BREAK);
}
continue_statement: TOK_CONTINUE {
  $$ = new Node(NODE_CONTINUE);
}

/* if statement with or without else */
if_statement: TOK_IF expression TOK_COLON block else_option {
  $$ = new Node(NODE_IF);
  $$->addChild($2);
  $$->addChild($4);
  if ($5) {
    $$->addChild($5);
  }
}

/* either an else block or nothing */
else_option: TOK_ELSE TOK_COLON block {
  $$ = $3;
} | {
  $$ = NULL;
}

/* a while loop */
while_statement: TOK_WHILE expression TOK_COLON block {
  $$ = new Node(NODE_WHILE);
  $$->addChild($2);
  $$->addChild($4);
}

/* TODO expressions */
expression: TOK_INTVAL {
  $$ = NULL;
}


%%

int yywrap( ) { 
  return 1;
}

void yyerror(const char* str) {
  fail(str);
}

/* print an error message and quit */
void fail(const std::string& mesg) {
  std::cerr << "Tetra error: " << mesg << " (Line " << yylineno << ")" << std::endl;
  exit(1);
}


