
/* tuple.h
 * declares the tuple class which represents a Tetra Tuple object */

#ifndef TUPLE_H
#define TUPLE_H

#include "list.h"
#include "error.h"

/* the List class */
class Tuple : public List {
   private:
    static const String L_DELIM;
    static const String R_DELIM;

   protected:
    String getLDelim() const {
        return L_DELIM;
    }

    String getRDelim() const {
        return R_DELIM;
    }
};

#endif
