%{                                                                             
/* the parser for Tetra, this Bison parser generates a yyparse function which
 * is used to parse Tetra code from stdin into a parse tree stored in the root
 * variable (this should be externed into programs which use Tetra parsing */

#include <iostream>
#include <fstream>
#include <string>
#include <stack>

using namespace std;

#include "tetra.hpp"

extern int yylineno;
int yylex( );
int yywrap( );
void yyerror(const char* str);

/* the root of the parse tree - this is set as a result of calling yyparse */
Node* root;

/* this function calls yyparse on a file and returns the parse tree node */
Node* parseFile(const string& fname);

/* this is a stack of line numbers, it is used for saving the appropriate line numbers
 * with each node.  eg. when a function is parsed, yylineno is the LAST line of the
 * function, so we save the start of the function before parsing the rest of it out */
stack<int> linenos;

%}

/* each non-terminal is represented with a node literlas are doubles */
%union {
  Node* node;
  TetraInt intval;
  TetraReal realval;
  TetraBool boolval;
  char stringval[256]; /* PODS only in union! */
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
%token TOK_BACKGROUND
%token TOK_LOCK

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
%type <node> functions function formal_param_list statements statement block formal_params
%type <node> compound_statement simple_statement pass_statement return_statement break_statement
%type <node> continue_statement expression if_statement while_statement else_option orterm andterm
%type <node> notterm relterm bitorterm xorterm bitandterm shiftterm plusterm timesterm unaryterm
%type <node> expterm funcall formal_param simple_statements actual_param_list variable assignterm
%type <node> elif_clause elif_clauses elif_statement


%type <data_type> return_type type

%error-verbose

%%

/* a program is a list of functions */
program: functions {
  root = $1;
}

/* zero or more new lines */
newl_star: TOK_NEWLINE newl_star {}
  | {}

/* one or more new lines */
newl_plus: TOK_NEWLINE newl_star {}

/* a list of functions */
functions: newl_star function functions {
  $$ = new Node(NODE_FUNCTION_LIST);
  $$->addChild($2);
  $$->addChild($3);
} | {
  $$ = NULL;
}

/* a single function */
function: TOK_DEF TOK_IDENTIFIER {linenos.push(yylineno);} formal_param_list return_type TOK_COLON block {
  $$ = new Node(NODE_FUNCTION);
  $$->setStringval(string($2));
  $$->setDataType($5);
  $$->addChild($4);
  $$->addChild($7);

  $$->setLine(linenos.top( ));
  linenos.pop( );
}

/* a parameter list (with bannanas) */
formal_param_list: TOK_LEFTPARENS formal_params TOK_RIGHTPARENS {
  $$ = $2;
  $$->setLine(yylineno);
} | TOK_LEFTPARENS TOK_RIGHTPARENS {
  $$ = NULL;
}

/* a list of at least one parameter */
formal_params: formal_param TOK_COMMA formal_params {
  $$ = new Node(NODE_FORMAL_PARAM_LIST);
  $$->setLine(yylineno);
  $$->addChild($1);
  $$->addChild($3);
} | formal_param {
  $$ = $1;
}

/* a single parameter */
formal_param: TOK_IDENTIFIER type {
  $$ = new Node(NODE_FORMAL_PARAM);
  $$->setLine(yylineno);
  $$->setStringval(string($1));
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
block: newl_plus TOK_INDENT statements TOK_DEDENT {
  $$ = $3;
}

/* a list of at least one statement */
statements: statement {linenos.push(yylineno);} statements {
  $$ = new Node(NODE_STATEMENT);
  $$->addChild($1);
  $$->addChild($3);

  $$->setLine(linenos.top( ));
  linenos.pop( );
} | statement {
  $$ = $1;
}

/* a single statement */
statement: simple_statements
  | compound_statement {
  $$ = $1;
}

/* simple statements are a list of simple statements separated by semi-conlons on one line */
simple_statements: simple_statement TOK_SEMICOLON simple_statements {
  $$ = new Node(NODE_STATEMENT);
  $$->setLine(yylineno);
  $$->addChild($1);
  $$->addChild($3);
} | simple_statement newl_plus {
  $$ = $1;
}

/* a simple statement is a one-liner that can't be broken down */
simple_statement: pass_statement
  | return_statement
  | break_statement
  | continue_statement
  | expression {
  $$ = $1;
  $$->setLine(yylineno);
}

/* a compound statement is one which has a block of code under it */
compound_statement: if_statement
  | elif_statement
/*  | for_statement */
  | while_statement {
  $$ = $1;
}

/* simple statements */
pass_statement: TOK_PASS {
  $$ = new Node(NODE_PASS);
  $$->setLine(yylineno);
}
return_statement: TOK_RETURN {
  $$ = new Node(NODE_RETURN);
  $$->setLine(yylineno);
} | TOK_RETURN {linenos.push(yylineno);} expression {
  $$ = new Node(NODE_RETURN);
  $$->addChild($3);
  $$->setLine(linenos.top( ));
  linenos.pop( );
}
break_statement: TOK_BREAK {
  $$ = new Node(NODE_BREAK);
  $$->setLine(yylineno);
}
continue_statement: TOK_CONTINUE {
  $$ = new Node(NODE_CONTINUE);
  $$->setLine(yylineno);
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

/* an elif statement */
elif_statement: TOK_IF expression TOK_COLON block elif_clauses else_option {
  $$ = new Node(NODE_ELIF);
  $$->addChild($2);
  $$->addChild($4);
  $$->addChild($5);
  if ($6) {
    $$->addChild($6);
  }
}

/* one or more elif clauses */
elif_clauses: elif_clause elif_clauses {
  $$ = new Node(NODE_ELIF_CHAIN);
  $$->addChild($1);
  $$->addChild($2);
} | elif_clause {
  $$ = $1;
}

/* a single elif clause */
elif_clause: TOK_ELIF expression TOK_COLON block {
  $$ = new Node(NODE_ELIF_CLAUSE);
  $$->addChild($2);
  $$->addChild($4);
}


/* a while loop */
while_statement: TOK_WHILE expression TOK_COLON block {
  $$ = new Node(NODE_WHILE);
  $$->addChild($2);
  $$->addChild($4);
}

/* expressions - assignments first */
expression: variable TOK_ASSIGN assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->addChild($3);
} | variable TOK_PLUSEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_PLUS);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | variable TOK_MINUSEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_MINUS);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | variable TOK_TIMESEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_TIMES);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | variable TOK_DIVIDEEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_DIVIDE);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | variable TOK_MODULUSEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_MODULUS);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | variable TOK_EXPEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_EXP);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | variable TOK_LSHIFTEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_SHIFTL);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | variable TOK_RSHIFTEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_SHIFTR);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | variable TOK_ANDEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_BITAND);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | variable TOK_OREQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_BITOR);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | variable TOK_XOREQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  Node* rhs = new Node(NODE_BITXOR);
  rhs->addChild($1);
  rhs->addChild($3);
  $$->addChild(rhs);
} | assignterm {
  $$ = $1;
}

