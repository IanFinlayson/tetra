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
    Error(const tstring& mesg, int lineno = 0);
    tstring getMessage() const;
    int getLine() const;

   private:
    tstring mesg;
    int lineno;
};

class RuntimeError : public Error {
   public:
    RuntimeError(const tstring& pMessage, int pLine);
};

class InterruptError : public Error {
   public:
    InterruptError();
};

class SystemError : public Error {
   public:
    SystemError(const tstring& pMessage, int pLine, const Node* pNode);
    const Node* getNode();

   private:
    const Node* node;
};

/* print an error */
std::ostream& operator<<(std::ostream& out, const Error& error);

#endif
