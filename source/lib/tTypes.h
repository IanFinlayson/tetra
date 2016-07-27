/* This file defines basic tetra data types as
 * wrappers around their c++ counterparts */

#include <cmath>


class tbool {
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
    friend ostream& operator<<(ostream& os, const tbool& tb) {
      tb.b ? os << "true" : os << "false";
      return os;
    }

  private:
    bool b;
};


class tint {
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
    friend ostream& operator<<(ostream& os, const tint& tint) {
      os << tint.i;
      return os;
    }

  private:
    int i;
};

class treal {
  public:
  private:
};
/*
class tstring {
  public:
  private:
};

*/
class tnone {
  public:
  private:
};
