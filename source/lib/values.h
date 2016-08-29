/* data types used in the language */

#ifndef VALUES_H
#define VALUES_H

#include <QString>
#include <cmath>
#include "types.h"

/* represents any piece of data in a tetra program */
class tdata {
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
    int toInt() const {return i;}

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
      treal() {
          r = 0.0;
      }

      double toDouble() const {
          return r;
      }
  private:
      double r;
};


class tstring : public tdata {
    public:
        tstring() {
            str = QString("");
        }
        tstring(const char* s) {
            str = QString(s);
        }

        tstring(tint val) {
            str = QString::number(val.toInt());
        }

        tstring(treal val) {
            str = QString::number(val.toDouble());
        }

        void push_back(QChar c) {
            str.push_back(c);
        }

        int find(char c) {
            return str.indexOf(QString(c));
        }

        tint toInt() {
            return tint(str.toInt());
        }

        treal toReal() {
            return treal(str.toDouble());
        }

        QString toQ() const {
            return str;
        }

        friend tstring operator+(const tstring& lhs, const tstring& rhs);
        friend tstring operator+(const char* lhs, const tstring& rhs);
        friend tstring operator+(const tstring& lhs, const char* rhs);

        tstring substring(int start, int len = -1) const;
        int length() const;
        bool empty() const;

        int indexOf(const tstring& s) const;

        tstring operator+=(const char* rhs);
        tstring operator+=(const tstring& rhs);

        friend bool operator<(const tstring& lhs, const tstring& rhs);
        friend bool operator==(const tstring& lhs, const tstring& rhs);
        friend bool operator<=(const tstring& lhs, const tstring& rhs);
        friend bool operator>(const tstring& lhs, const tstring& rhs);
        friend bool operator>=(const tstring& lhs, const tstring& rhs);
        friend bool operator!=(const tstring& lhs, const tstring& rhs);

        friend std::ostream& operator<<(std::ostream& os, const tstring& o) {
            os << o.str.toStdString();
            return os;
        }

  private:
        QString str;
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


