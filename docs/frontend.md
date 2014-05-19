# Tetra Frontend Documentation

### Usage
The frontend contains a lexical analyzer, a syntax  analyzer, and type
checking/inference code for Tetra.  To use it, include the main header file:

```cpp
#include "frontend/frontend.hpp"
```
Then link to the "frontend/libfrontend.a" static library when compiling.

The first function to call would be:
```cpp
Node* parseFile(const string& fname);
```
Which loads code from the file indicated by the file name parameter, lexes it,
parses it, performs type checking/inference on it, and returns a pointer to the
root node of the program.

Should anything go wrong with the process, it throws an "Error" object as an
exception.

### Errors
Errors are handled with exceptions thrown by the frontend.  The Error class
contains the following functions:

```cpp
Error(const string& mesg, int lineno = 0); 
```
Theructor is used when throwing an Error.  The message and line number
describe the error.

```cpp
string getMessage( );
```
Returns the message reported when the Error was raised.  Some of these need to
be improved in terms of clarity.

```cpp
int getLine( );
```
Returns the source line where the Error was detected.  Some of these are not as
accurate as they could be - due to the way the parser works, it is not always
obvious which line is associated with each syntax element.

### Symbols
The symbol class represents identifiers used in a function - local variables
and parameters.  The Symbol class contains the following functions:

```cpp
Symbol( );
Symbol(string name, DataType* type, int lineno);
```
Constructors used for creating symbols.

```cpp
int getLine( );
string getName( );
DataType* getType( );
```
Accessors used for getting the name of the symbol, the line associated with it,
and the data type.  Parameter symbols get their types from the declarations in
the code, while local variable symbols get their types computed.

### Data Types
Data types are handled with the DataType class.  Data types are comprised of
two elements:
- The *kind* of type it is.  This is handled with an enumeration:
 - TYPE_INT
 - TYPE_REAL
 - TYPE_STRING
 - TYPE_BOOL
 - TYPE_VOID
 - TYPE_VECTOR
- The *subtype* which is only used TYPE_VECTOR types.  The subtype is another
pointer to a DataType object.

Data types have the following functions:
```cpp
DataType(DataTypeKind kind);
```
Theructor takes the kind of type (one of the above enumeration values).

```cpp
void setSubType(DataType* subtype);
```
Sets the subtype of the data type (again only done for vectors).

```cpp
DataTypeKind getKind( );
```
Returns the kind of type this is.

```cpp
DataType* getSub( );
```
Returns the subtype of this type.

Data types also provide the == and != operators for comparing two types for
equality.

There is also a function
```cpp
string typeToString(DataType* t); 
```
Which returns a string representation of the given data type.

### Nodes
Node objects comprise the parse tree returned from parseFile.  Below, we
describe the member functions of this class along with the different kinds of
nodes produced.

##### Member Functions

```cpp
Node(NodeKind type);
```
Theructor takes the *kind* of node to be created.  This is an enum whose
values are described at the end of this document.

```cpp
void setDataType(DataType* data_type);
```
This sets the DataType of the node.  Not all nodes have a data type (such as a
node representing a while loop), but many do (such as values, operators, etc.)

```cpp
void setStringval(const string& stringval);
void setIntval(int intval);
void setRealval(double realval);
void setBoolval(bool boolval);
```
These functions set the string, int, real and bool values associated with
nodes.

Most types of nodes do not have any of these values associated with them.
Identifiers, functions and stringants do have string values, and nodes
representing numeric or boolean literals have the appropriate value set.

```cpp
void setLine(int lineno);
```
This sets the source line number to be associated with this node.

```cpp
void setType(DataType* t);
```
This sets the data type to be associated with this node.  Not all nodes have a
data type (again like a while loop node), but many do.  The frontend tries to
infer as many types as it can, and sets the types using this function.

```cpp
int getLine( );
```
Returns the line number associated with the node - this is used in error
reporting.  The line numbers actually may not be entirely accurate.  The
frontend currently does not do a very good job of assigning accurate line
numbers, but this should be fixed.

```cpp
string getString( );
int getInt( );
double getReal( );
bool getBool( );
```
Each of these functions returns the value associated with the node as described
above.

```cpp
NodeKind kind( );
```
This function returns the kind of node this is - the possible values are
described in detail below.

```cpp
DataType* type( );
```
This function returns the data type of the node - or NULL if it does not have
one.

```cpp
void addChild(Node* child);
```
This function adds a node as a child of another - this is probably only useful
to the frontend code itself.

```cpp
int numChildren( );
```
This function returns the number of children that a given node has.

```cpp
Node* child(int which);
```
This function returns a child of a node.  The which parameter indicates which
child should be returned.  The first child is 0, the second is 1 and so on.

```cpp
void insertSymbol(Symbol sym);
```
This function inserts a Symbol object into the nodes symbol table.  Only
NODE_FUNCTION nodes have symbol tables.

```cpp
Symbol lookupSymbol(string name, int lineno);
```
This function returns a symbol from the nodes symbol table.  If the symbol is
not found, then this function throws an Error.  The line number parameter is
used for the Error message.

```cpp
bool hasSymbol(const string& name);
```
This function checks to see if a symbol is in the nodes symbol table.



