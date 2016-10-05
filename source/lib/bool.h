/* bool.h
 * the Bool class represents a boolean value in a Tetra program */

#include "values.h"

#ifndef BOOL_H
#define BOOL_H

class Bool : public Value {
   public:
    /* constructors */
    Bool() {
        this->b = false;
    }
    Bool(const bool& b) {
        this->b = b;
    }
    Bool(const Bool& other) {
        this->b = other.b;
    }

    /* assignment */
    Bool operator=(const Bool& other) {
        this->b = other.b;
        return *this;
    }

    /* conversion */
    bool toBool() {
        return this->b;
    }

    void copyValue(const Value& other) {
        b = ((Bool&) other).b;
    }

    /* logical operators */
    Bool operator&&(const Bool& other) const {
        return Bool(this->b && other.b);
    }
    Bool operator||(const Bool& other) const {
        return Bool(this->b || other.b);
    }
    Bool operator!() const {
        return Bool(!this->b);
    }

    /* comparison operators */
    Bool operator==(const Bool& other) const {
        return Bool(this->b == other.b);
    }
    Bool operator!=(const Bool& other) const {
        return !(this->b == other.b);
    }

    /* Bool output */
    friend std::ostream& operator<<(std::ostream& os, const Bool& tb) {
        tb.b ? os << "true" : os << "false";
        return os;
    }

    String toString() const;

   private:
    bool b;
};

#endif
