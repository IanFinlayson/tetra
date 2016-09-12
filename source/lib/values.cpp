/* functions for working with data values */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include <QDebug>

#include "values.h"

Tstring operator+(const Tstring& lhs, const Tstring& rhs) {
    Tstring t;
    t.str = lhs.str + rhs.str;
    return t;
}

Tstring operator+(const char* lhs, const Tstring& rhs) {
    return Tstring(lhs) + rhs;
}

Tstring operator+(const Tstring& lhs, const char* rhs) {
    return lhs + Tstring(rhs);
}

Tstring Tstring::operator+=(const char* rhs) {
    str = str + QString(rhs);
    return *this;
}

Tstring Tstring::substring(int start, int len) const {
    Tstring t;
    t.str = str.mid(start, len);
    return t;
}

unsigned int Tstring::length() const {
    return str.length();
}

bool Tstring::empty() const {
    return str.length() == 0;
}

int Tstring::indexOf(const Tstring& s) const {
    return str.indexOf(s.str);
}

Tstring Tstring::operator+=(const Tstring& rhs) {
    str = str + rhs.str;
    return *this;
}

bool operator<(const Tstring& lhs, const Tstring& rhs) {
    return lhs.str < rhs.str;
}
bool operator==(const Tstring& lhs, const Tstring& rhs) {
    return lhs.str == rhs.str;
}

bool operator>(const Tstring& lhs, const Tstring& rhs) {
    return lhs.str > rhs.str;
}

bool operator>=(const Tstring& lhs, const Tstring& rhs) {
    return lhs.str >= rhs.str;
}
bool operator<=(const Tstring& lhs, const Tstring& rhs) {
    return lhs.str <= rhs.str;
}

bool operator!=(const Tstring& lhs, const Tstring& rhs) {
    return lhs.str != rhs.str;
}
