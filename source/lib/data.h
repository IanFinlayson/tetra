/* data.h
 * declares the Data class which represents any allocated Tetra data value */

#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QDebug>
#include <cmath>
#include <iostream>

#include "types.h"
#include "error.h"

/* represents any piece of data in a tetra program */
class Data {
   public:
    Data* opAssign(const Data* other);

    /* operator methods */
    Data* opPlus(const Data* other);
    Data* opMinus(const Data* other);
    Data* opTimes(const Data* other);
    Data* opDivide(const Data* other);
    Data* opModulus(const Data* other);
    Data* opExp(const Data* other);
    Data* opEq(const Data* other);
    Data* opLt(const Data* other);
    Data* opLte(const Data* other);
    Data* opGt(const Data* other);
    Data* opGte(const Data* other);
    Data* opNeq(const Data* other);
    Data* opBitxor(const Data* other);
    Data* opBitand(const Data* other);
    Data* opBitor(const Data* other);
    Data* opBitnot();
    Data* opShiftl(const Data* other);
    Data* opShiftr(const Data* other);
    Data* opOr(const Data* other);
    Data* opAnd(const Data* other);
    Data* opNot();
    Data* opNegate();
    Data* opIndex(const Data* other);

    /* create a Data of a given type */
    static Data* create(DataType* type, const Value* value);

    /* return the value of this */
    Value* getValue() const{
        return value;
    }

    /* return the data type of this */
    DataType* getType() {
        return &type;
    }

   private:
    /* Data are garbage collected, so we can't create them directly
     * only by calling the create methods above */
    Data() : type(TYPE_NONE), value(NULL) {}

    /* the data type and the actual value */
    DataType type;
    Value* value;
};

#endif
