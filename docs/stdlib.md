# Tetra Standard Library

This document contains standard library functions for Tetra.  It currently only
contains the few functions that would be needed to "bootstrap" the library.
That is, those functions which could not be written in Tetra itself.

### IO
The ```print``` function in Tetra is a special case in that it can be called
with any number of arguments of any type - unlike any other function in Tetra.
The frontend compiler has an exception where it will not type check the
parameters against a functions signature if the function name is ```print```.

The following functions each read one primitive value from the user and return
it.

```
read_string( ) string
read_int( ) int
read_real( ) real
read_bool( ) bool
```

### Vectors and Strings
The ```len``` function takes any vector or string as a parameter, and returns the
number of elements in that vector or string.


