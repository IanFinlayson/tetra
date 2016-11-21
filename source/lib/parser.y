%{                                                                             
/* the parser for Tetra, this Bison parser generates a yyparse function which
 * is used to parse Tetra code from stdin into a parse tree stored in the root
 * variable (this should be externed into programs which use Tetra parsing */

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <QDebug>
#include <QFile>

#define YYDEBUG 1

#include "node.h"
#include "types.h"
#include "error.h"
#include "lexer.h"
#include "symbol.h"

extern int yylineNumber;
int yylex( );
int yywrap( );
void yyerror(const char* str);

/* the root of the parse tree - this is set as a result of calling yyparse */
Node* root;

%}

/* each non-terminal is represented with a node literlas are doubles */
%union {
    Node* node;

    /* PODS only in union! */
    Int* intValue;
    Real* realValue;
    Bool* boolValue;
    String* stringval;

    DataType* dataType;
    int lineNumber;
}

/* typless tokens */
%token <lineNumber> TOK_IF 100
%token <lineNumber> TOK_ELIF 101
%token <lineNumber> TOK_ELSE 102
%token <lineNumber> TOK_FOR 103
%token <lineNumber> TOK_IN 104
%token <lineNumber> TOK_PARALLEL 105
%token <lineNumber> TOK_WHILE 106
%token <lineNumber> TOK_CONTINUE 107
%token <lineNumber> TOK_BREAK 108
%token <lineNumber> TOK_DEF 109
%token <lineNumber> TOK_OR 110
%token <lineNumber> TOK_AND 111
%token <lineNumber> TOK_NOT 112
%token <lineNumber> TOK_PASS 113
%token <lineNumber> TOK_RETURN 114
%token <lineNumber> TOK_INT 115
%token <lineNumber> TOK_REAL 116
%token <lineNumber> TOK_BOOL 117
%token <lineNumber> TOK_STRING 118
%token <lineNumber> TOK_ASSIGN 119
%token <lineNumber> TOK_PLUS 120
%token <lineNumber> TOK_MINUS 121
%token <lineNumber> TOK_TIMES 122
%token <lineNumber> TOK_DIVIDE 123
%token <lineNumber> TOK_MODULUS 124
%token <lineNumber> TOK_BITXOR 125
%token <lineNumber> TOK_BITAND 126
%token <lineNumber> TOK_BITOR 127
%token <lineNumber> TOK_BITNOT 128
%token <lineNumber> TOK_LEFTPARENS 129
%token <lineNumber> TOK_RIGHTPARENS 130
%token <lineNumber> TOK_LEFTBRACKET 131
%token <lineNumber> TOK_RIGHTBRACKET 132
%token <lineNumber> TOK_COMMA 133
%token <lineNumber> TOK_SEMICOLON 134
%token <lineNumber> TOK_COLON 135
%token <lineNumber> TOK_LSHIFT 136
%token <lineNumber> TOK_RSHIFT 137
%token <lineNumber> TOK_EXP 138
%token <lineNumber> TOK_LTE 139
%token <lineNumber> TOK_GTE 140
%token <lineNumber> TOK_EQ 141
%token <lineNumber> TOK_NEQ 142
%token <lineNumber> TOK_LT 143
%token <lineNumber> TOK_GT 144
%token <lineNumber> TOK_PLUSEQ 145
%token <lineNumber> TOK_MINUSEQ 146
%token <lineNumber> TOK_TIMESEQ 147
%token <lineNumber> TOK_DIVIDEEQ 148
%token <lineNumber> TOK_MODULUSEQ 149
%token <lineNumber> TOK_EXPEQ 150
%token <lineNumber> TOK_RSHIFTEQ 151
%token <lineNumber> TOK_LSHIFTEQ 152
%token <lineNumber> TOK_ANDEQ 153
%token <lineNumber> TOK_XOREQ 154
%token <lineNumber> TOK_OREQ 155
%token <lineNumber> TOK_ELLIPSIS 156
%token <lineNumber> TOK_BACKGROUND 157
%token <lineNumber> TOK_LOCK 158
%token <lineNumber> TOK_CONST 159
%token <lineNumber> TOK_GLOBAL 160
%token <lineNumber> TOK_RIGHTBRACE 169
%token <lineNumber> TOK_LEFTBRACE 170
%token <lineNumber> TOK_INIT 171
%token <lineNumber> TOK_LAMBDA 172
%token <lineNumber> TOK_WAIT 173
%token <lineNumber> TOK_SELF 174
%token <lineNumber> TOK_CLASS 175
%token <lineNumber> TOK_OPEN 176
%token <lineNumber> TOK_IMPORT 177
%token <lineNumber> TOK_MUTEX 178
%token <lineNumber> TOK_TASK 179
%token <lineNumber> TOK_NONE 180
%token <lineNumber> TOK_RIGHTARROW 181
%token <lineNumber> TOK_DOT 182
%token <lineNumber> TOK_AS 183

