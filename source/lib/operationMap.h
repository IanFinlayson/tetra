/*
 * This file will contain a map of the operational nodes (+, %, &, etc) and map
 * them to functions used to execute them
 * Presently, the methods that cause errors for certyain types wil be
 * specialized to do nothing on the incompatible types
 * The comparison operators, since they have a different signature than these
 * operators, are defined in seperately
 */

#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include "backend.h"
#include "frontend.h"

using std::string;

// External function declaration and required args
class TetraContext;

template <typename T>
void evaluateNode(const Node*, TData<T>&, TetraContext&);

// These operators are stubbed so that the operations exist, and can be modified
// later. also prevents us from having to make too many special cases for
// strings.  By default, all "improper" operations will return a blank string,
// since any other behavior would be non-intuitive for now.
tstring operator/(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}

tstring operator*(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}
tstring operator-(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}
tstring operator^(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}
tstring operator||(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}
tstring operator&&(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}
tstring operator|(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}
tstring operator&(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}
tstring operator<<(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}
tstring operator>>(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}
tstring operator!(tstring a) {
  UNUSED(a)
  return "";
}
tstring operator%(tstring a, tstring b) {
  UNUSED(a)
  UNUSED(b)
  return "";
}

// overloaded calculate exoponential
// Types that can be cast to doubles get cast to doubles
// Other types return stubbed values
template <typename T>
T calculateExponential(T a, T b) {
  assert(false);
  UNUSED(a)
  UNUSED(b)
  return T();
}
template <>
tint calculateExponential<tint>(tint a, tint b) {
  return a.pow(b);
}
template <>
double calculateExponential<double>(double a, double b) {
  return std::pow(a,b);
}

// overloaded funcitons for bitwise operators (to make the compiler happy that
// our templated class might allow for 1.1 << 2.2, etc
template <typename T>
T bitLeftShift(T a, T b) {
  assert(false);
  UNUSED(a)
  UNUSED(b)
  return T();
}
template <>
tint bitLeftShift<tint>(tint a, tint b) {
  return a << b;
}

template <typename T>
T bitRightShift(T a, T b) {
  assert(false);
  UNUSED(a)
  UNUSED(b)
  return T();
}
template <>
tint bitRightShift<tint>(tint a, tint b) {
  return a >> b;
}

template <typename T>
T bitwiseOr(T a, T b) {
  assert(false);
  UNUSED(a)
  UNUSED(b)
  return T();
}
template <>
tint bitwiseOr<tint>(tint a, tint b) {
  return a | b;
}

template <typename T>
T bitwiseAnd(T a, T b) {
  assert(false);
  UNUSED(a)
  UNUSED(b)
  return T();
}
template <>
tint bitwiseAnd<tint>(tint a, tint b) {
  return a & b;
}

template <typename T>
T bitwiseXOr(T a, T b) {
  assert(false);
  UNUSED(a)
  UNUSED(b)
  return T();
}
template <>
tint bitwiseXOr<tint>(tint a, tint b) {
  return a ^ b;
}

template <typename T>
T bitwiseNot(T a) {
  assert(false);
  UNUSED(a)
  return T();
}
template <>
tint bitwiseNot<tint>(tint a) {
  return ~a;
}

template <typename T>
T remainderDivision(T a, T b) {
  assert(false);
  UNUSED(a)
  UNUSED(b)
  return T();
}
template <>
tint remainderDivision<tint>(tint a, tint b) {
  return a % b;
}

template <typename T>
T logicalAnd(T a, Node* b, TetraContext& context) {
  assert(false);
  UNUSED(context)
  UNUSED(a)
  UNUSED(b)
  return T();
}
template <>
tbool logicalAnd<tbool>(tbool a, Node* b, TetraContext& context) {
  if (!a) {
    return false;
  } else {
    TData<tbool> op2;
    evaluateNode<tbool>(b, op2, context);
    return op2.getData();
  }
}

template <typename T>
T logicalOr(Node* a, Node* b, TetraContext& context) {
  assert(false);
  UNUSED(context)
  UNUSED(a)
  UNUSED(b)
  return T();
}
template <>
tbool logicalOr<tbool>(Node* a, Node* b, TetraContext& context) {
  TData<tbool> op1;
  evaluateNode<tbool>(a, op1, context);
  if (op1.getData()) {
    return true;
  } else {
    TData<tbool> op2;
    evaluateNode<tbool>(b, op2, context);
    return op2.getData();
  }
}

