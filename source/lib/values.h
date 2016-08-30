/* data types used in the language */

#ifndef VALUES_H
#define VALUES_H

#include <QString>
#include <cmath>
#include "types.h"

/* represents any piece of data in a tetra program */
class Tdata {};

class Tbool : public Tdata {
   public:
    /* constructors */
    Tbool() {
        this->b = false;
    }
    Tbool(const bool& b) {
        this->b = b;
    }
    Tbool(const Tbool& other) {
        this->b = other.b;
    }

    /* assignment */
    Tbool operator=(const Tbool& other) {
        this->b = other.b;
        return *this;
    }

    /* conversion */
    operator bool const() {
        return this->b;
    }

    /* logical operators */
    Tbool operator&&(const Tbool& other) const {
        return Tbool(this->b && other.b);
    }
    Tbool operator||(const Tbool& other) const {
        return Tbool(this->b || other.b);
    }
    Tbool operator!() const {
        return Tbool(!this->b);
    }

    /* comparison operators */
    Tbool operator==(const Tbool& other) const {
        return Tbool(this->b == other.b);
    }
    Tbool operator!=(const Tbool& other) const {
        return !(this->b == other.b);
    }

    /* Tbool output */
    friend std::ostream& operator<<(std::ostream& os, const Tbool& tb) {
        tb.b ? os << "true" : os << "false";
        return os;
    }

   private:
    bool b;
};

class Tint : public Tdata {
   public:
    /* constructors */
    Tint() {
        this->i = 0;
    }
    Tint(const int& i) {
        this->i = i;
    }
    Tint(const Tint& other) {
        this->i = other.i;
    }

    /* assignment */
    Tint operator=(const Tint& other) {
        this->i = other.i;
        return *this;
    }

    /* conversion */
    int toInt() const {
        return i;
    }

    /* bitwise operators */
    Tint operator~() {
        return Tint(~(this->i));
    }
    Tint operator<<(const Tint& other) {
        return Tint(this->i << other.i);
    }
    Tint operator>>(const Tint& other) {
        return Tint(this->i >> other.i);
    }
    Tint operator&(const Tint& other) {
        return Tint(this->i & other.i);
    }
    Tint operator|(const Tint& other) {
        return Tint(this->i | other.i);
    }
    Tint operator^(const Tint& other) {
        return Tint(this->i ^ other.i);
    }

    /* arithmetic operators */
    Tint operator+(const Tint& other) {
        return Tint(this->i + other.i);
    }
    Tint operator-(const Tint& other) {
        return Tint(this->i - other.i);
    }
    Tint operator*(const Tint& other) {
        return Tint(this->i * other.i);
    }
    Tint operator/(const Tint& other) {
        return Tint(this->i / other.i);
    }
    Tint operator%(const Tint& other) {
        return Tint(this->i % other.i);
    }
    Tint pow(const Tint& other) {
        return Tint(std::pow(this->i, other.i));
    }

    /* unary operators */
    Tint operator+() {
        return Tint(+(this->i));
    }
    Tint operator-() {
        return Tint(-(this->i));
    }
    Tint operator++(int) {
        return Tint((this->i)++);
    }
    Tint operator++() {
        return Tint(++(this->i));
    }
    Tint operator--(int) {
        return Tint((this->i)--);
    }
    Tint operator--() {
        return Tint(--(this->i));
    }

    /* comparison operators */
    Tbool operator<(const Tint& other) const {
        return Tbool(this->i < other.i);
    }
    Tbool operator>=(const Tint& other) const {
        return !(*this < other);
    }
    Tbool operator>(const Tint& other) const {
        return other < *this;
    }
    Tbool operator<=(const Tint& other) const {
        return !(other < *this);
    }
    Tbool operator!=(const Tint& other) const {
        return ((*this < other) || (other < *this));
    }
    Tbool operator==(const Tint& other) const {
        return !(*this != other);
    }

    /* Tint output */
    friend std::ostream& operator<<(std::ostream& os, const Tint& Tint) {
        os << Tint.i;
        return os;
    }

   private:
    int i;
};

class Treal : public Tdata {
   public:
    Treal(double value) {
        r = value;
    }
    Treal() {
        r = 0.0;
    }

    double toDouble() const {
        return r;
    }

   private:
    double r;
};

class Tstring : public Tdata {
   public:
    Tstring() {
        str = QString("");
    }
    Tstring(const char* s) {
        str = QString(s);
    }

    Tstring(Tint value) {
        str = QString::number(value.toInt());
    }

    Tstring(Treal value) {
        str = QString::number(value.toDouble());
    }

    void push_back(QChar c) {
        str.push_back(c);
    }

    int find(char c) {
        return str.indexOf(QString(c));
    }

    Tint toInt() {
        return Tint(str.toInt());
    }

    Treal toReal() {
        return Treal(str.toDouble());
    }

    QString toQ() const {
        return str;
    }

    friend Tstring operator+(const Tstring& lhs, const Tstring& rhs);
    friend Tstring operator+(const char* lhs, const Tstring& rhs);
    friend Tstring operator+(const Tstring& lhs, const char* rhs);

    Tstring substring(int start, int len = -1) const;
    int length() const;
    bool empty() const;

    int indexOf(const Tstring& s) const;

    Tstring operator+=(const char* rhs);
    Tstring operator+=(const Tstring& rhs);

    friend bool operator<(const Tstring& lhs, const Tstring& rhs);
    friend bool operator==(const Tstring& lhs, const Tstring& rhs);
    friend bool operator<=(const Tstring& lhs, const Tstring& rhs);
    friend bool operator>(const Tstring& lhs, const Tstring& rhs);
    friend bool operator>=(const Tstring& lhs, const Tstring& rhs);
    friend bool operator!=(const Tstring& lhs, const Tstring& rhs);

    friend std::ostream& operator<<(std::ostream& os, const Tstring& o) {
        os << o.str.toStdString();
        return os;
    }

   private:
    QString str;
};

class Tlist : public Tdata {
   public:
   private:
};

class Tnone : public Tdata {
   public:
   private:
};

#endif
