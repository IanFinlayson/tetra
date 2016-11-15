/* tsl.h
 * declares the tetra standard library functions */

#ifndef STDLIB_H
#define STDLIB_H

class Node;
class Context;

/* the print function prints any arguments which it is given */
Data* tslPrint(Node* args, Context* context);

/* the input function reads a string from the user, with an optional prompt */
Data* tslInput(Node* args, Context* context);

/* the type conversion functions */
Data* tslInt(Node* args, Context* context);
Data* tslReal(Node* args, Context* context);
Data* tslString(Node* args, Context* context);
Data* tslBool(Node* args, Context* context);

/* the len function returns the size of lists, dicts or strings */
Data* tslLen(Node* args, Context* context);

#endif
