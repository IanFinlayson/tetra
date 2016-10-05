/* error.h
 * this file declares the different error types */

#ifndef ERROR_H
#define ERROR_H

#include <map>
#include <string>
#include <vector>

#include "types.h"
#include "strings.h"

/* any type of error is handled with this exception */
class Error {
   public:
    Error(const String& mesg, int lineNumber = 0);
    String getMessage() const;
    int getLine() const;

   private:
    String mesg;
    int lineNumber;
};

class RuntimeError : public Error {
   public:
    RuntimeError(const String& message, int lineNumber);
};

class InterruptError : public Error {
   public:
    InterruptError();
};

class SystemError : public Error {
   public:
    SystemError(const String& message, int lineNumber, Node* nodeArg);
    Node* getNode();

   private:
    Node* node;
};

/* print an error */
std::ostream& operator<<(std::ostream& out, const Error& error);

#endif
