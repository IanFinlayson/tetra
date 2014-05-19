# Tetra Frontend Documentation

### Usage


### Nodes
Node objects comprise the parse tree returned from parseFile.
Below, we describe the member functions of this class along with the
different kinds of nodes produced.

###### Member Functions

```cpp
 Node(NodeKind type);
```

```cpp
 void setDataType(DataType* data_type);
```

```cpp
 void setStringval(const string& stringval);
```

```cpp
 void setIntval(int intval);
```

```cpp
 void setBoolval(bool boolval);
```

```cpp
 void setRealval(double realval);
```

```cpp
 void setLine(int lineno);
```

```cpp
 void setType(DataType* t);
```

```cpp
 int getLine( );
```

```cpp
 string getString( );
```

```cpp
 int getInt( );
```

```cpp
 double getReal( );
```

```cpp
 bool getBool( );
```

```cpp
 NodeKind kind( );
```

```cpp
 DataType* type( );
```

```cpp
 void addChild(Node* child);
```

```cpp
 int numChildren( );
```

```cpp
 Node* child(int which);
```

```cpp
 void insertSymbol(Symbol sym);
```

```cpp
 Symbol lookupSymbol(string name, int lineno);
```

```cpp
 bool hasSymbol(const string& name);
```



##### Kinds
Each of the following describes a *kind* of node.  The kind defines which other
fields of the node make sense to use, and tells you what number and kind of
children it should have.

###### NODE_FUNCTION_LIST
A list of functions.  These nodes are used for chaining all of the functions in
a program together.  Most have two children, the first being a function and the
second being another list of functions.  The last function in the list has only
one child.

###### NODE_FUNCTION
A function node represents a function definition.  Its name is stored in the
string field.  Its data type stores the return type for the function.  Its
first child is the parameter list for the function.  Its second child holds the
block of code that represents the body.

###### NODE_FORMAL_PARAM_LIST
A list of formal parameters into a function.  A *formal* parameter is one in a
function definition (as opposed to an *actual* parameter which is used in a
function call).  It has two children: the first is represents the next
parameter, and the second is either the list of remaining parameters or the
last parameter.

###### NODE_FORMAL_PARAM
A single formal parameter to a function.  Its stringval holds the name of the
formal parameter, and its data type holds the type of this parameter.  It has
no children.

###### NODE_STATEMENT
This node is used for chaining multiple statements together in the same way
that NODE_FUNCTION_LIST is.  Each has two children.  The first is the next
statement in the sequence.  The second is either the last statement in the
sequence, or else another NODE_STATEMENT node.

###### NODE_PASS
A pass node represents the *pass* statement.  It has no children.

###### NODE_BREAK
A break node represents the *break* statement.  It has no children.

###### NODE_CONTINUE
A continue node represents the *continue* statement.  It has no children.

###### NODE_RETURN
A return node represents the *return* statement.  It has either no children,
for a return in a void function, or one child representing the expression to be
returned.

###### NODE_IF
This represents an if statement.  The first child is an expression representing
the if condition.  The second child represents the block of code to run if the
condition is true.  The third child is either NULL or, if there was an else
clause, represents the code to run if the condition was false.

###### NODE_ELIF
An elif node represents an entire if/elif*/else? structure.  The first child is
a NODE_ELIF_CLAUSE representing the first condition/statement pair.  The second
child is a NODE_ELIF_CHAIN representing the other clauses.  The third child (if
present) represents the statement block to run if none of the conditions are
true.

###### NODE_ELIF_CHAIN
A chain of one or more elif clauses.  The first child is the next clause.  The
second child is either another chain, or the last clause in the chain.

###### NODE_ELIF_CLAUSE
An elif clause is one condition/statement pair.  The first child is the
condition, and the second is the statement to execute if it is true.

###### NODE_WHILE
Represents a while statement.  The first child is the condition expression
node, and the second child is the body to be executed as long as the condition
is true.

###### NODE_FOR or NODE_PARFOR
Represents a for loop or parallel for loop statement.  The first child is the
identifier node for the control variable.  The second child is the expression
that evaluates to a vector of elements to loop over.  The third child is the
body of the loop.

###### NODE_PARALLEL and NODE_BACKGROUND
These represent parallel and background blocks respectively.  The only child is
the body of the block giving the code to run in parallel - or in the
background.

###### NODE_LOCK
Represents a lock statement.  The first child is the identifier node of the
lock, the second child is the block of code to do under the lcok.

###### NODE_ASSIGN
An assignment node has two children.  The first is an identifier node giving
the target of the assignment.  The second is an expression representing the
right hand side.  Note that regular assignment nodes are automatically created
for += -= *= etc. operators.  Also note that assignments are *expressions* in
Tetra: they can be used in the condition for an if or while statement, function
arguments etc.

###### Other binary operators
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

###### NODE_NOT and NODE_BITNOT
These two unary operators have one child representing their operand.  The
difference is that NODE_NOT is used for boolean values and NODE_BITNOT is used
on integral ones.

###### NODE_VECREF
A node which represents a vector reference - an identifier followed by some
number of indices.  The first child is an identifier node which stores the
vector id.  The second child is a NODE_INDEX.

###### NODE_INDEX
A node which represents a set of indices on an identifier.  The first child is
the next expression used for calculating the index.  If there are more indices,
they are stored as a NODE_INDEX in the second child.

###### NODE_FUNCALL
Represents a function call.  The string value stores the name of the function
being called.  If there are parameters, they are stored in a child node.  If
there is only one, it is an expression.  If there are multiple, then they are
stored as a NODE_ACTUAL_PARAM_LIST.

###### NODE_ACTUAL_PARAM_LIST
Represents a list of *actual* parameters - those used in a function call.  The
first child is the next actual parameter to be passed.  The second child is
either the last parameter (an expression) or another NODE_ACTUAL_PARAM_LIST
node.


###### NODE_INTVAL, NODE_REALVAL, NODE_BOOLVAL and NODE_STRINGVAL
Each of these are leaf nodes which store only one value in the correspondingly
typed value.

###### NODE_IDENTIFIER
A leaf node that represents an identifier node and stores the name in the
string value.

