# Tetra Standard Library

This document contains standard library functions for Tetra.  It currently only
contains the few functions that would be needed to "bootstrap" the library.
That is, those functions which could not be written in Tetra itself.

### IO

The following functions each take one primitive Tetra type and print it to the
screen.  Eventually Tetra should have overloading of functions which would allow
these to be collapsed into one function.  Another possibility would be to make
print a statement level construct a la Python 2.x

```
print_string(s string)
print_int(i int)
print_real(r real)
print_bool(b bool)
```

The following functions each read one primitive value from the user and return
it.

```
read_string( ) string
read_int( ) int
read_real( ) real
read_bool( ) bool
```


### Vectors and Strings
The following functions return the number of elements in a vector or string.
The first will take some thought as currently, there is no way to write or call
a function that accepts *any* type of vector...

```
vector_size(v)
string_size(s string)
```


