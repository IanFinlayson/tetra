
/* tuple.h
 * declares the tuple class which represents a Tetra Tuple object */

#ifndef TUPLE_H
#define TUPLE_H

#include "values.h"
#include "error.h"

class Data;

/* the List class */
class Tuple : public List {
   public:
    String toString() const;
   private:
    static const String L_DELIM;
    static const String R_DELIM;
};

#endif
