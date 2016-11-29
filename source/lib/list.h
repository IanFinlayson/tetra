/* list.h
 * declares the List class which represents a Tetra List object */

#ifndef LIST_H
#define LIST_H

#include "container.h"

/* the List class */
class List : public Container {
   public:
    /* concatenate two lists */
    List operator+(const List& other) {
        List result;

        /* copy from lhs */
        for (unsigned i = 0; i < values.size(); i++) {
            result.values.push_back(values[i]);
        }

        /* copy from rhs */
        for (unsigned i = 0; i < other.values.size(); i++) {
            result.values.push_back(other.values[i]);
        }

        return result;
    }

    /* for loop iteration through elems*/
    Data* operator[](unsigned i) const{
        return values[i];
    }

    /* add an element */
    void add(Data* element) {
        //Data* valPtr = Data::create(element->getType(), element->getValue());
        values.push_back(element);
    }

    /* get an element of the array at a given index */
    Data*& get(Data* index) {
        unsigned i = ((Int*) index->getValue())->toInt();
        if (i > values.size()) {
            throw RuntimeError("List index out of bounds.", 0);
        } else {
            return values[i];
        }
    }

    /* get an element at a index */
    Data* get(Data* index) const {
        unsigned i = ((Int*) index->getValue())->toInt();
        if (i > values.size()) {
            throw RuntimeError("List index out of bounds.", 0);
        } else {
            return values[i];
        }
    }

   protected:
    String getLDelim() const {
        return L_DELIM;
    }

    String getRDelim() const {
        return R_DELIM;
    }

   private:
    static const String L_DELIM;
    static const String R_DELIM;
};

#endif
