/* values.cpp
 * this file contains functions for working with data values and implements the
 * value classes e.g. Int, String, etc. */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include <QDebug>

#include "tetra.h"

const String List::L_DELIM = "[";
const String List::R_DELIM = "]";
const String Tuple::L_DELIM = "(";
const String Tuple::R_DELIM = ")";

String operator+(const String& lhs, const String& rhs) {
    String t;
    t.str = lhs.str + rhs.str;
    return t;
}

String operator+(const char* lhs, const String& rhs) {
    return String(lhs) + rhs;
}

String operator+(const String& lhs, const char* rhs) {
    return lhs + String(rhs);
}

String String::operator=(const char* rhs) {
    str = QString(rhs);
    return *this;
}

String String::operator+=(const char* rhs) {
    str = str + QString(rhs);
    return *this;
}

String String::substring(int start, int len) const {
    String t;
    t.str = str.mid(start, len);
    return t;
}

unsigned int String::length() const {
    return str.length();
}

bool String::empty() const {
    return str.length() == 0;
}

int String::indexOf(const String& s) const {
    return str.indexOf(s.str);
}

String String::operator=(const String& rhs) {
    str = rhs.str;
    return *this;
}

String String::operator+=(const String& rhs) {
    str = str + rhs.str;
    return *this;
}

bool operator<(const String& lhs, const String& rhs) {
    return lhs.str < rhs.str;
}
bool operator==(const String& lhs, const String& rhs) {
    return lhs.str == rhs.str;
}

bool operator>(const String& lhs, const String& rhs) {
    return lhs.str > rhs.str;
}

bool operator>=(const String& lhs, const String& rhs) {
    return lhs.str >= rhs.str;
}
bool operator<=(const String& lhs, const String& rhs) {
    return lhs.str <= rhs.str;
}

bool operator!=(const String& lhs, const String& rhs) {
    return lhs.str != rhs.str;
}

String::String(const Int& value) {
    str = QString::number(value.toInt());
}

String::String(const Real& value) {
    str = QString::number(value.toDouble());
}

String Bool::toString() const {
    return b ? String("true") : String("false");
}

String Int::toString() const {
    return String(*this);
}
String Real::toString() const {
    return String(*this);
}

String Container::toString() const {
    String result = getLDelim();
    result += getValString();
    result += getRDelim();
    return result;
}

String Dict::toString() const {
    String result = "{";
    unsigned elemsLeft = values.size();

    for (auto const& pair : values) {
        DataTypeKind keyKind = pair.second[0]->getType()->getKind();
        DataTypeKind valKind = pair.second[1]->getType()->getKind();

        String keyOuter = (keyKind == TYPE_STRING) ? "'" : "";
        String valOuter = (valKind == TYPE_STRING) ? "'" : "";

        result += keyOuter;
        result += pair.first;
        result += keyOuter;

        result += ":";

        result += valOuter;
        result += pair.second[1]->getValue()->toString();
        result += valOuter;

        elemsLeft--;

        if (elemsLeft) {
            result += ", ";
        }
    }

    result += "}";
    return result;
}
