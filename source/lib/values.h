/* data types used in the language */

#ifndef VALUES_H
#define VALUES_H

#include <QString>
#include <QDebug>
#include <cmath>
#include <iostream>

#include "types.h"

/* forward declare classes so they cen reference each other */
class Tdata;
class Tstring;
class Tint;
class Treal;
class Tbool;

/* represents one Tetra value, e.g. a number, string, list, anything */
class Tvalue {
   public:
    virtual ~Tvalue() {}
    virtual Tstring toString() const = 0;

    virtual void copyValue(const Tvalue& other) = 0;
};

class Tbool : public Tvalue {
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
    bool toBool() {
        return this->b;
    }

    void copyValue(const Tvalue& other) {
        b = ((Tbool&) other).b;
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

    Tstring toString() const;

   private:
    bool b;
};

class Tint : public Tvalue {
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

    void copyValue(const Tvalue& other) {
        i = ((Tint&) other).i;
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

    Tstring toString() const;

   private:
    int i;
};

class Treal : public Tvalue {
   public:
    Treal(double value) {
        r = value;
    }
    Treal() {
        r = 0.0;
    }

    Treal operator=(const Treal& other) {
        this->r = other.r;
        return *this;
    }

    double toDouble() const {
        return r;
    }

    void copyValue(const Tvalue& other) {
        r = ((Treal&) other).r;
    }

    Tstring toString() const;

   private:
    double r;
};

class Tlist : public Tvalue {
   public:
   private:
};

class Tnone : public Tvalue {
   public:
   private:
};

class Tstring : public Tvalue {
   public:
    Tstring() {
        str = QString("");
    }
    Tstring(const char* s) {
        str = QString(s);
    }

    Tstring(const Tint& value);

    Tstring(const Treal& value);

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

    void copyValue(const Tvalue& other) {
        str = ((Tstring&) other).str;
    }

    /* returns the unicode point for a character */
    unsigned short at(int index) const {
        return str.at(index).unicode();
    }

    friend Tstring operator+(const Tstring& lhs, const Tstring& rhs);
    friend Tstring operator+(const char* lhs, const Tstring& rhs);
    friend Tstring operator+(const Tstring& lhs, const char* rhs);

    Tstring substring(int start, int len = -1) const;
    unsigned int length() const;
    bool empty() const;

    int indexOf(const Tstring& s) const;

    Tstring operator=(const char* rhs);
    Tstring operator=(const Tstring& rhs);
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

    Tstring toString() const {
        return *this;
    }

   private:
    QString str;
};

#include "error.h"

/* represents any piece of data in a tetra program */
class Tdata {
   public:
    /* all of the operators */
    Tdata* opDot(const Tdata* other);
    Tdata* opAssign(const Tdata* other) {
        /* copy our data type and also value */
        this->type = other->type;
        this->value = other->value;

        /* return a pointer to this Tdata */
        return this;
    }
    Tdata* opOr(const Tdata* other);
    Tdata* opAnd(const Tdata* other);
    Tdata* opLt(const Tdata* other);
    Tdata* opLte(const Tdata* other);
    Tdata* opGt(const Tdata* other);
    Tdata* opGte(const Tdata* other);
    Tdata* opEq(const Tdata* other);
    Tdata* opNeq(const Tdata* other);
    Tdata* opNot(const Tdata* other);
    Tdata* opBitxor(const Tdata* other);
    Tdata* opBitand(const Tdata* other);
    Tdata* opBitor(const Tdata* other);
    Tdata* opBitnot(const Tdata* other);
    Tdata* opShiftl(const Tdata* other);
    Tdata* opShiftr(const Tdata* other);
    Tdata* opPlus(const Tdata* other);
    Tdata* opMinus(const Tdata* other);
    Tdata* opTimes(const Tdata* other);
    Tdata* opDivide(const Tdata* other);
    Tdata* opModulus(const Tdata* other);
    Tdata* opExp(const Tdata* other);

    /* create a Tdata of a given type */
    static Tdata* create(DataType* type, const Tvalue& value) {
        /* make a new Tdata currently a memory leak - TODO gc */
        Tdata* newData = new Tdata;

        /* copy the type over */
        newData->type = *type;

        /* set the value to be the right sub type */
        switch (type->getKind()) {
            case TYPE_INT:
                newData->value = new Tint();
                break;
            case TYPE_REAL:
                newData->value = new Treal();
                break;
            case TYPE_STRING:
                newData->value = new Tstring();
                break;
            case TYPE_BOOL:
                newData->value = new Tbool();
                break;
            default:
                throw RuntimeError("Unhandled data type in Tdata::create", 0);
        }

        /* copy the actual value in */
        newData->value->copyValue(value);

        /* return it */
        return newData;
    }

    /* return the value of this */
    Tvalue* getValue() {
        return value;
    }

   private:
    /* Tdata are garbage collected, so we can't create them directly
     * only by calling the create methods above */
    Tdata() : type(TYPE_NONE), value(NULL) {}

    /* the data type and the actual value */
    DataType type;
    Tvalue* value;
};

#endif