assignterm: assignterm TOK_OR orterm {
  $$ = new Node(NODE_OR);
  $$->addChild($1);
  $$->addChild($3);
} | orterm {
  $$ = $1;
}

/* and operator */
orterm: orterm TOK_AND andterm {
  $$ = new Node(NODE_AND);
  $$->addChild($1);
  $$->addChild($3);
} | andterm {
  $$ = $1;
}

/* not operator */
andterm: TOK_NOT andterm {
  $$ = new Node(NODE_NOT);
  $$->addChild($2);
} | notterm {
  $$ = $1;
}

/* relational operators */
notterm: notterm TOK_LT relterm {
  $$ = new Node(NODE_LT);
  $$->addChild($1);
  $$->addChild($3);
} | notterm TOK_GT relterm {
  $$ = new Node(NODE_GT);
  $$->addChild($1);
  $$->addChild($3);
} | notterm TOK_LTE relterm {
  $$ = new Node(NODE_LTE);
  $$->addChild($1);
  $$->addChild($3);
} | notterm TOK_GTE relterm {
  $$ = new Node(NODE_GTE);
  $$->addChild($1);
  $$->addChild($3);
} | notterm TOK_EQ relterm {
  $$ = new Node(NODE_EQ);
  $$->addChild($1);
  $$->addChild($3);
} | notterm TOK_NEQ relterm {
  $$ = new Node(NODE_NEQ);
  $$->addChild($1);
  $$->addChild($3);
} | relterm {
  $$ = $1;
}

/* | operator */
relterm: relterm TOK_BITOR bitorterm {
  $$ = new Node(NODE_BITOR);
  $$->addChild($1);
  $$->addChild($3);
} | bitorterm {
  $$ = $1;
}

/* ^ operator */
bitorterm: bitorterm TOK_BITXOR xorterm {
  $$ = new Node(NODE_BITXOR);
  $$->addChild($1);
  $$->addChild($3);
} | xorterm {
  $$ = $1;
}

