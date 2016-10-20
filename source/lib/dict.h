/* dict.h
 * declares the Dict class which represents a Tetra Dictionary object */

#ifndef DICT_H
#define DICT_H

#include "values.h"
#include "error.h"

class Data;

/* the Dict class */
class Dict : public Value {
   public:
    String toString() const;

    void copyValue(const Value& other) {
        /* clear our data first */
        values.clear();

        /* cast it to a dict */
        Dict* otherDict = (Dict*) &other;

        /* copy each element */
        keys.insert(otherDict->keys.begin(),otherDict->keys.end());
        values.insert(otherDict->values.begin(),otherDict->values.end());
    }

    /* get the value from the dict mapped to by the given key */
    Data*& get(const Data* key) {
        String keyString = key->getValue()->toString();
        if (!values.count(keyString)) {
            throw RuntimeError("Key not contained in dictionary.", 0);
        } else {
            return values.at(keyString);
        }
    }

    /* get the value from the dict mapped to by the given key */
    Data* get(const Data* key) const {
        String keyString = key->getValue()->toString();
        if (!values.count(keyString)) {
            throw RuntimeError("Key not contained in dictionary.", 0);
        } else {
            return values.at(keyString);
        }
    }

    /* add a key value pair to the dict*/
    void put(Data* key, Data* value) {
        keys[key->getValue()->toString()] = key;
        values[key->getValue()->toString()] = value;
    }

    /* get the length of the dict */
    int length() const {
        return values.size();
    }

    /* return a pointer to the map */
    std::map<String,Data*>* getKeys() { 
        return &keys; 
    }

   private:
    std::map<String,Data*> keys;
    std::map<String,Data*> values;
};

#endif
