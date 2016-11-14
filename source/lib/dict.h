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
        values.insert(otherDict->values.begin(),otherDict->values.end());
    }

    /* get the value from the dict mapped to by the given key */
    Data*& get(Data* key) {
          String keyString = key->getValue()->toString();
        if (!values.count(keyString)) {
            throw RuntimeError("Key not contained in dictionary.", 0);
        } else {
            return values.at(keyString)[1];
        }
    }

    /* get the value from the dict mapped to by the given key */
    Data* get(Data* key) const {
          String keyString = key->getValue()->toString();
        if (!values.count(keyString)) {
            throw RuntimeError("Key not contained in dictionary.", 0);
        } else {
            return values.at(keyString)[1];
        }
    }

    /* return true if the dictionary contains the given key */
    bool hasKey(const Data* key) const {
        String keyString = key->getValue()->toString();
        return values.count(keyString);
    }

    /* add a key value pair to the dict
     * If no value is given, the key is mapped to 
     * a placeholder.*/
    void put(Data* key, Data* value = NULL) {
        String keyString = key->getValue()->toString();
        std::vector<Data*> vec;
        Data* keyPtr = Data::create(key->getType(), key->getValue());
        Data* valPtr; 
        if (!value) {
            valPtr = Data::create(key->getType(), key->getValue());
        } else {
            valPtr = Data::create(value->getType(), value->getValue());
        }
        vec.push_back(keyPtr);
        vec.push_back(valPtr);
        values[keyString] = vec;
    }

      /* get the length of the dict */
    int length() const {
        return values.size();
    }

    /* return a pointer to the map */
    std::map<String, std::vector<Data*> >* getValues() { 
        return &values; 
    }

   private:
    std::map<String, std::vector<Data*> > values;
};

#endif
