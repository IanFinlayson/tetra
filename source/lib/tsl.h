/* tsl.h
 * declares the tetra standard library functions */

#ifndef STDLIB_H
#define STDLIB_H

class Node;
class Context;

/* the print function prints any arguments which it is given */
Data* tslPrint(Node* args, Context* context, unsigned int threadid);

/* the input function reads a string from the user, with an optional prompt */
Data* tslInput(Node* args, Context* context, unsigned int threadid);

/* a void function to sleep for some real number of seconds */
Data* tslSleep(Node* args, Context* context, unsigned int threadid);

/* the type conversion functions */
Data* tslInt(Node* args, Context* context, unsigned int threadid);
Data* tslReal(Node* args, Context* context, unsigned int threadid);
Data* tslString(Node* args, Context* context, unsigned int threadid);
Data* tslBool(Node* args, Context* context, unsigned int threadid);

/* the len function returns the size of lists, dicts or strings */
Data* tslLen(Node* args, Context* context, unsigned int threadid);

#endif
