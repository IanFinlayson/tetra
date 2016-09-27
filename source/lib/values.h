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

    Treal operator+(const Treal& other) {
        Treal result;
        result.r = r + other.r;
        return result;
    }

    Treal operator-(const Treal& other) {
        Treal result;
        result.r = r - other.r;
        return result;
    }

    Treal operator%(const Treal& other) {
        Treal result;
        result.r = fmod(r, other.r);
        return result;
    }

    Treal operator/(const Treal& other) {
        Treal result;
        result.r = r / other.r;
        return result;
    }

    Treal operator*(const Treal& other) {
        Treal result;
        result.r = r * other.r;
        return result;
    }

    Treal pow(const Treal& other) {
        Treal result;
        result.r = std::pow(r, other.r);
        return result;
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
    Tdata* opAssign(const Tdata* other) {
        /* copy our data type and also value */
        this->type = other->type;
        this->value = other->value;

        /* return a pointer to this Tdata */
        return this;
    }

    Tdata* opPlus(const Tdata* other) {
        /* create the result variable */
        Tdata* result = create(&type, NULL);

        /* do different things depending on type TODO what happens with real+int etc. */
        switch (type.getKind()) {
            case TYPE_INT:
                result->value->copyValue(*((Tint*) value) + *((Tint*) other->value));
                break;
            case TYPE_REAL:
                result->value->copyValue(*((Treal*) value) + *((Treal*) other->value));
                break;
            case TYPE_STRING:
                result->value->copyValue(*((Tstring*) value) + *((Tstring*) other->value));
                break;
            default:
                throw RuntimeError("Unhandled operands to + operator", 0);
        }

        return result;
    }

    Tdata* opMinus(const Tdata* other) {
        /* create the result variable */
        Tdata* result = create(&type, NULL);

        /* do different things depending on type TODO what happens with real-int etc. */
        switch (type.getKind()) {
            case TYPE_INT:
                result->value->copyValue(*((Tint*) value) - *((Tint*) other->value));
                break;
            case TYPE_REAL:
                result->value->copyValue(*((Treal*) value) - *((Treal*) other->value));
                break;
            default:
                throw RuntimeError("Unhandled operands to - operator", 0);
        }

        return result;
    }

    Tdata* opTimes(const Tdata* other) {
        /* create the result variable */
        Tdata* result = create(&type, NULL);

        /* do different things depending on type TODO what happens with real*int etc. */
        switch (type.getKind()) {
            case TYPE_INT:
                result->value->copyValue(*((Tint*) value) * *((Tint*) other->value));
                break;
            case TYPE_REAL:
                result->value->copyValue(*((Treal*) value) * *((Treal*) other->value));
                break;
            default:
                throw RuntimeError("Unhandled operands to * operator", 0);
        }

        return result;
    }

    Tdata* opDivide(const Tdata* other) {
        /* create the result variable */
        Tdata* result = create(&type, NULL);

        /* do different things depending on type TODO what happens with real/int etc. */
        switch (type.getKind()) {
            case TYPE_INT:
                result->value->copyValue(*((Tint*) value) / *((Tint*) other->value));
                break;
            case TYPE_REAL:
                result->value->copyValue(*((Treal*) value) / *((Treal*) other->value));
                break;
            default:
                throw RuntimeError("Unhandled operands to / operator", 0);
        }

        return result;
    }

    Tdata* opModulus(const Tdata* other) {
        /* create the result variable */
        Tdata* result = create(&type, NULL);

        /* this onw only works for integers */
        switch (type.getKind()) {
            case TYPE_INT:
                result->value->copyValue(*((Tint*) value) % *((Tint*) other->value));
                break;
            case TYPE_REAL:
                result->value->copyValue(*((Treal*) value) % *((Treal*) other->value));
                break;
            default:
                throw RuntimeError("Unhandled operands to % operator", 0);
        }

        return result;
    }

    Tdata* opExp(const Tdata* other) {
        /* create the result variable */
        Tdata* result = create(&type, NULL);

        /* do different things depending on type TODO what happens with real**int etc. */
        switch (type.getKind()) {
            case TYPE_INT:
                result->value->copyValue(((Tint*) value)->pow(*((Tint*) other->value)));
                break;
            case TYPE_REAL:
                result->value->copyValue(((Treal*) value)->pow(*((Treal*) other->value)));
                break;
            default:
                throw RuntimeError("Unhandled operands to ** operator", 0);
        }

        return result;
    }
    



    /* TODO add the rest of these */
    Tdata* opOr(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opAnd(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opLt(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opLte(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opGt(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opGte(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opEq(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opNeq(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opNot(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opBitxor(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opBitand(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opBitor(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opBitnot(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opShiftl(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opShiftr(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }
    Tdata* opDot(const Tdata* other) {
        UNUSED(other);
        return NULL;
    }

    /* create a Tdata of a given type */
    static Tdata* create(DataType* type, const Tvalue* value) {
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

        /* copy the actual value in, if given */
        if (value) {
            newData->value->copyValue(*value);
        }

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
