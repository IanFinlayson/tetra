/* object.h
 * the Object class represents a tetra class object */

#ifndef OBJECT_H
#define OBJECT_H

#include "tetra.h"

/* the class context map is defined in types.cpp */
extern std::map<String, ClassContext> classes;

class Object : public Value {
   public:
    Object(DataType* type) {
        /* find the appropriate class context */
        class_context = &classes[*(type->className)];

        /* insert all of the data fields */
        for (std::map<String, Symbol>::iterator i = class_context->members.begin();
             i != class_context->members.end(); i++) {
            Symbol symbol = i->second;
            Data* value = Data::create(symbol.getType(), NULL);
            insert(i->first, value);
        }
    }

    String toString() const {
        return "<object>";
    }

    void copyValue(const Value& other) {
        std::cout << "\n\nRUTRO\n\n";
    }

    void insert(const String& name, Data* value) {
        members[name] = value;
    }

    Data* lookup(const String& name) {
        auto iter = members.find(name);
        if (iter != members.end()) {
            return iter->second;
        } else {
            return NULL;
        }
    }

   private:
    std::map<String, Data*> members;
    ClassContext* class_context;
};

#endif
