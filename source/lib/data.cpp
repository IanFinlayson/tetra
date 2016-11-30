/* data.cpp
 * implements the Data class which represents any allocated Tetra data value */

#include <QString>
#include <QDebug>
#include <cmath>
#include <iostream>

#include "types.h"
#include "error.h"
#include "data.h"
#include "strings.h"
#include "int.h"
#include "real.h"
#include "bool.h"
#include "list.h"
#include "dict.h"
#include "tuple.h"

Data* Data::opAssign(const Data* other) {
    /* copy our data type and also value */
    this->type = other->type;
    this->value = other->value;

    /* return a pointer to this Data */
    return this;
}

/* operator methods
 * TODO: a lot of duplication here, how can these best be generalized?
 */
Data* Data::opPlus(const Data* other) {
    /* create the result variable */
    Data* result = create(&type, NULL);

    /* do different things depending on type what happens with real+int etc. */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(*((Int*) value) + *((Int*) other->value));
            break;
        case TYPE_REAL:
            result->value->copyValue(*((Real*) value) + *((Real*) other->value));
            break;
        case TYPE_STRING:
            result->value->copyValue(*((String*) value) + *((String*) other->value));
            break;
        case TYPE_LIST:
            result->value->copyValue(*((List*) value) + *((List*) other->value));
            break;
        default:
            throw RuntimeError("Unhandled operands to + operator", 0);
    }

    return result;
}

Data* Data::opMinus(const Data* other) {
    /* create the result variable */
    Data* result = create(&type, NULL);

    /* do different things depending on type what happens with real-int etc. */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(*((Int*) value) - *((Int*) other->value));
            break;
        case TYPE_REAL:
            result->value->copyValue(*((Real*) value) - *((Real*) other->value));
            break;
        default:
            throw RuntimeError("Unhandled operands to - operator", 0);
    }

    return result;
}

Data* Data::opTimes(const Data* other) {
    /* create the result variable */
    Data* result = create(&type, NULL);

    /* do different things depending on type what happens with real*int etc. */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(*((Int*) value) * *((Int*) other->value));
            break;
        case TYPE_REAL:
            result->value->copyValue(*((Real*) value) * *((Real*) other->value));
            break;
        default:
            throw RuntimeError("Unhandled operands to * operator", 0);
    }

    return result;
}

Data* Data::opDivide(const Data* other) {
    /* create the result variable */
    Data* result = create(&type, NULL);

    /* do different things depending on type what happens with real/int etc. */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(*((Int*) value) / *((Int*) other->value));
            break;
        case TYPE_REAL:
            result->value->copyValue(*((Real*) value) / *((Real*) other->value));
            break;
        default:
            throw RuntimeError("Unhandled operands to / operator", 0);
    }

    return result;
}

Data* Data::opModulus(const Data* other) {
    /* create the result variable */
    Data* result = create(&type, NULL);

    /* do different things depending on type what happens with real%int etc. */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(*((Int*) value) % *((Int*) other->value));
            break;
        case TYPE_REAL:
            result->value->copyValue(*((Real*) value) % *((Real*) other->value));
            break;
        default:
            throw RuntimeError("Unhandled operands to % operator", 0);
    }

    return result;
}

Data* Data::opExp(const Data* other) {
    /* create the result variable */
    Data* result = create(&type, NULL);

    /* do different things depending on type what happens with real**int etc. */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(((Int*) value)->pow(*((Int*) other->value)));
            break;
        case TYPE_REAL:
            result->value->copyValue(((Real*) value)->pow(*((Real*) other->value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to ** operator", 0);
    }

    return result;
}

Data* Data::opEq(const Data* other) {
    /* create the bool we'll return */
    DataType boolType(TYPE_BOOL);
    Data* result = create(&boolType, NULL);

    /* compare based on the types */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(Bool((*((Int*) value)) == (*((Int*) other->value))));
            break;
        case TYPE_REAL:
            result->value->copyValue(Bool((*((Real*) value)) == (*((Real*) other->value))));
            break;
        case TYPE_STRING:
            result->value->copyValue(Bool((*((String*) value)) == (*((String*) other->value))));
            break;
        case TYPE_BOOL:
            result->value->copyValue(Bool((*((Bool*) value)) == (*((Bool*) other->value))));
            break;
        default:
            throw RuntimeError("Unhandled operands to == operator", 0);
    }

    return result;
}

Data* Data::opLt(const Data* other) {
    /* create the bool we'll return */
    DataType boolType(TYPE_BOOL);
    Data* result = create(&boolType, NULL);

    /* compare based on the types */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(Bool((*((Int*) value)) < (*((Int*) other->value))));
            break;
        case TYPE_REAL:
            result->value->copyValue(Bool((*((Real*) value)) < (*((Real*) other->value))));
            break;
        case TYPE_STRING:
            result->value->copyValue(Bool((*((String*) value)) < (*((String*) other->value))));
            break;
        default:
            throw RuntimeError("Unhandled operands to < operator", 0);
    }

    return result;
}