template <class T>
class OperationList {
 public:
  // constructor fills table with all relevant operators
  OperationList() {
    // functionMap[NODE_] = &OperationList<T>::;
    opMap[NODE_PLUS] = &OperationList<T>::add;
    opMap[NODE_MINUS] = &OperationList<T>::subtract;
    opMap[NODE_TIMES] = &OperationList<T>::multiply;
    opMap[NODE_DIVIDE] = &OperationList<T>::divide;
    opMap[NODE_MODULUS] = &OperationList<T>::modulus;
    opMap[NODE_EXP] = &OperationList<T>::exponential;
    opMap[NODE_BITNOT] = &OperationList<T>::bitNot;
    opMap[NODE_OR] = &OperationList<T>::logOr;
    opMap[NODE_AND] = &OperationList<T>::logAnd;
    opMap[NODE_BITXOR] = &OperationList<T>::bitXOr;
    opMap[NODE_BITAND] = &OperationList<T>::bitAnd;
    opMap[NODE_BITOR] = &OperationList<T>::bitOr;
    opMap[NODE_SHIFTL] = &OperationList<T>::shiftLeft;
    opMap[NODE_SHIFTR] = &OperationList<T>::shiftRight;
  }

  // looks up the appropriate operation (via node's NodeKind) and executes it
  T execute(NodeKind n, TData<T>& a, TData<T>& b) {
    return (this->*functionMap[n])(a.getData(), b.getData());
  }

  T execute(NodeKind n, Node* op1, Node* op2, TetraContext& context) {
    return (this->*opMap[n])(op1, op2, context);
  }

 private:
  std::map<NodeKind, T (OperationList<T>::*)(T, T), less<NodeKind>, gc_allocator<pair<NodeKind, T (OperationList<T>::*)(T, T)> > > functionMap;
  std::map<NodeKind, T (OperationList<T>::*)(Node*, Node*, TetraContext&),less<NodeKind>, gc_allocator<pair<NodeKind, T (OperationList<T>::*)(Node*, Node*, TetraContext&)> > >
      opMap;
  // Define all the operations:
  // Note that in cases where due consideration should be considered when
  // attempting to perform an operation (i.e. 1.2 << 4.8)
  // the interpreter defines basic (though not necessarily functional or
  // intuitive) behavior by specializing a function to call
  // Note also that in all cases (except && and || for bools, where we can use
  // short circuiting), the interpreter will evaluate both arguments regardless
  // of whether the operaiton itself makes any sense
  // That is, if foo() and bar() return strings, the hypothetical expression
  // foo() % bar() will execute both foo and bar.
  T add(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return op1.getData() + op2.getData();
  }

  T subtract(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return op1.getData() - op2.getData();
  }

  T multiply(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return op1.getData() * op2.getData();
  }

  T divide(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return op1.getData() / op2.getData();
  }

  T modulus(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return remainderDivision(op1.getData(), op2.getData());
  }

  T exponential(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return calculateExponential(op1.getData(), op2.getData());
  }
  // for and and or, the functionality may be offloaded to a boolean
  // specialization which allows for short circuiting
  T logOr(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    /*TData<T> op1;
      evaluateNode<T>(a,op1,context);
      TData<T> op2;
      evaluateNode<T>(b,op2,context);*/
    return logicalOr<T>(a, b, context);
  }

  T logAnd(Node* a, Node* b, TetraContext& context) {
    // return a + b;
      	TData<T> op1;
        evaluateNode<T>(a,op1,context);
       // TData<T> op2;
        //evaluateNode<T>(b,op2,context);
    return logicalAnd<T>(op1.getData(), b, context);
  }

  T bitXOr(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return bitwiseXOr(op1.getData(), op2.getData());
  }

  T bitAnd(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return bitwiseAnd(op1.getData(), op2.getData());
  }

  T bitOr(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return bitwiseOr(op1.getData(), op2.getData());
  }

  T bitNot(Node* a, Node* b, TetraContext& context) {
    UNUSED(b)
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    return bitwiseNot(op1.getData());
  }

  T shiftLeft(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return bitLeftShift(op1.getData(), op2.getData());
  }

  T shiftRight(Node* a, Node* b, TetraContext& context) {
    // return a + b;
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return bitRightShift(op1.getData(), op2.getData());
  }
};
