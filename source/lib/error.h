/* error types */

#ifndef ERROR_H
#define ERROR_H

#include <map>
#include <string>
#include <vector>

#include "types.h"
#include "values.h"

/* any type of error is handled with this exception */
class Error {
   public:
    Error(const Tstring& mesg, int lineNumber = 0);
    Tstring getMessage() const;
    int getLine() const;

   private:
    Tstring mesg;
    int lineNumber;
};

class RuntimeError : public Error {
   public:
    RuntimeError(const Tstring& message, int lineNumber);
};

class InterruptError : public Error {
   public:
    InterruptError();
};

class SystemError : public Error {
   public:
    SystemError(const Tstring& message, int lineNumber, Node* nodeArg);
    Node* getNode();

   private:
    Node* node;
};

/* print an error */
std::ostream& operator<<(std::ostream& out, const Error& error);

#endif
