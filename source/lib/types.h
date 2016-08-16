/* data types used in the language */

#ifndef TYPE_H
#define TYPE_H

#include <cmath>

#include "node.h"

/* forward declare this since it's used for lots of things */
class tstring;

/* the different possibilities a data type can be */
enum DataTypeKind {
  TYPE_INT,
  TYPE_REAL,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_NONE,
  TYPE_VECTOR,
  TYPE_TUPLE,
  TYPE_DICT,
  TYPE_FUNCTION,
  TYPE_CLASS,
  TYPE_MUTEX,
  TYPE_TASK,
  TYPE_OVERLOAD
};

/* a data type contains the above enum, along with a pointer to the "sub" type
 * currently this is only used for vectors */
class DataType {
 public:
  DataType(DataTypeKind kind);
  DataType(const DataType& other);
  ~DataType();
  DataTypeKind getKind() const;
  std::vector<DataType>* subtypes;
  tstring* className;
  DataType operator=(const DataType& other);
  bool isEmptyContainerType() const;

 private:
  DataTypeKind kind;
};

/* operators for comparing types for equality */
bool operator==(const DataType& lhs, const DataType& rhs);
bool operator!=(const DataType& lhs, const DataType& rhs);

/* function which returns a string representation of a data type */
tstring typeToString(DataType* t);
void inferParams(Node* node, Node* func = NULL);
void buildParamTupleType(DataType* type, const Node* node);
void initTypes(Node* node);
void inferTypes(Node* node);

#endif


