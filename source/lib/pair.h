/* pair->h
 * declares the pair class which represents a Tetra
 * dictionary key-value pair */

#ifndef PAIR_H
#define PAIR_H

#include "values.h"
#include "error.h"
#include "container.h"

/* the List class */
class Pair : public Value {
   public:
    void copyValue(const Value& other) {
        this->key = ((Pair&) other).key;
        this->value = ((Pair&) other).value;
    }

    String toString() const;

    void setKey(Data* key) {
        this->key = key;
    }
    void setVal(Data* value) {
        this->value = value;
    }
    void set(Data* key, Data* value) {
        this->key = key;
        this->value = value;
    }
    Data* getKey() {
        return key;
    }
    Data*& getVal() {
        return value;
    }

   private:
    Data* key;
    Data* value;
};

#endif
