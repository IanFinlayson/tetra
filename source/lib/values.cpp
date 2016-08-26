/* functions for working with data values */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include <QDebug>

#include "values.h"


tstring tdata::toString() const {
    return "hi";
}

int tdata::toInt() const {
    return 32;
}

tstring operator+(const tstring& lhs, const tstring& rhs) {
    tstring t;
    t.str = lhs.str + rhs.str;
    return t;
}

tstring operator+(const char* lhs, const tstring& rhs) {
    return tstring(lhs) + rhs;
}

tstring operator+(const tstring& lhs, const char* rhs) {
    return lhs + tstring(rhs);
}

tstring tstring::operator+=(const char* rhs) {
    str = str + QString(rhs);
    return *this;
}

tstring tstring::substring(int start, int len) const {
    tstring t;
    t.str = str.mid(start, len);
    return t;
}

int tstring::length() const {
    return str.length();
}

bool tstring::empty() const {
    return str.length() == 0;
}

int tstring::indexOf(const tstring& s) const {
    return str.indexOf(s.str);
}

tstring tstring::operator+=(const tstring& rhs) {
    str = str + rhs.str;
    return *this;
}

bool operator<(const tstring& lhs, const tstring& rhs) {
    return lhs.str < rhs.str;
}
bool operator==(const tstring& lhs, const tstring& rhs) {
    return lhs.str == rhs.str;
}

bool operator>(const tstring& lhs, const tstring& rhs) {
    return lhs.str > rhs.str;
}

bool operator>=(const tstring& lhs, const tstring& rhs) {
    return lhs.str >= rhs.str;
}
bool operator<=(const tstring& lhs, const tstring& rhs) {
    return lhs.str <= rhs.str;
}

bool operator!=(const tstring& lhs, const tstring& rhs) {
    return lhs.str != rhs.str;
}

