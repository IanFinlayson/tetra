%{                                                                             
/* the parser for Tetra, this Bison parser generates a yyparse function which
 * is used to parse Tetra code from stdin into a parse tree stored in the root
 * variable (this should be externed into programs which use Tetra parsing */

#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#define YYDEBUG 1

using namespace std;

#include "frontend.h"

extern int yylineno;
int yylex( );
int yywrap( );
void yyerror(const char* str);

/* the root of the parse tree - this is set as a result of calling yyparse */
Node* root;

/* this function calls yyparse on a file and returns the parse tree node */
Node* parseFile(const string& fname);

%}

/* each non-terminal is represented with a node literlas are doubles */
%union {
  Node* node;
  int intval;
  double realval;
  bool boolval;
  char stringval[256]; /* PODS only in union! */
  DataType* data_type;
  int lineno;
}

/* typless tokens */
%token <lineno> TOK_IF 100
%token <lineno> TOK_ELIF 101
%token <lineno> TOK_ELSE 102
%token <lineno> TOK_FOR 103
%token <lineno> TOK_IN 104
%token <lineno> TOK_PARALLEL 105
%token <lineno> TOK_WHILE 106
%token <lineno> TOK_CONTINUE 107
%token <lineno> TOK_BREAK 108
%token <lineno> TOK_DEF 109
%token <lineno> TOK_OR 110
%token <lineno> TOK_AND 111
%token <lineno> TOK_NOT 112
%token <lineno> TOK_PASS 113
%token <lineno> TOK_RETURN 114
%token <lineno> TOK_INT 115
%token <lineno> TOK_REAL 116
%token <lineno> TOK_BOOL 117
%token <lineno> TOK_STRING 118
%token <lineno> TOK_ASSIGN 119
%token <lineno> TOK_PLUS 120
%token <lineno> TOK_MINUS 121
%token <lineno> TOK_TIMES 122
%token <lineno> TOK_DIVIDE 123
%token <lineno> TOK_MODULUS 124
%token <lineno> TOK_BITXOR 125
%token <lineno> TOK_BITAND 126
%token <lineno> TOK_BITOR 127
%token <lineno> TOK_BITNOT 128
%token <lineno> TOK_LEFTPARENS 129
%token <lineno> TOK_RIGHTPARENS 130
%token <lineno> TOK_LEFTBRACKET 131
%token <lineno> TOK_RIGHTBRACKET 132
%token <lineno> TOK_COMMA 133
%token <lineno> TOK_SEMICOLON 134
%token <lineno> TOK_COLON 135
%token <lineno> TOK_LSHIFT 136
%token <lineno> TOK_RSHIFT 137
%token <lineno> TOK_EXP 138
%token <lineno> TOK_LTE 139
%token <lineno> TOK_GTE 140
%token <lineno> TOK_EQ 141
%token <lineno> TOK_NEQ 142
%token <lineno> TOK_LT 143
%token <lineno> TOK_GT 144
%token <lineno> TOK_PLUSEQ 145
%token <lineno> TOK_MINUSEQ 146
%token <lineno> TOK_TIMESEQ 147
%token <lineno> TOK_DIVIDEEQ 148
%token <lineno> TOK_MODULUSEQ 149
%token <lineno> TOK_EXPEQ 150
%token <lineno> TOK_RSHIFTEQ 151
%token <lineno> TOK_LSHIFTEQ 152
%token <lineno> TOK_ANDEQ 153
%token <lineno> TOK_XOREQ 154
%token <lineno> TOK_OREQ 155
%token <lineno> TOK_ELLIPSIS 156
%token <lineno> TOK_BACKGROUND 157
%token <lineno> TOK_LOCK 158
%token <lineno> TOK_CONST 159
%token <lineno> TOK_GLOBAL 160
%token <lineno> TOK_RIGHTBRACE 169
%token <lineno> TOK_LEFTBRACE 170
%token <lineno> TOK_INIT 171
%token <lineno> TOK_LAMBDA 172
%token <lineno> TOK_WAIT 173
%token <lineno> TOK_SELF 174
%token <lineno> TOK_CLASS 175
%token <lineno> TOK_OPEN 176
%token <lineno> TOK_IMPORT 177
%token <lineno> TOK_MUTEX 178
%token <lineno> TOK_TASK 179
%token <lineno> TOK_NONE 180
%token <lineno> TOK_RIGHTARROW 181
%token <lineno> TOK_DOT 182

/* typed tokens */
%token <intval> TOK_INTVAL 161
%token <realval> TOK_REALVAL 162
%token <boolval> TOK_BOOLVAL 163
%token <stringval> TOK_STRINGVAL 164
%token <stringval> TOK_IDENTIFIER 165

