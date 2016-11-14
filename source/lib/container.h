/* container.h
 * Base class for tetra container types */

#ifndef CONTAINER_H
#define CONTAINER_H

#include "values.h"
#include "error.h"

class Data;

class Container : public Value {
   public:
    String toString() const;

   protected:
    String getValString() const {
        String valString = "";
        for (unsigned i = 0; i < values.size(); i++) {
            DataTypeKind kind = values[i]->getType()->getKind();
            String outer = (kind == TYPE_STRING) ? "'" : "";
            valString += outer + values[i]->getValue()->toString() + outer;
            /* if not the last, print a comma */
            if ((i + 1) < values.size()) {
                valString += ", ";
            }
        }
        return valString;
    }

    virtual String getLDelim() const = 0;
    virtual String getRDelim() const = 0;
    std::vector<Data*> values;
};

#endif
