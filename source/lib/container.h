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

    /* for loop iteration through elems*/
    Data* operator[](unsigned i) const{
        return values[i];
    }

    /* get the length of the container */
    unsigned length() const {
        return values.size();
    }

    void copyValue(const Value& other) {
        /* clear our data first */
        values.clear();

        /* cast it to a constainer */
        Container* otherContainer = (Container*) &other;

        /* copy each element */
        for (unsigned i = 0; i < otherContainer->length(); i++) {
            this->add(otherContainer->values[i]);
        }
    }

    /* add an element */
    void add(Data* element) {
        Data* valPtr = Data::create(element->getType(), element->getValue());
        values.push_back(valPtr);
    }

    /* tetra program element access */ 
    virtual Data*& get(Data* idx) = 0;
    virtual Data* get(Data* idx) const = 0;
    
   protected:
    virtual String getLDelim() const = 0;
    virtual String getRDelim() const = 0;
    std::vector<Data*> values;
};

#endif