Data* Data::opLte(const Data* other) {
    /* create the bool we'll return */
    DataType boolType(TYPE_BOOL);
    Data* result = create(&boolType, NULL);

    /* compare based on the types */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(Bool((*((Int*) value)) <= (*((Int*) other->value))));
            break;
        case TYPE_REAL:
            result->value->copyValue(Bool((*((Real*) value)) <= (*((Real*) other->value))));
            break;
        case TYPE_STRING:
            result->value->copyValue(Bool((*((String*) value)) <= (*((String*) other->value))));
            break;
        default:
            throw RuntimeError("Unhandled operands to <= operator", 0);
    }

    return result;
}

Data* Data::opGt(const Data* other) {
    /* create the bool we'll return */
    DataType boolType(TYPE_BOOL);
    Data* result = create(&boolType, NULL);

    /* compare based on the types */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(Bool((*((Int*) value)) > (*((Int*) other->value))));
            break;
        case TYPE_REAL:
            result->value->copyValue(Bool((*((Real*) value)) > (*((Real*) other->value))));
            break;
        case TYPE_STRING:
            result->value->copyValue(Bool((*((String*) value)) > (*((String*) other->value))));
            break;
        default:
            throw RuntimeError("Unhandled operands to > operator", 0);
    }

    return result;
}

Data* Data::opGte(const Data* other) {
    /* create the bool we'll return */
    DataType boolType(TYPE_BOOL);
    Data* result = create(&boolType, NULL);

    /* compare based on the types */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(Bool((*((Int*) value)) >= (*((Int*) other->value))));
            break;
        case TYPE_REAL:
            result->value->copyValue(Bool((*((Real*) value)) >= (*((Real*) other->value))));
            break;
        case TYPE_STRING:
            result->value->copyValue(Bool((*((String*) value)) >= (*((String*) other->value))));
            break;
        default:
            throw RuntimeError("Unhandled operands to >= operator", 0);
    }

    return result;
}

Data* Data::opNeq(const Data* other) {
    /* create the bool we'll return */
    DataType boolType(TYPE_BOOL);
    Data* result = create(&boolType, NULL);

    /* compare based on the types */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(Bool((*((Int*) value)) != (*((Int*) other->value))));
            break;
        case TYPE_REAL:
            result->value->copyValue(Bool((*((Real*) value)) != (*((Real*) other->value))));
            break;
        case TYPE_STRING:
            result->value->copyValue(Bool((*((String*) value)) != (*((String*) other->value))));
            break;
        case TYPE_BOOL:
            result->value->copyValue(Bool((*((Bool*) value)) != (*((Bool*) other->value))));
            break;
        default:
            throw RuntimeError("Unhandled operands to != operator", 0);
    }

    return result;
}