/* & operator */
xorterm: xorterm TOK_BITAND bitandterm {
  $$ = new Node(NODE_BITAND);
  $$->addChild($1);
  $$->addChild($3);
} | bitandterm {
  $$ = $1;
}

/* << and >> operator */
bitandterm: bitandterm TOK_LSHIFT shiftterm {
  $$ = new Node(NODE_SHIFTL);
  $$->addChild($1);
  $$->addChild($3);
} | bitandterm TOK_RSHIFT shiftterm {
  $$ = new Node(NODE_SHIFTR);
  $$->addChild($1);
  $$->addChild($3);
} | shiftterm {
  $$ = $1;
}

/* + and - operator */
shiftterm: shiftterm TOK_PLUS plusterm {
  $$ = new Node(NODE_PLUS);
  $$->addChild($1);
  $$->addChild($3);
} | shiftterm TOK_MINUS plusterm {
  $$ = new Node(NODE_MINUS);
  $$->addChild($1);
  $$->addChild($3);
} | plusterm {
  $$ = $1;
}

/* * / % operators */
plusterm: plusterm TOK_TIMES timesterm {
  $$ = new Node(NODE_TIMES);
  $$->addChild($1);
  $$->addChild($3);
} | plusterm TOK_DIVIDE timesterm {
  $$ = new Node(NODE_DIVIDE);
  $$->addChild($1);
  $$->addChild($3);
} | plusterm TOK_MODULUS timesterm {
  $$ = new Node(NODE_MODULUS);
  $$->addChild($1);
  $$->addChild($3);
} | timesterm {
  $$ = $1;
}

/* unary operators */
timesterm: TOK_PLUS timesterm {
  /* why would anybody do this??? */
  $$ = $2;
} | TOK_MINUS timesterm {
  /* subtract from zero */
  $$ = new Node(NODE_MINUS);
  Node* zero = new Node(NODE_INTVAL);
  zero->setIntval(0);
  $$->addChild(zero);
  $$->addChild($2);
} | TOK_BITNOT timesterm {
  $$ = new Node(NODE_BITNOT);
  $$->addChild($2);
} | unaryterm {
  $$ = $1;
}

/* exponent operator - this is right associative!!! */
unaryterm: expterm TOK_EXP unaryterm {
  $$ = new Node(NODE_EXP);
  $$->addChild($1);
  $$->addChild($3);
} | expterm {
  $$ = $1;
}

/* indivisible thing */
expterm: funcall {
  $$ = $1;
} | TOK_LEFTPARENS expression TOK_RIGHTPARENS {
  $$ = $2;
} | TOK_INTVAL {
  $$ = new Node(NODE_INTVAL);
  $$->setIntval($1);
} | TOK_REALVAL {
  $$ = new Node(NODE_REALVAL);
  $$->setRealval($1);
} | TOK_BOOLVAL {
  $$ = new Node(NODE_BOOLVAL);
  $$->setBoolval($1);
} | TOK_STRINGVAL {
  $$ = new Node(NODE_STRINGVAL);
  $$->setStringval($1);
} | variable {
  $$ = $1;
}

/* an l-value - will need to have vector refs too */
variable: TOK_IDENTIFIER {
  $$ = new Node(NODE_IDENTIFIER);
  $$->setStringval($1);
}

/* a function call */
funcall: TOK_IDENTIFIER TOK_LEFTPARENS TOK_RIGHTPARENS {
  $$ = new Node(NODE_FUNCALL);
  $$->setStringval($1);
} | TOK_IDENTIFIER TOK_LEFTPARENS actual_param_list TOK_RIGHTPARENS {
  $$ = new Node(NODE_FUNCALL);
  $$->setStringval($1);
  $$->addChild($3);
}

/* a list of at least one parameter */
actual_param_list: expression TOK_COMMA actual_param_list {
  $$ = new Node(NODE_ACTUAL_PARAM_LIST);
  $$->addChild($1);
  $$->addChild($3);
} | expression {
  $$ = $1;
}


%%

int yywrap( ) { 
  return 1;
}

void yyerror(const char* str) {
  throw Error(str, yylineno);
}

/* parse from a file */
extern istream* in;
Node* parseFile(const string& fname) {
  /* open the file */
  ifstream file(fname.c_str( ));

  /* if it's not open, we failed */
  if (!file.is_open( )) {
    throw Error("Could not open file '" + fname + "'");
  }
  
  /* set the in stream (defined in lexer.cpp) */
  in = &file;

  /* call yyparse */
  yyparse( );

  /* return the root of the parse tree */
  return root;
}

