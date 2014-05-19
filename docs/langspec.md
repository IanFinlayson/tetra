# Minimal Language Specification for Tetra

### TYPES
Tetra has for primitive types:
- real (floating point numbers).
- int
- bool
- string

The "void" type also refers to the absence of a type, but that is only used
internally.

There are also vectors (arrays) which can be of other vectors, or any of the
primitive types.  A type declaration for a vector of ints would be [int] and
a 2D vector of strings would be [[string]].

Tetra is statically typed, and uses local type inference.  This means the
compiler will automatically guess the types of local variables based on what
is assigned into them.  Function parameters and return values need explicit
declarations, however.

### Statements


### Expressions


### Parallel Features