/* dummy tokens */
%token TOK_INDENT 166
%token TOK_DEDENT 167
%token TOK_NEWLINE 168

/* types */
%type <node> toplevels function formal_param_list statements statement block formal_params
%type <node> compound_statement simple_statement pass_statement return_statement break_statement
%type <node> continue_statement expression if_statement while_statement else_option orterm andterm
%type <node> notterm relterm bitorterm xorterm bitandterm shiftterm plusterm timesterm unaryterm
%type <node> expterm funcall formal_param simple_statements actual_param_list variable assignterm
%type <node> elif_clause elif_clauses elif_statement for_statement identifier parblock parfor
%type <node> background lock_statement index indices vector_value vector_values datadecl 
%type <node> wait_statement

%type <data_type> return_type type tuple_types tuple_type_list

%error-verbose

%%

/* a program is a list of functions */
program: toplevels {
  root = $1;
}

/* zero or more new lines */
newl_star: TOK_NEWLINE newl_star {}
  | {}

/* one or more new lines */
newl_plus: TOK_NEWLINE newl_star {}

/* a list of functions */
toplevels: newl_star function toplevels {
  $$ = new Node(NODE_TOPLEVEL_LIST);
  $$->setLine(0);
  $$->addChild($2);
  $$->addChild($3);
} | newl_star datadecl toplevels {
  $$ = new Node(NODE_TOPLEVEL_LIST);
  $$->setLine(0);
  $$->addChild($2);
  $$->addChild($3);
} | {
  $$ = NULL;
}

/* a data declaration - either a constant or global */
datadecl: TOK_CONST identifier TOK_ASSIGN expression {
  $$ = new Node(NODE_CONST);
  $$->addChild($2);
  $$->addChild($4);
} | TOK_GLOBAL identifier TOK_ASSIGN expression {
  $$ = new Node(NODE_GLOBAL);
  $$->addChild($2);
  $$->addChild($4);
}

/* a single function */
function: TOK_DEF TOK_IDENTIFIER formal_param_list return_type TOK_COLON block {
  $$ = new Node(NODE_FUNCTION);
  $$->setStringval(string($2));
  $$->setDataType($4);
  $$->addChild($3);
  $$->addChild($6);
  $$->setLine($1);
}

/* a parameter list (with bananas) */
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

/* tuple types */
tuple_type_list: TOK_LEFTPARENS tuple_types TOK_RIGHTPARENS {
 /* TODO */ 
  $$ = new DataType(TYPE_TUPLE);
} | TOK_LEFTPARENS TOK_RIGHTPARENS {
  $$ = NULL;
}

/* a list of at least one parameter */
tuple_types: type TOK_COMMA tuple_types {
 /* TODO */ 
} | type {
 /* TODO */ 
}

/* types just primitives and vectors for now */
type: TOK_INT {
  $$ = new DataType(TYPE_INT);
} | TOK_REAL {
  $$ = new DataType(TYPE_REAL);
} | TOK_STRING {
  $$ = new DataType(TYPE_STRING);
} | TOK_BOOL {
  $$ = new DataType(TYPE_BOOL);
} | TOK_MUTEX {
  $$ = new DataType(TYPE_MUTEX);
} | TOK_TASK {
  $$ = new DataType(TYPE_TASK);
} | TOK_LEFTBRACKET type TOK_RIGHTBRACKET {
  $$ = new DataType(TYPE_VECTOR);
  $$->addSubtype($2);
} | tuple_type_list{
  $$ = $1;
}

/* a return type is either a simple type or none which means void */
return_type: type {
  $$ = $1;
} | {
  $$ = new DataType(TYPE_VOID);
}

/* a block is a set of statements, indented over */
block: newl_plus TOK_INDENT statements TOK_DEDENT {
  $$ = $3;
}

