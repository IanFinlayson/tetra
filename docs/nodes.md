# Tetra Node Documentation

Each of the following describes a *kind* of node.  The kind defines which other
fields of the node make sense to use, and tells you what number and kind of
children it should have.

Below we describe each kind of node.

### NODE_FUNCTION_LIST
A list of functions.  These nodes are used for chaining all of the functions in
a program together.  Most have two children, the first being a function and the
second being another list of functions.  The last function in the list has only
one child.

### NODE_FUNCTION
A function node represents a function definition.  Its name is stored in the
string field.  Its data type stores the return type for the function.  Its
first child is the parameter list for the function.  Its second child holds the
block of code that represents the body.

### NODE_FORMAL_PARAM_LIST
A list of formal parameters into a function.  A *formal* parameter is one in a
function definition (as opposed to an *actual* parameter which is used in a
    function call).  It has two children: the first is represents the next
parameter, and the second is either the list of remaining parameters or the
last parameter.

### NODE_FORMAL_PARAM
A single formal parameter to a function.  Its stringval holds the name of the
formal parameter, and its data type holds the type of this parameter.  It has
no children.

### NODE_STATEMENT
This node is used for chaining multiple statements together in the same way
that NODE_FUNCTION_LIST is.  Each has two children.  The first is the next
statement in the sequence.  The second is either the last statement in the
sequence, or else another NODE_STATEMENT node.

### NODE_PASS
A pass node represents the *pass* statement.  It has no children.

### NODE_BREAK
A break node represents the *break* statement.  It has no children.

### NODE_CONTINUE
A continue node represents the *continue* statement.  It has no children.

### NODE_RETURN
A return node represents the *return* statement.  It has either no children, for a return in a void function,
or one child representing the expression to be returned.

### NODE_IF
### NODE_ELIF
### NODE_ELIF_CHAIN
### NODE_ELIF_CLAUSE
### NODE_WHILE
### NODE_FOR
### NODE_PARALLEL
### NODE_PARFOR
### NODE_BACKGROUND
### NODE_LOCK

### NODE_ASSIGN
An assignment node has two children.  The first is an identifier node giving
the target of the assignment.  The second is an expression representing the
right hand side.  Note that regular assignment nodes are automatically created
for += -= *= etc. operators.  Also note that assignments are *expressions* in
Tetra: they can be used in the condition for an if or while statement, function
arguments etc.

### Other binary operators
These operators including:
- NODE_OR
- NODE_AND
- NODE_LT
- NODE_LTE
- NODE_GT
- NODE_GTE
- NODE_EQ
- NODE_NEQ
- NODE_BITXOR
- NODE_BITAND
- NODE_BITOR
- NODE_SHIFTL
- NODE_SHIFTR
- NODE_PLUS
- NODE_MINUS
- NODE_TIMES
- NODE_DIVIDE
- NODE_MODULUS
- NODE_EXP
Each have two children representing the left and right operands.

### NODE_NOT and NODE_BITNOT
These two unary operators have one child representing their operand.  The
difference is that NODE_NOT is used for boolean values and NODE_BITNOT is used
on integral ones.



### NODE_VECREF
### NODE_INDEX

### NODE_FUNCALL
### NODE_ACTUAL_PARAM_LIST

### NODE_INTVAL
### NODE_REALVAL
### NODE_BOOLVAL
### NODE_STRINGVAL
### NODE_IDENTIFIER









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
  
  /* make a node for the first clause */
  Node* c1 = new Node(NODE_ELIF_CLAUSE);
  c1->addChild($2);
  c1->addChild($4);
  $$->addChild(c1);

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

/* a for loop */
for_statement: TOK_FOR identifier TOK_IN expression TOK_COLON block {
  $$ = new Node(NODE_FOR);
  $$->addChild($2);
  $$->addChild($4);
  $$->addChild($6);
}

/* a parallel for loop */
parfor: TOK_PARALLEL TOK_FOR identifier TOK_IN expression TOK_COLON block {
  $$ = new Node(NODE_PARFOR);
  $$->addChild($3);
  $$->addChild($5);
  $$->addChild($7);
}

/* a while loop */
while_statement: TOK_WHILE expression TOK_COLON block {
  $$ = new Node(NODE_WHILE);
  $$->addChild($2);
  $$->addChild($4);
}

/* a parallel block */
parblock: TOK_PARALLEL TOK_COLON block {
  $$ = new Node(NODE_PARALLEL);
  $$->addChild($3);
}

/* a background block */
background: TOK_BACKGROUND TOK_COLON block {
  $$ = new Node(NODE_BACKGROUND);
  $$->addChild($3);
}

/* a lock statement */
lock_statement: TOK_LOCK identifier TOK_COLON block {
  $$ = new Node(NODE_LOCK);
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


