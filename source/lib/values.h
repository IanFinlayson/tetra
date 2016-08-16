/* data types used in the language */

#ifndef VALUES_H
#define VALUES_H

#include <cmath>
#include "types.h"

/* represents any piece of data in a tetra program */
class tdata {
    public:
        tstring toString() const;
        int toInt() const;
};

class tbool : public tdata {
  public:
    /* constructors */
    tbool() {this->b = false;}
    tbool(const bool& b) {this->b = b;}
    tbool(const tbool& other) {this->b = other.b;}

    /*assignment */
    tbool operator=(const tbool& other) {
      this->b = other.b;
      return *this;
    }

    /* conversion */
    operator bool const (){return this->b;}

    /* logical operators */
    tbool operator&&(const tbool& other) const {return tbool(this->b && other.b);}
    tbool operator||(const tbool& other) const {return tbool(this->b || other.b);}
    tbool operator!() const {return tbool(!this->b);}

    /* comparison operators */
    tbool operator==(const tbool& other) const {return tbool(this->b == other.b);}
    tbool operator!=(const tbool& other) const {return !(this->b == other.b);}

    /* tbool output */
    friend std::ostream& operator<<(std::ostream& os, const tbool& tb) {
      tb.b ? os << "true" : os << "false";
      return os;
    }

  private:
    bool b;
};


class tint : public tdata {
  public:
    /* constructors */
    tint() {this->i = 0;}
    tint(const int& i) {this->i = i;}
    tint(const tint& other) {this->i = other.i;}

    /*assignment */
    tint operator=(const tint& other) {
      this->i = other.i;
      return *this;
    }

    /* conversion */
    explicit operator int const (){return this->i;}
    explicit operator unsigned long int const (){return this->i;}

    /* bitwise operators */
    tint operator~() {return tint(~(this->i));}
    tint operator<<(const tint& other) {return tint(this->i << other.i);}
    tint operator>>(const tint& other) {return tint(this->i >> other.i);}
    tint operator&(const tint& other) {return tint(this->i & other.i);}
    tint operator|(const tint& other) {return tint(this->i | other.i);}
    tint operator^(const tint& other) {return tint(this->i ^ other.i);}

    /* arithmetic operators */
    tint operator+(const tint& other) {return tint(this->i + other.i);}
    tint operator-(const tint& other) {return tint(this->i - other.i);}
    tint operator*(const tint& other) {return tint(this->i * other.i);}
    tint operator/(const tint& other) {return tint(this->i / other.i);}
    tint operator%(const tint& other) {return tint(this->i % other.i);}
    tint pow(const tint& other) {return tint(std::pow(this->i, other.i));}

    /* unary operators */
    tint operator+() {return tint(+(this->i));}
    tint operator-() {return tint(-(this->i));}
    tint operator++(int) {return tint((this->i)++);}
    tint operator++() {return tint(++(this->i));}
    tint operator--(int) {return tint((this->i)--);}
    tint operator--() {return tint(--(this->i));}

    /* comparison operators */
    tbool operator<(const tint& other) const {return tbool(this->i < other.i);}
    tbool operator>=(const tint& other) const {return !(*this < other);}
    tbool operator>(const tint& other) const {return other < *this;}
    tbool operator<=(const tint& other) const {return !(other < *this);}
    tbool operator!=(const tint& other) const {return ((*this < other) || (other < *this));}
    tbool operator==(const tint& other) const {return !(*this != other);}

    /* tint output */
    friend std::ostream& operator<<(std::ostream& os, const tint& tint) {
      os << tint.i;
      return os;
    }

  private:
    int i;
};

class treal : public tdata {
  public:
      treal(double val) {
          r = val;
      }
  private:
      double r;
};


class tstring : public tdata, public std::string {
    public:
        tstring() {
            *this = "";
        }
        tstring(const char* s) {
            *this = s;
        }
        tstring(const std::string& s) {
            *this = s;
        }

  private:
};

class tlist : public tdata {
  public:
  private:
};

class tnone : public tdata {
  public:
  private:
};

#endif