/* typed tokens */
%token <intValue> TOK_INTVAL 161
%token <realValue> TOK_REALVAL 162
%token <boolValue> TOK_BOOLVAL 163
%token <stringval> TOK_STRINGVAL 164
%token <stringval> TOK_IDENTIFIER 165

/* dummy tokens */
%token TOK_INDENT 166
%token TOK_DEDENT 167
%token TOK_NEWLINE 168

/* types */
%type <node> toplevels function formal_param_list statements statement block formal_params
             compound_statement simple_statement pass_statement return_statement break_statement
             continue_statement expression if_statement while_statement else_option orterm andterm
             notterm relterm bitorterm xorterm bitandterm shiftterm plusterm timesterm unaryterm
             expterm functionCall simple_statements actual_param_list rvalue assignterm
             elif_clause elif_clauses elif_statement for_statement identifier parblock parfor
             background lock_statement index list_value list_values datadecl 
             wait_statement declaration lambda identifiers module tuple_value tuple_values
             dict_value dict_values typed_identifier class class_block class_parts class_part
             init_function lvalue type_decs lambdaterm interm funcname

%type <dataType> return_type type type_dec_tuple function_type dict_type

%error-verbose

%%

/* a program is a list of functions */
program: toplevels {
    root = $1;
}

/* zero or more new lines */
newl_star: TOK_NEWLINE newl_star {}
         | {}

/* one or more new lines, or end of file */
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
} | newl_star module toplevels { 
    $$ = new Node(NODE_TOPLEVEL_LIST);
    $$->setLine(0);
    $$->addChild($2);
    $$->addChild($3);
} | newl_star class toplevels {
    $$ = new Node(NODE_TOPLEVEL_LIST);
    $$->setLine(0);
    $$->addChild($2);
    $$->addChild($3);
} | {
    $$ = NULL;
}

/* modules */
identifiers: identifier TOK_COMMA identifiers {
    $$ = new Node(NODE_IDENTIFIERS);
    $$->setLine(yylineNumber);
    $$->addChild($1);
    $$->addChild($3);
} | identifier {
    $$ = new Node(NODE_IDENTIFIERS);
    $$->setLine(yylineNumber);
    $$->addChild($1);
}

module: TOK_OPEN identifiers {
    $$ = new Node(NODE_OPEN); 
    $$->setLine(yylineNumber);
    $$->addChild($2);

} | TOK_IMPORT identifiers {
    $$ = new Node(NODE_IMPORT); 
    $$->setLine(yylineNumber);
    $$->addChild($2);
}


/* class */
class: TOK_CLASS TOK_IDENTIFIER TOK_COLON newl_plus class_block {
    $$ = new Node(NODE_CLASS);
    $$->setStringvalue(String(*$2));
    delete $2;
    $$->addChild($5);
}

class_block: TOK_INDENT class_parts TOK_DEDENT {
    $$ = $2;
} | TOK_INDENT pass_statement newl_star TOK_DEDENT{
    $$ = $2;
}

class_parts: class_part class_parts {
    $$ = new Node(NODE_CLASS_PART);
    $$->addChild($1);
    $$->addChild($2);
    $$->setLine($1->getLine( ));
} | class_part {
    $$ = new Node(NODE_CLASS_PART);
    $$->addChild($1);
}

class_part: function 
          | typed_identifier newl_plus
          | init_function {
    $$ = $1;
}

