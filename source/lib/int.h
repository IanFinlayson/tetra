/* int.h
 * the Int class represents an integer in a tetra program */

#ifndef INT_H
#define INT_H

#include "values.h"
#include "bool.h"

class Int : public Value {
   public:
    /* constructors */
    Int() {
        this->i = 0;
    }
    Int(const int& i) {
        this->i = i;
    }
    Int(const Int& other) {
        this->i = other.i;
    }

    /* assignment */
    Int operator=(const Int& other) {
        this->i = other.i;
        return *this;
    }

    /* conversion */
    int toInt() const {
        return i;
    }

    void copyValue(const Value& other) {
        i = ((Int&) other).i;
    }

    /* bitwise operators */
    Int operator~() {
        return Int(~(this->i));
    }
    Int operator<<(const Int& other) {
        return Int(this->i << other.i);
    }
    Int operator>>(const Int& other) {
        return Int(this->i >> other.i);
    }
    Int operator&(const Int& other) {
        return Int(this->i & other.i);
    }
    Int operator|(const Int& other) {
        return Int(this->i | other.i);
    }
    Int operator^(const Int& other) {
        return Int(this->i ^ other.i);
    }

    /* arithmetic operators */
    Int operator+(const Int& other) {
        return Int(this->i + other.i);
    }
    Int operator-(const Int& other) {
        return Int(this->i - other.i);
    }
    Int operator*(const Int& other) {
        return Int(this->i * other.i);
    }
    Int operator/(const Int& other) {
        return Int(this->i / other.i);
    }
    Int operator%(const Int& other) {
        return Int(this->i % other.i);
    }
    Int pow(const Int& other) {
        return Int(std::pow(this->i, other.i));
    }

    /* unary operators */
    Int operator+() {
        return Int(+(this->i));
    }
    Int operator-() {
        return Int(-(this->i));
    }
    Int operator++(int) {
        return Int((this->i)++);
    }
    Int operator++() {
        return Int(++(this->i));
    }
    Int operator--(int) {
        return Int((this->i)--);
    }
    Int operator--() {
        return Int(--(this->i));
    }

    /* comparison operators */
    Bool operator<(const Int& other) const {
        return Bool(this->i < other.i);
    }
    Bool operator>=(const Int& other) const {
        return !(*this < other);
    }
    Bool operator>(const Int& other) const {
        return other < *this;
    }
    Bool operator<=(const Int& other) const {
        return !(other < *this);
    }
    Bool operator!=(const Int& other) const {
        return ((*this < other) || (other < *this));
    }
    Bool operator==(const Int& other) const {
        return !(*this != other);
    }

    /* Int output */
    friend std::ostream& operator<<(std::ostream& os, const Int& Int) {
        os << Int.i;
        return os;
    }

    String toString() const;

   private:
    int i;
};

#endif
