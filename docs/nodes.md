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
This represents an if statement.  The first child is an expression representing
the if condition.  The second child represents the block of code to run if the
condition is true.  The third child is either NULL or, if there was an else
clause, represents the code to run if the condition was false.

### NODE_ELIF
An elif node represents an entire if/elif*/else? structure.  The first child is
a NODE_ELIF_CLAUSE representing the first condition/statement pair.  The second
child is a NODE_ELIF_CHAIN representing the other clauses.  The third child (if
present) represents the statement block to run if none of the conditions are
true.

### NODE_ELIF_CHAIN
A chain of one or more elif clauses.  The first child is the next clause.  The
second child is either another chain, or the last clause in the chain.

### NODE_ELIF_CLAUSE
An elif clause is one condition/statement pair.  The first child is the
condition, and the second is the statement to execute if it is true.



















TODO

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