init_function: TOK_DEF TOK_INIT formal_param_list TOK_COLON block {
    $$ = new Node(NODE_FUNCTION);
    $$->setDataType(DataType(TYPE_CLASS));
    $$->setStringvalue(String("init"));
    $$->addChild($3);
    $$->addChild($5);
    $$->setLine($1);
} 

/* a data declaration - either a constant or global */
datadecl: TOK_CONST identifier TOK_ASSIGN assignterm {
    $$ = new Node(NODE_CONST);
    $$->addChild($2);
    $$->addChild($4);
} | TOK_CONST typed_identifier TOK_ASSIGN assignterm {
    $$ = new Node(NODE_CONST);
    $$->addChild($2);
    $$->addChild($4);
} | TOK_CONST identifier type TOK_ASSIGN assignterm {
    $$ = new Node(NODE_CONST);
    $$->addChild($2);
    $$->addChild($5);
    $2->setDataType(*$3);
    delete $3;
} | TOK_GLOBAL typed_identifier TOK_ASSIGN assignterm {
    $$ = new Node(NODE_GLOBAL);
    $$->addChild($2);
    $$->addChild($4);
} | TOK_GLOBAL identifier TOK_ASSIGN assignterm {
    $$ = new Node(NODE_GLOBAL);
    $$->addChild($2);
    $$->addChild($4);
} | TOK_GLOBAL identifier type TOK_ASSIGN assignterm {
    $$ = new Node(NODE_GLOBAL);
    $$->addChild($2);
    $$->addChild($5);
    $2->setDataType(*$3);
    delete $3;
} | TOK_GLOBAL identifier type {
    $$ = new Node(NODE_GLOBAL);
    $$->addChild($2);
    $2->setDataType(*$3);
    delete $3;
} | TOK_GLOBAL typed_identifier {
    $$ = new Node(NODE_GLOBAL);
    $$->addChild($2);
}

/* a single function */
function: TOK_DEF TOK_IDENTIFIER formal_param_list return_type TOK_COLON block {
    $$ = new Node(NODE_FUNCTION);
    $$->setStringvalue(String(*$2));
    delete $2;
    $$->setDataType(*$4);
    delete $4;
    $$->addChild($3);
    $$->addChild($6);
    $$->setLine($1);
} 

/* a parameter list (with bananas) */
formal_param_list: TOK_LEFTPARENS formal_params TOK_RIGHTPARENS {
    $$ = $2;
    $$->setLine(yylineNumber);
} | TOK_LEFTPARENS TOK_RIGHTPARENS {
    $$ = NULL;
}

/* a list of at least one parameter */
formal_params: declaration TOK_COMMA formal_params {
    $$ = new Node(NODE_FORMAL_PARAM_LIST);
    $$->setLine(yylineNumber);
    $$->addChild($1);
    $$->addChild($3);
} | declaration{
    $$ = new Node(NODE_FORMAL_PARAM_LIST);
    $$->addChild($1);
}

/* a single parameter */
declaration: TOK_IDENTIFIER type {
    $$ = new Node(NODE_DECLARATION);
    $$->setLine(yylineNumber);
    $$->setStringvalue(String(*$1));
    delete $1;
    $$->setDataType(*$2);
    delete $2;
} 

/* tuple types */
type_dec_tuple: TOK_LEFTPARENS type_decs TOK_RIGHTPARENS {
    $$ = new DataType(TYPE_TUPLE);
    Node* decs = $2;
  while(decs){
    $$->subtypes->push_back(*decs->type());
    decs = decs->child(0);
  }
} | TOK_LEFTPARENS type TOK_COMMA TOK_RIGHTPARENS {
    $$ = new DataType(TYPE_TUPLE);
    $$->subtypes->push_back(*$2);
    delete $2;
} | TOK_LEFTPARENS TOK_RIGHTPARENS {
    $$ = new DataType(TYPE_TUPLE);
}

type_decs: type TOK_COMMA type_decs {
    $$ = new Node(NODE_TUPLE_TYPES);
    $$->setDataType(*$1);
    delete $1;
    $$->addChild($3);
} | type {
    $$ = new Node(NODE_TUPLE_TYPES);
    $$->setDataType(*$1);
    delete $1;
}

