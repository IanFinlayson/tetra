/* list.h
 * declares the List class which represents a Tetra List object */

#ifndef FUNCTION_H
#define FUNCTION_H

#include "values.h"
#include "node.h"

/* the List class */
class Function : public Value {
   public:
    String toString() const;
    
    Function() {
        f = nullptr;
    }
    
    Function(Node* f) {
        this->f = f;
    }

    void copyValue(const Value& other) {
        f = ((Function&)other).f;
    }

    String getName() const {
        return f->getStringvalue();
    }

    String getAddress() const {
        return String(Int((unsigned long)f));
    }

   private:
    Node* f;
};

#endif
