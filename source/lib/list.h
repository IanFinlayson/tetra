/* list.h
 * declares the List class which represents a Tetra List object */

#ifndef LIST_H
#define LIST_H

#include "values.h"
#include "error.h"

class Data;

/* the List class */
class List : public Value {
   public:
    String toString() const;

    void copyValue(const Value& other) {
        /* clear our data first */
        values.clear();

        /* cast it to a list */
        List* otherList = (List*) &other;

        /* copy each element */
        for (unsigned i = 0; i < otherList->values.size(); i++) {
            values.push_back(otherList->values[i]);
        }
    }

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

    /* set an element of the array at a given index */
    Data*& get(unsigned int index) {
        if (index > values.size()) {
            throw RuntimeError("List index out of bounds.", 0);
        } else {
            return values[index];
        }
    }

    /* get an element at a index */
    Data* get(unsigned int index) const {
        if (index > values.size()) {
            throw RuntimeError("List index out of bounds.", 0);
        } else {
            return values[index];
        }
    }

    /* append an element to the list */
    void append(Data* element) {
        Data* valPtr = Data::create(element->getType(), NULL);
        valPtr->opAssign(element);
        values.push_back(valPtr);
    }

    /* get the length of the list */
    int length() const {
        return values.size();
    }

   private:
    std::vector<Data*> values;
};

#endif