/* types just primitives and lists for now */
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
    $$ = new DataType(TYPE_LIST);
    $$->subtypes->push_back(*$2);
    delete $2;
} | type_dec_tuple {
    $$ = $1;
} | function_type {
    $$ = $1;
} | dict_type { 
    $$ = $1;
} | TOK_IDENTIFIER {
    $$ = new DataType(TYPE_CLASS);
    $$->className = new String(*$1);
    delete $1;
}

/* function_type */
function_type: type_dec_tuple TOK_RIGHTARROW return_type{
    $$ = new DataType(TYPE_FUNCTION);
    $$->subtypes->push_back(*$1);
    $$->subtypes->push_back(*$3);
    delete $1;
    delete $3;
}

/* dict_type */
dict_type: TOK_LEFTBRACE type TOK_COLON type TOK_RIGHTBRACE {
    $$ = new DataType(TYPE_DICT);
    $$->subtypes->push_back(*$2);
    $$->subtypes->push_back(*$4);
    delete $2;
    delete $4;
}

/* a return type is either a simple type or none which means void */
return_type: type {
    $$ = $1;
} | TOK_NONE {
    $$ = new DataType(TYPE_NONE);
}  | {
    $$ = new DataType(TYPE_NONE);
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
    $$->setLine(yylineNumber);
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
    $$->setLine(yylineNumber);
} | typed_identifier {
    $$ = $1;
} | typed_identifier TOK_ASSIGN assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->addChild($3);
    $$->setLine($2);
}

typed_identifier: identifier TOK_AS type {
    $$ = $1;
    $$->setLine(yylineNumber);
    $1->setDataType(*$3);
    delete $3;
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
    $$->setLine(yylineNumber);
}
return_statement: TOK_RETURN {
    $$ = new Node(NODE_RETURN);
    $$->setLine(yylineNumber);

} | TOK_RETURN expression {
    $$ = new Node(NODE_RETURN);
    $$->addChild($2);
    $$->setLine($1);
}
break_statement: TOK_BREAK {
    $$ = new Node(NODE_BREAK);
    $$->setLine(yylineNumber);
}
continue_statement: TOK_CONTINUE {
    $$ = new Node(NODE_CONTINUE);
    $$->setLine(yylineNumber);
}
wait_statement: TOK_WAIT identifier{
    $$ = new Node(NODE_WAIT);
    $$->addChild($2);
    $$->setLine(yylineNumber);
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
expression: lvalue TOK_ASSIGN expression {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->addChild($3);
    $$->setLine($2);
} | lvalue TOK_PLUSEQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_PLUS);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | lvalue TOK_MINUSEQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_MINUS);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | lvalue TOK_TIMESEQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_TIMES);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | lvalue TOK_DIVIDEEQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_DIVIDE);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | lvalue TOK_MODULUSEQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_MODULUS);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | lvalue TOK_EXPEQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_EXP);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | lvalue TOK_LSHIFTEQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_SHIFTL);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | lvalue TOK_RSHIFTEQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_SHIFTR);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | lvalue TOK_ANDEQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_BITAND);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | lvalue TOK_OREQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_BITOR);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | lvalue TOK_XOREQ assignterm {
    $$ = new Node(NODE_ASSIGN);
    $$->addChild($1);
    $$->setLine($2);
    Node* rhs = new Node(NODE_BITXOR);
    rhs->addChild(cloneTree($1));
    rhs->addChild($3);
    rhs->setLine($2);
    $$->addChild(rhs);
} | assignterm {
    $$ = $1;
}

assignterm: lambda
          | lambdaterm{
    $$ = $1;
}

lambdaterm: lambdaterm TOK_OR orterm {
    $$ = new Node(NODE_OR);
    $$->addChild($1);
    $$->addChild($3);
    $$->setLine($2);
} | orterm {
    $$ = $1;
}

