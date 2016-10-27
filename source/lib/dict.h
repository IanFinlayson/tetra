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
    Data*& get(const Data* key) {
          std::cout << "in get()" << std::endl;
          std::string keyString = key->getValue()->toString().toQ().toUtf8().constData();
          std::cout << "keyString = '" << keyString << "'"<<std::endl;
          std::cout << "values.count(keystring) = " << values.count(keyString)<< std::endl;
          std::cout << "values.at(keyString) = '" << values.at(keyString).second->getValue()->toString() << "'."<< std::endl;
        if (!values.count(keyString)) {
            throw RuntimeError("Key not contained in dictionary.", 0);
        } else {
            return values.at(keyString).second;
        }
    }

    /* get the value from the dict mapped to by the given key */
    Data* get(const Data* key) const {
          std::string keyString = key->getValue()->toString().toQ().toUtf8().constData();
          std::cout << "in get()const " << std::endl;
          std::cout << "keyString = '" << keyString << "'"<<std::endl;
          std::cout << "values.count(keystring) = " << values.count(keyString)<< std::endl;
          std::cout << "values.at(keyString) = '" << values.at(keyString).second->getValue()->toString() << "'."<< std::endl;
        if (!values.count(keyString)) {
            throw RuntimeError("Key not contained in dictionary.", 0);
        } else {
            return values.at(keyString).second;;
        }
    }

    /* return true if the dictionary contains the given key */
    bool hasKey(const Data* key) const {
        std::string keyString = key->getValue()->toString().toQ().toUtf8().constData();
        std::cout << "in hasKey()" << std::endl;
        std::cout << "keyString = '" << keyString << "'"<<std::endl;
        std::cout << "values.count(keystring) = " << values.count(keyString) << std::endl;
        return values.count(keyString);
    }

    /* add a key value pair to the dict*/
    void put(Data* key, Data* value = NULL) {
        std::cout << "in put()" << std::endl;
        std::string keyString = key->getValue()->toString().toQ().toUtf8().constData();
        std::cout <<"set keystring to '" << keyString<< "'.\n";
        std::cout << "before put, values.count(keyString) = " << values.count(keyString)<< std::endl;
        if (!value) {
            value = key;
        }
        values[keyString] = std::make_pair(key,value);
        std::cout << "after put, values.count(keyString) = " << values.count(keyString)<< std::endl;
        std::cout << "values.at(keyString) = '" << values.at(keyString).second->getValue()->toString() << "'."<< std::endl;
    }

    /* get the length of the dict */
    int length() const {
        return values.size();
    }

    /* return a pointer to the map */
    std::map<std::string, std::pair<Data*, Data*> >* getValues() { 
        return &values; 
    }

   private:
    std::map<std::string,std::pair<Data*, Data*> > values;
};

#endif
