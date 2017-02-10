/* dict.h
 * declares the Dict class which represents a Tetra Dictionary object */

#ifndef DICT_H
#define DICT_H

#include "container.h"
#include "pair.h"

class Data;

/* the Dict class */
class Dict : public Container {
   public:
    Data* operator[](unsigned i) const {
        return ((Pair*) (values[i]->getValue()))->getKey();
    }

    /* get the value from the dict mapped to by the given key */
    Data*& get(Data* key) {
        String keyString = key->getValue()->toString();
        if (!idxMap.count(keyString)) {
            throw RuntimeError("Key not contained in dictionary.", 0);
        } else {
            return ((Pair*) (values[idxMap.at(keyString)])->getValue())->getVal();
        }
    }

    /* get the value from the dict mapped to by the given key */
    Data* get(Data* key) const {
        String keyString = key->getValue()->toString();
        if (!idxMap.count(keyString)) {
            throw RuntimeError("Key not contained in dictionary.", 0);
        } else {
            return ((Pair*) (values[idxMap.at(keyString)])->getValue())->getVal();
        }
    }

    /* return true if the dictionary contains the given key */
    bool hasKey(const Data* key) const {
        String keyString = key->getValue()->toString();
        return idxMap.count(keyString);
    }

    void add(Data* element) {
        /* add to the val list*/
        values.push_back(element);
        /*make the index mapping*/
        String keyString = ((Pair*) (element->getValue()))->getKey()->getValue()->toString();
        idxMap[keyString] = (length() - 1);
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
    /* maps key to array index */
    std::map<String, unsigned> idxMap;
};

#endif