/* a list of at least one statement */
statements: statement statements {
  $$ = new Node(NODE_STATEMENT);
  $$->addChild($1);
  $$->addChild($2);
  $$->setLine($1->getLine( ));
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
  | wait_statement
  | expression {
  $$ = $1;
  $$->setLine(yylineno);
}

/* a compound statement is one which has a block of code under it */
compound_statement: if_statement
  | elif_statement
  | for_statement
  | while_statement 
  | parfor
  | background
  | lock_statement
  | parblock {
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

} | TOK_RETURN expression {
  $$ = new Node(NODE_RETURN);
  $$->addChild($2);
  $$->setLine($1);
}
break_statement: TOK_BREAK {
  $$ = new Node(NODE_BREAK);
  $$->setLine(yylineno);
}
continue_statement: TOK_CONTINUE {
  $$ = new Node(NODE_CONTINUE);
  $$->setLine(yylineno);
}
wait_statement: TOK_WAIT identifier{
  $$ = new Node(NODE_WAIT);
  $$->addChild($2);
  $$->setLine(yylineno);
}

/* if statement with or without else */
if_statement: TOK_IF expression TOK_COLON block else_option {
  $$ = new Node(NODE_IF);
  $$->addChild($2);
  $$->addChild($4);
  $$->setLine($1);

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
  
  /* make a node for the first clause */
  Node* c1 = new Node(NODE_ELIF_CLAUSE);
  c1->addChild($2);
  c1->addChild($4);
  $$->addChild(c1);
  $$->setLine($1);

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
  $$->setLine($1->getLine( ));
} | elif_clause {
  $$ = $1;
}

/* a single elif clause */
elif_clause: TOK_ELIF expression TOK_COLON block {
  $$ = new Node(NODE_ELIF_CLAUSE);
  $$->addChild($2);
  $$->addChild($4);
  $$->setLine($1);
}

/* a for loop */
for_statement: TOK_FOR identifier TOK_IN expression TOK_COLON block {
  $$ = new Node(NODE_FOR);
  $$->addChild($2);
  $$->addChild($4);
  $$->addChild($6);
  $$->setLine($1);
}

/* a parallel for loop */
parfor: TOK_PARALLEL TOK_FOR identifier TOK_IN expression TOK_COLON block {
  $$ = new Node(NODE_PARFOR);
  $$->addChild($3);
  $$->addChild($5);
  $$->addChild($7);
  $$->setLine($1);
}

/* a while loop */
while_statement: TOK_WHILE expression TOK_COLON block {
  $$ = new Node(NODE_WHILE);
  $$->addChild($2);
  $$->addChild($4);
  $$->setLine($1);
}

/* a parallel block */
parblock: TOK_PARALLEL TOK_COLON block {
  $$ = new Node(NODE_PARALLEL);
  $$->addChild($3);
  $$->setLine($1);
}

/* a background block */
background: TOK_BACKGROUND TOK_COLON block {
  $$ = new Node(NODE_BACKGROUND);
  $$->addChild($3);
  $$->setLine($1);

} | TOK_BACKGROUND identifier TOK_COLON block {
  $$ = new Node(NODE_BACKGROUND);
  $$->addChild($2);
  $$->addChild($4);
  $$->setLine($1);
}

/* a lock statement */
lock_statement: TOK_LOCK TOK_COLON block {
  $$ = new Node(NODE_LOCK);
  $$->addChild($3);
  $$->setLine($1);
}

lock_statement: TOK_LOCK identifier TOK_COLON block {
  $$ = new Node(NODE_LOCK);
  $$->addChild($2);
  $$->addChild($4);
  $$->setLine($1);
}

/* expressions - assignments first */
expression: variable TOK_ASSIGN assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | variable TOK_PLUSEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_PLUS);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | variable TOK_MINUSEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_MINUS);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | variable TOK_TIMESEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_TIMES);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | variable TOK_DIVIDEEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_DIVIDE);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | variable TOK_MODULUSEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_MODULUS);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | variable TOK_EXPEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_EXP);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | variable TOK_LSHIFTEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_SHIFTL);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | variable TOK_RSHIFTEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_SHIFTR);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | variable TOK_ANDEQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_BITAND);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | variable TOK_OREQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_BITOR);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | variable TOK_XOREQ assignterm {
  $$ = new Node(NODE_ASSIGN);
  $$->addChild($1);
  $$->setLine($2);
  Node* rhs = new Node(NODE_BITXOR);
  rhs->addChild($1);
  rhs->addChild($3);
  rhs->setLine($2);
  $$->addChild(rhs);
} | assignterm {
  $$ = $1;
}

