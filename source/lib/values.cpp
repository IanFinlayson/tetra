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

String List::toString() const {
    String result = "[";

    for (unsigned i = 0; i < values.size(); i++) {
        result += values[i]->getValue()->toString();

        /* if not the last, print a comma */
        if ((i + 1) < values.size()) {
            result += ", ";
        }
    }

    result += "]";
    return result;
}

String Dict::toString() const {
    String result = "{";
    unsigned elemsLeft = values.size();

    for (auto const &pair : values) {
        result += String(pair.first.c_str());
        result += ":";
        result += pair.second.second->getValue()->toString();
        elemsLeft--;
        if (elemsLeft) {
          result += ", ";
        }
    }

    result += "}";
    return result;
}