##### Kinds
Each of the following describes a *kind* of node.  The kind defines which other
fields of the node make sense to use, and tells you what number and kind of
children it should have.

##### NODE_FUNCTION_LIST
A list of functions.  These nodes are used for chaining all of the functions in
a program together.  Most have two children, the first being a function and the
second being another list of functions.  The last function in the list has only
one child.

##### NODE_FUNCTION
A function node represents a function definition.  Its name is stored in the
string field.  Its data type stores the return type for the function.  Its
first child is the parameter list for the function.  Its second child holds the
block of code that represents the body.

##### NODE_FORMAL_PARAM_LIST
A list of formal parameters into a function.  A *formal* parameter is one in a
function definition (as opposed to an *actual* parameter which is used in a
function call).  It has two children: the first is represents the next
parameter, and the second is either the list of remaining parameters or the
last parameter.

##### NODE_FORMAL_PARAM
A single formal parameter to a function.  Its stringval holds the name of the
formal parameter, and its data type holds the type of this parameter.  It has
no children.

##### NODE_STATEMENT
This node is used for chaining multiple statements together in the same way
that NODE_FUNCTION_LIST is.  Each has two children.  The first is the next
statement in the sequence.  The second is either the last statement in the
sequence, or else another NODE_STATEMENT node.

##### NODE_PASS
A pass node represents the *pass* statement.  It has no children.

##### NODE_BREAK
A break node represents the *break* statement.  It has no children.

##### NODE_CONTINUE
A continue node represents the *continue* statement.  It has no children.

##### NODE_RETURN
A return node represents the *return* statement.  It has either no children,
for a return in a void function, or one child representing the expression to be
returned.

##### NODE_IF
This represents an if statement.  The first child is an expression representing
the if condition.  The second child represents the block of code to run if the
condition is true.  The third child is either NULL or, if there was an else
clause, represents the code to run if the condition was false.

##### NODE_ELIF
An elif node represents an entire if/elif*/else? structure.  The first child is
a NODE_ELIF_CLAUSE representing the first condition/statement pair.  The second
child is a NODE_ELIF_CHAIN representing the other clauses.  The third child (if
present) represents the statement block to run if none of the conditions are
true.

##### NODE_ELIF_CHAIN
A chain of one or more elif clauses.  The first child is the next clause.  The
second child is either another chain, or the last clause in the chain.

##### NODE_ELIF_CLAUSE
An elif clause is one condition/statement pair.  The first child is the
condition, and the second is the statement to execute if it is true.

##### NODE_WHILE
Represents a while statement.  The first child is the condition expression
node, and the second child is the body to be executed as long as the condition
is true.

##### NODE_FOR or NODE_PARFOR
Represents a for loop or parallel for loop statement.  The first child is the
identifier node for the control variable.  The second child is the expression
that evaluates to a vector of elements to loop over.  The third child is the
body of the loop.

##### NODE_PARALLEL and NODE_BACKGROUND
These represent parallel and background blocks respectively.  The only child is
the body of the block giving the code to run in parallel - or in the
background.

##### NODE_LOCK
Represents a lock statement.  The first child is the identifier node of the
lock, the second child is the block of code to do under the lcok.

##### NODE_ASSIGN
An assignment node has two children.  The first is an identifier node giving
the target of the assignment.  The second is an expression representing the
right hand side.  Note that regular assignment nodes are automatically created
for += -= *= etc. operators.  Also note that assignments are *expressions* in
Tetra: they can be used in the condition for an if or while statement, function
arguments etc.

##### Other binary operators
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

##### NODE_NOT and NODE_BITNOT
These two unary operators have one child representing their operand.  The
difference is that NODE_NOT is used for boolean values and NODE_BITNOT is used
on integral ones.

##### NODE_VECREF
A node which represents a vector reference - an identifier followed by some
number of indices.  The first child is an identifier node which stores the
vector id.  The second child is a NODE_INDEX.

##### NODE_INDEX
A node which represents a set of indices on an identifier.  The first child is
the next expression used for calculating the index.  If there are more indices,
they are stored as a NODE_INDEX in the second child.

##### NODE_FUNCALL
Represents a function call.  The string value stores the name of the function
being called.  If there are parameters, they are stored in a child node.  If
there is only one, it is an expression.  If there are multiple, then they are
stored as a NODE_ACTUAL_PARAM_LIST.

##### NODE_ACTUAL_PARAM_LIST
Represents a list of *actual* parameters - those used in a function call.  The
first child is the next actual parameter to be passed.  The second child is
either the last parameter (an expression) or another NODE_ACTUAL_PARAM_LIST
node.


##### NODE_INTVAL, NODE_REALVAL, NODE_BOOLVAL and NODE_STRINGVAL
Each of these are leaf nodes which store only one value in the correspondingly
typed value.

##### NODE_VECVAL
This node type represents a vector literal.  If there are no children, it
represents the empty vector [].  If there is one child, that child is an
expression which gives the value of the next element in the vector.  If there
is a second child, it is another NODE_VECVAL giving the rest of the vectors
values.

##### NODE_IDENTIFIER
A leaf node that represents an identifier node and stores the name in the
string value.

