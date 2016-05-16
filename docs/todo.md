# TODO list for Tetra


### Language Features
- Allow variable declarations by themselves, e.g. "x int" by itself.
- Allow for ranges to work on variables.  Right now, [1 ... 10] is OK,
but [1 ... N] is not.
- Add types for tasks and mutexes to the list of built in types.
- Allow for named tasks, and for the wait command to wait for them.
- Add tuples as a type, including declarations.
- Add dictionaries as a type, including declarations.
- Add user-created objects as an available type for declaraitions etc.
- Implement the class statement, methods etc.
- Implement init functions.
- Add the module system, allowing for code in multiple files to be used.
- Add functions as one of the available types for declarations.
- Allow functions to be passed into functions/returned from functions.
- Implement the lambda expression.


### Standard Library Stuff
- Implement the "list" function which creates a new list of a given size.
- Implement the "keys" function for dictionaries.
- Add a math library.
- Add a generic sort/search/count type list library.


### Other Things
- Rename "vector" to "list" every where.
- Improve speedup for parallel for loops.


### Parser
- Add memory de-allocation for parse tree nodes and related structures.
- Improve error message clarity.
- Imporve the accuracy of the line numbers associated with nodes.
- Allow for the automatic promotion of ints to reals.
- Improve type checking on binary operators.