/* lambda function */
lambda: TOK_LAMBDA formal_params TOK_COLON assignterm {
    $$ = new Node(NODE_LAMBDA);
    $$->addChild($2);
    $$->addChild($4);
    $$->setLine($1);
} | TOK_LAMBDA TOK_COLON assignterm {
    $$ = new Node(NODE_LAMBDA);
    $$->addChild($3);
    $$->setLine($1);
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
    $$ = new Node(NODE_UMINUS);
    $$->addChild($2);
} | TOK_BITNOT timesterm {
    $$ = new Node(NODE_BITNOT);
    $$->addChild($2);
} | unaryterm {
    $$ = $1;
}

/* exponent operator - this is right associative!!! */
unaryterm: interm TOK_EXP unaryterm {
    $$ = new Node(NODE_EXP);
    $$->addChild($1);
    $$->addChild($3);
    $$->setLine($2);
} | interm {
    $$ = $1;
}

/* in operator */
interm: interm TOK_IN expterm {
    $$ = new Node(NODE_IN);
    $$->addChild($1);
    $$->addChild($3);
    $$->setLine($2);
} |expterm {
    $$ = $1;
}

/* expterm */
expterm: expterm TOK_DOT functionCall {
    $$ = new Node(NODE_METHOD_CALL);
    $$->addChild($1);
    $$->addChild($3);
    $$->setLine($2);
} | rvalue {
    $$ = $1;
} 


/* indivisible thing */
rvalue: functionCall {
    $$ = $1;
} | TOK_LEFTPARENS expression TOK_RIGHTPARENS {
    $$ = $2;
} | TOK_INTVAL {
    $$ = new Node(NODE_INTVAL);
    $$->setIntvalue(Int(*$1));
    delete $1;
} | TOK_REALVAL {
    $$ = new Node(NODE_REALVAL);
    $$->setRealvalue(Real((*$1)));
    delete $1;
} | TOK_BOOLVAL {
    $$ = new Node(NODE_BOOLVAL);
    $$->setBoolvalue(Bool((*$1)));
    delete $1;
} | TOK_STRINGVAL {
    $$ = new Node(NODE_STRINGVAL);
    $$->setStringvalue(String(*$1));
    delete $1;
} | TOK_NONE {
    $$ = new Node(NODE_NONEVAL);
} | list_value {
    $$ = $1;
} | tuple_value {
    $$ = $1;
} | dict_value {
    $$ = $1;
} | TOK_SELF { 
    $$ = new Node(NODE_SELF);
} | lvalue {
    $$ = $1;
}

lvalue: expterm index {
    $$ = new Node(NODE_INDEX);
    $$->addChild($1);
    $$->addChild($2);
} | expterm TOK_DOT identifier { 
    $$ = new Node(NODE_DOT);
    $$->addChild($1);
    $$->addChild($3);
    $$->setLine($2);
} | identifier {
    $$ = $1;
} 
/* a list literal */
list_value: TOK_LEFTBRACKET TOK_RIGHTBRACKET {
    /* an empty list definition */
    $$ = new Node(NODE_LISTVAL);

} | TOK_LEFTBRACKET expression TOK_ELLIPSIS expression TOK_RIGHTBRACKET {
    /* a list with elipsis eg [1 .. 5] */
    $$ = new Node(NODE_LISTRANGE);

    /* check that the values are legit */
    $$->addChild($2);
    $$->addChild($4);

} | TOK_LEFTBRACKET list_values TOK_RIGHTBRACKET {
    /* a set of one or more list initializers */
    $$ = $2;
}

/* one or more expressions to be made into a list */
list_values: expression TOK_COMMA list_values {
    $$ = new Node(NODE_LISTVAL);
    $$->addChild($1);
    $$->addChild($3);
} | expression {
    $$ = new Node(NODE_LISTVAL);
    $$->addChild($1);
}

/* a tuple literal */
tuple_value: TOK_LEFTPARENS TOK_RIGHTPARENS {
    /* an empty list definition */
    $$ = new Node(NODE_TUPVAL);
} | TOK_LEFTPARENS expression TOK_COMMA TOK_RIGHTPARENS {
    /* one tuple initializer */
    $$ = new Node(NODE_TUPVAL); 
    $$->addChild($2);
} | TOK_LEFTPARENS expression TOK_COMMA tuple_values TOK_RIGHTPARENS{
    /* a set of two or more tuple initializers */
    $$ = new Node(NODE_TUPVAL); 
    $$->addChild($2);
    $$->addChild($4);
} 

