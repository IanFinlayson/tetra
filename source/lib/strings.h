/* string.h
 * the String class represents a (unicode) string in a Tetra program */

#ifndef TSTRING_H
#define TSTRING_H

#include <QString>
#include <QDebug>

#include "int.h"
#include "real.h"
#include "bool.h"
#include "values.h"

class String : public Value {
   public:
    String() {
        str = QString("");
    }

    String(const char* s) {
        str = QString(s);
    }

    String(const void* address) {
        str.sprintf("%lX",(unsigned long)address);
    }

    String(const Int& value);

    String(const Real& value);

    void push_back(QChar c) {
        str.push_back(c);
    }

    int find(char c) {
        return str.indexOf(QString(c));
    }

    Int toInt() {
        return Int(str.toInt());
    }

    Bool toBool() {
        QString lower = str.toLower();
        if (lower == "true" || lower == "1" || lower == "yes" || lower == "y") {
            return Bool(true);
        } else {
            return Bool(false);
        }
    }

    Real toReal() {
        return Real(str.toDouble());
    }

    QString toQ() const {
        return str;
    }

    void copyValue(const Value& other) {
        str = ((String&) other).str;
    }

    /* returns the unicode point for a character */
    unsigned short at(int index) const {
        return str.at(index).unicode();
    }

    friend String operator+(const String& lhs, const String& rhs);
    friend String operator+(const char* lhs, const String& rhs);
    friend String operator+(const String& lhs, const char* rhs);

    void replace(const String& before, const String& after, bool case_sensitive = true);

    String substring(int start, int len = -1) const;
    unsigned int length() const;
    bool empty() const;

    int indexOf(const String& s) const;

    String operator=(const char* rhs);
    String operator=(const String& rhs);
    String operator+=(const char* rhs);
    String operator+=(const String& rhs);

    friend bool operator<(const String& lhs, const String& rhs);
    friend bool operator==(const String& lhs, const String& rhs);
    friend bool operator<=(const String& lhs, const String& rhs);
    friend bool operator>(const String& lhs, const String& rhs);
    friend bool operator>=(const String& lhs, const String& rhs);
    friend bool operator!=(const String& lhs, const String& rhs);

    friend std::ostream& operator<<(std::ostream& os, const String& o) {
        os << o.str.toStdString();
        return os;
    }

    String toString() const {
        return *this;
    }

   private:
    QString str;
};

#endif