Data* Data::opBitxor(const Data* other) {
    /* create the resulting value */
    Data* result = create(&type, NULL);

    /* only ints are allowed for this */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue((*((Int*) value)) ^ (*((Int*) other->value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to ^ operator", 0);
    }

    return result;
}

Data* Data::opBitand(const Data* other) {
    /* create the resulting value */
    Data* result = create(&type, NULL);

    /* only ints are allowed for this */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue((*((Int*) value)) & (*((Int*) other->value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to & operator", 0);
    }

    return result;
}

Data* Data::opBitor(const Data* other) {
    /* create the resulting value */
    Data* result = create(&type, NULL);

    /* only ints are allowed for this */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue((*((Int*) value)) | (*((Int*) other->value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to | operator", 0);
    }

    return result;
}

Data* Data::opBitnot() {
    /* create the result */
    Data* result = create(&type, NULL);

    /* only ints are allowed for this */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(~(*((Int*) value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to ~ operator", 0);
    }

    return result;
}

Data* Data::opShiftl(const Data* other) {
    /* create the resulting value */
    Data* result = create(&type, NULL);

    /* only ints are allowed for this */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue((*((Int*) value)) << (*((Int*) other->value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to << operator", 0);
    }

    return result;
}

Data* Data::opShiftr(const Data* other) {
    /* create the resulting value */
    Data* result = create(&type, NULL);

    /* only ints are allowed for this */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue((*((Int*) value)) >> (*((Int*) other->value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to >> operator", 0);
    }

    return result;
}

Data* Data::opOr(const Data* other) {
    /* create the resulting value */
    Data* result = create(&type, NULL);

    /* only bools are allowed for this */
    switch (type.getKind()) {
        case TYPE_BOOL:
            result->value->copyValue((*((Bool*) value)) || (*((Bool*) other->value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to or operator", 0);
    }

    return result;
}

Data* Data::opAnd(const Data* other) {
    /* create the resulting value */
    Data* result = create(&type, NULL);

    /* only bools are allowed for this */
    switch (type.getKind()) {
        case TYPE_BOOL:
            result->value->copyValue((*((Bool*) value)) && (*((Bool*) other->value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to and operator", 0);
    }

    return result;
}

Data* Data::opNot() {
    /* create the result */
    Data* result = create(&type, NULL);

    /* only bools are allowed for this */
    switch (type.getKind()) {
        case TYPE_BOOL:
            result->value->copyValue(!(*((Bool*) value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to not operator", 0);
    }

    return result;
}

Data* Data::opNegate() {
    /* create the result */
    Data* result = create(&type, NULL);

    /* only numeric types are allowed for this */
    switch (type.getKind()) {
        case TYPE_INT:
            result->value->copyValue(-(*((Int*) value)));
            break;
        case TYPE_REAL:
            result->value->copyValue(-(*((Real*) value)));
            break;
        default:
            throw RuntimeError("Unhandled operands to not operator", 0);
    }

    return result;
}

Data* Data::opIndex(Data* other, bool isLValue) {
    switch (type.getKind()) {
        case TYPE_DICT:  
            if (isLValue && !(((Dict*) value)->hasKey(other))) {
                Pair p;
                p.set(other, Data::create(&((*(type.subtypes))[1]), nullptr));
                ((Dict*)value)->add(Data::create(PAIR_TYPE,&p));
             }
        case TYPE_LIST:
            return ((Container*)value)->get(other);
        case TYPE_STRING: {
            int index = ((Int*) other->value)->toInt();
            String letter = ((String*) value)->substring(index, 1);
            return Data::create(&type, &letter);
        }
        default:
            throw RuntimeError("Unhandled operands to + operator", 0);
    }
}

/* create a Data of a given type */
Data* Data::create(DataType* type, const Value* value) {
    /* make a new Data currently a memory leak - TODO gc */
    Data* newData = new Data;

    /* copy the type over */
    newData->type = *type;

    /* set the value to be the right sub type */
    switch (type->getKind()) {
        case TYPE_INT:
            newData->value = new Int();
            break;
        case TYPE_REAL:
            newData->value = new Real();
            break;
        case TYPE_STRING:
            newData->value = new String();
            break;
        case TYPE_BOOL:
            newData->value = new Bool();
            break;
        case TYPE_TUPLE:
            newData->value = new Tuple();
            break;
        case TYPE_LIST:
            newData->value = new List();
            break;
        case TYPE_DICT:
            newData->value = new Dict();
            break;
        case TYPE_PAIR:
            newData->value = new Pair();
            break;
        default:
            throw RuntimeError("Unhandled data type in Data::create", 0);
    }

    /* copy the actual value in, if given */
    if (value) {
        newData->value->copyValue(*value);
    }

    /* return it */
    return newData;
}