/* zero or more expressions to be made into a tuple */
tuple_values: expression TOK_COMMA tuple_values {
    $$ = new Node(NODE_TUPVAL);
    $$->addChild($1);
    $$->addChild($3);
} | expression {
    $$ = new Node(NODE_TUPVAL);
    $$->addChild($1);
}

/* a dictionary literal */
dict_value: TOK_LEFTBRACE TOK_RIGHTBRACE {
    /* an empty dictionary */
    $$ = new Node(NODE_DICTVAL);
} | TOK_LEFTBRACE dict_values TOK_RIGHTBRACE {
    /* a literal dictionary with one or more key-value pairs */
    $$ = $2;
}

/* one or more expressions to be made into a list */
dict_values: expression TOK_COLON expression TOK_COMMA dict_values {
    $$ = new Node(NODE_DICTVAL);
    $$->addChild($1);
    $$->addChild($3);
    $$->addChild($5);
} | expression TOK_COLON expression {
    $$ = new Node(NODE_DICTVAL);
    $$->addChild($1);
    $$->addChild($3);
}

/* a single index */
index: TOK_LEFTBRACKET expression TOK_RIGHTBRACKET { 
    $$ = $2; 
}

/* a node wrapper around an ID */
identifier: TOK_IDENTIFIER {
    $$ = new Node(NODE_IDENTIFIER);
    $$->setStringvalue(String(*$1));
    delete $1;
    $$->setLine(yylineNumber);
}  

/* a function call */
functionCall: funcname TOK_LEFTPARENS TOK_RIGHTPARENS {
    $$ = new Node(NODE_FUNCALL);
    $$->addChild($1);
    $$->setLine($2);

} | funcname TOK_LEFTPARENS actual_param_list TOK_RIGHTPARENS {
    $$ = new Node(NODE_FUNCALL);
    $$->addChild($1);
    $$->addChild($3);
    $$->setLine($2);
  }

/* a functiona name is either a plain id or a basic types
   these can be called for type conversions a la Python */
funcname: identifier {
        $$ = $1;
} | TOK_INT {
    /* make an id node for this */
    $$ = new Node(NODE_IDENTIFIER);
    $$->setStringvalue(String("int"));
    $$->setLine($1);
} | TOK_REAL {
    /* make an id node for this */
    $$ = new Node(NODE_IDENTIFIER);
    $$->setStringvalue(String("real"));
    $$->setLine($1);
} | TOK_STRING {
    /* make an id node for this */
    $$ = new Node(NODE_IDENTIFIER);
    $$->setStringvalue(String("string"));
    $$->setLine($1);
} | TOK_BOOL {
    /* make an id node for this */
    $$ = new Node(NODE_IDENTIFIER);
    $$->setStringvalue(String("bool"));
    $$->setLine($1);
}

/* a list of at least one parameter */
actual_param_list: expression TOK_COMMA actual_param_list {
    $$ = new Node(NODE_ACTUAL_PARAM_LIST);
    $$->addChild($1);
    $$->addChild($3);
    $$->setLine($2);
} | expression {
    $$ = new Node(NODE_ACTUAL_PARAM_LIST);
    $$->addChild($1);
}

%%

int yywrap( ) { 
    return 1;
}

void yyerror(const char* str) {
    throw Error(str, yylineNumber);
}

/* parse from a file */
extern std::istream* in;
Node* parseFile(const String& fname) {
    /* reset all of the lexer stuff */
    reset_lexer( );

    /* clear the type state information */
    globals.clear();
    functions.clearAll();

    /* open the file */
    QFile file(fname.toQ());

    /* if it's not open, we failed */
    if (!file.open(QIODevice::ReadOnly)) {
        throw Error("Could not open file '" + fname + "'");
    }

    /* set the in stream (defined in lexer.cpp) */
    setLexFile(file);

    /* call yyparse */
    yyparse();

    /* clean up */
    deleteLexFile();

    /* check and infer the types in the tree */
    initTypes(root);
    inferTypes(root);

    /* return the root of the parse tree */
    return root;
}
