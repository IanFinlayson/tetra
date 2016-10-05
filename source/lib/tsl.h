/* tsl.h
 * declares the tetra standard library functions */

#ifndef STDLIB_H
#define STDLIB_H

class Node;
class Context;

/* the print function prints any arguments which it is given */
Data* tslPrint(Node* args, Context* context);

#endif
