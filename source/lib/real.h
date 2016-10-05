/* real.h
 * the Real class represents a real number in a Tetra program */

#include "values.h"

#ifndef REAL_H
#define REAL_H

class Real : public Value {
   public:
    Real(double value) {
        r = value;
    }
    Real() {
        r = 0.0;
    }

    Real operator=(const Real& other) {
        this->r = other.r;
        return *this;
    }

    Real operator+(const Real& other) {
        Real result;
        result.r = r + other.r;
        return result;
    }

    Real operator-(const Real& other) {
        Real result;
        result.r = r - other.r;
        return result;
    }

    Real operator%(const Real& other) {
        Real result;
        result.r = fmod(r, other.r);
        return result;
    }

    bool operator==(const Real& other) {
        return r == other.r;
    }
    bool operator!=(const Real& other) {
        return r != other.r;
    }
    bool operator<=(const Real& other) {
        return r <= other.r;
    }
    bool operator>=(const Real& other) {
        return r >= other.r;
    }
    bool operator<(const Real& other) {
        return r < other.r;
    }
    bool operator>(const Real& other) {
        return r > other.r;
    }

    Real operator/(const Real& other) {
        Real result;
        result.r = r / other.r;
        return result;
    }

    Real operator*(const Real& other) {
        Real result;
        result.r = r * other.r;
        return result;
    }

    Real pow(const Real& other) {
        Real result;
        result.r = std::pow(r, other.r);
        return result;
    }

    double toDouble() const {
        return r;
    }

    void copyValue(const Value& other) {
        r = ((Real&) other).r;
    }

    String toString() const;

   private:
    double r;
};

#endif
