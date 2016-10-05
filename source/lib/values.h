/* values.h
 * declares the data value types used in the language */

#ifndef VALUES_H
#define VALUES_H

#include <cmath>
#include <iostream>

class String;

/* represents one Tetra value, e.g. a number, string, list, anything */
class Value {
   public:
    virtual ~Value() {}
    virtual String toString() const = 0;

    virtual void copyValue(const Value& other) = 0;
};

#endif