assignterm: assignterm TOK_OR orterm {
  $$ = new Node(NODE_OR);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | orterm {
  $$ = $1;
}

/* and operator */
orterm: orterm TOK_AND andterm {
  $$ = new Node(NODE_AND);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
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
  $$->setLine($2);
} | notterm TOK_GT relterm {
  $$ = new Node(NODE_GT);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | notterm TOK_LTE relterm {
  $$ = new Node(NODE_LTE);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | notterm TOK_GTE relterm {
  $$ = new Node(NODE_GTE);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | notterm TOK_EQ relterm {
  $$ = new Node(NODE_EQ);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | notterm TOK_NEQ relterm {
  $$ = new Node(NODE_NEQ);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | relterm {
  $$ = $1;
}

/* | operator */
relterm: relterm TOK_BITOR bitorterm {
  $$ = new Node(NODE_BITOR);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | bitorterm {
  $$ = $1;
}

/* ^ operator */
bitorterm: bitorterm TOK_BITXOR xorterm {
  $$ = new Node(NODE_BITXOR);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | xorterm {
  $$ = $1;
}

/* & operator */
xorterm: xorterm TOK_BITAND bitandterm {
  $$ = new Node(NODE_BITAND);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | bitandterm {
  $$ = $1;
}

/* << and >> operator */
bitandterm: bitandterm TOK_LSHIFT shiftterm {
  $$ = new Node(NODE_SHIFTL);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | bitandterm TOK_RSHIFT shiftterm {
  $$ = new Node(NODE_SHIFTR);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | shiftterm {
  $$ = $1;
}

/* + and - operator */
shiftterm: shiftterm TOK_PLUS plusterm {
  $$ = new Node(NODE_PLUS);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | shiftterm TOK_MINUS plusterm {
  $$ = new Node(NODE_MINUS);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | plusterm {
  $$ = $1;
}

/* * / % operators */
plusterm: plusterm TOK_TIMES timesterm {
  $$ = new Node(NODE_TIMES);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | plusterm TOK_DIVIDE timesterm {
  $$ = new Node(NODE_DIVIDE);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
} | plusterm TOK_MODULUS timesterm {
  $$ = new Node(NODE_MODULUS);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
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
  $$->setLine($2);
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
} | vector_value {
  $$ = $1;
} | variable {
  $$ = $1;
}

/* a vector literal */
vector_value: TOK_LEFTBRACKET TOK_RIGHTBRACKET {
  /* an empty vector definition */
  $$ = new Node(NODE_VECVAL);

} | TOK_LEFTBRACKET expression TOK_ELLIPSIS expression TOK_RIGHTBRACKET {
  /* a vector with elipsis eg [1 .. 5] */
/* create a range node for these (this code used to build up a chain of VECVAL's manually, but that
       killed memory when we did [1 ... 100000] */
  $$ = new Node(NODE_VECRANGE);

  /* check that the values are legit */
  $$->addChild($2);
  $$->addChild($4);

} | TOK_LEFTBRACKET vector_values TOK_RIGHTBRACKET {
  /* a set of one or more vector initializers */
  $$ = $2;
}

/* one or more expressions to be made into a vector */
vector_values: expression TOK_COMMA vector_values {
             $$ = new Node(NODE_VECVAL);
  $$->addChild($1);
  $$->addChild($3);
} | expression {
  $$ = new Node(NODE_VECVAL);
  $$->addChild($1);
}



/* an l-value - any identifier with any number of indexes after it */
variable: identifier indices {
  /* if it's a vector reference */
  if ($2) {
    $$ = new Node(NODE_VECREF);
    $$->addChild($1);
    $$->addChild($2);
  } else {
    /* just a humble identifier */
    $$ = $1;
  }
}

/* any number of indices */
indices: index indices {
  $$ = new Node(NODE_INDEX);
  $$->addChild($1);
  $$->addChild($2);
} | {
  $$ = NULL;
}

/* a single index */
index: TOK_LEFTBRACKET expression TOK_RIGHTBRACKET {
  $$ = $2;
}

/* a node wrapper around an ID */
identifier: TOK_IDENTIFIER {
  $$ = new Node(NODE_IDENTIFIER);
  $$->setStringval($1);
}

/* a function call */
funcall: TOK_IDENTIFIER TOK_LEFTPARENS TOK_RIGHTPARENS {
  $$ = new Node(NODE_FUNCALL);
  $$->setStringval($1);
  $$->setLine($2);

} | TOK_IDENTIFIER TOK_LEFTPARENS actual_param_list TOK_RIGHTPARENS {
  $$ = new Node(NODE_FUNCALL);
  $$->setStringval($1);
  $$->addChild($3);
  $$->setLine($2);
}

/* a list of at least one parameter */
actual_param_list: expression TOK_COMMA actual_param_list {
  $$ = new Node(NODE_ACTUAL_PARAM_LIST);
  $$->addChild($1);
  $$->addChild($3);
  $$->setLine($2);
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

    reset_lexer( );

    /* open the file */
    ifstream file(fname.c_str( ));

    /* if it's not open, we failed */
    if (!file.is_open( )) {
        throw Error("Could not open file '" + fname + "'");
    }

    /* set the in stream (defined in lexer.cpp) */
    in = &file;

    /* Check parsing only */
    /*int token;
    while ((token = yylex())) {
      printf("%d\n", token); 
    }
    */
    
    /* call yyparse */
    yyparse();

    dumpTreeGraphviz(root);
    exit(0);

    /* check and infer the types in the tree */
    inferTypes(root);
    /* return the root of the parse tree */
    return root;
}
