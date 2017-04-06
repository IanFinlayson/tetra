/* object.h
 * the Object class represents a tetra class object */

#ifndef OBJECT_H
#define OBJECT_H

#include "values.h"

class Object : public Value {
   public:
    Object() {
    
    }

    String toString() const {
        return "<object>";
    }

    void copyValue(const Value& other) {

    }


   private:

};

#endif
