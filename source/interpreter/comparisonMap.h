/*
 * This file will contain a map of the comparison nodes (==, >, <, etc) and map
 * them to functions used to execute them
 * Presently, the methods that cause errors for certyain types wil be
 * specialized to do nothing on the incompatible types
 * These operators are defined seperately from the others because they have a
 * different signature (i.e. they return booleans)
 */

#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include "backend.h"
#include "frontend.h"
using std::string;

template <typename T>
void evaluateNode(const Node*, TData<T>&, TetraContext&);

// Offloaded negation methods
// For all types except for which a specialization is declared, classic negation
// will do
template <typename T>
bool negator(T a) {
  return !a;
}

// The string negation will check for an empty string, which is as close to NULL
// as the language presently becomes
template <>
bool negator<string>(string a) {
  return a != "";
}

// Specialization for bitwise array negation for now will just return null
template <>
bool negator<TArray>(TArray a) {
  UNUSED(a)
  return false;
}

template <class T>
class ComparisonList {
 public:
  // constructor fills table with all relevant operators (defined by functions)
  ComparisonList() {
    // functionMap[NODE_] = &ComparisonList<T>::;
    compMap[NODE_NOT] = &ComparisonList<T>::logNot;
    compMap[NODE_LT] = &ComparisonList<T>::lessThan;
    compMap[NODE_LTE] = &ComparisonList<T>::lessThanEq;
    compMap[NODE_GT] = &ComparisonList<T>::greaterThan;
    compMap[NODE_GTE] = &ComparisonList<T>::greaterThanEq;
    compMap[NODE_EQ] = &ComparisonList<T>::equal;
    compMap[NODE_NEQ] = &ComparisonList<T>::notEqual;
  }

  // looks up the appropriate operator according to the given NODE_KIND and
  // executes it
  bool execute(const NodeKind n, TData<T>& a, TData<T>& b) {
    // This method violates the short-circuit policy of the language
    assert(false);
    return (this->*functionMap[n])(a.getData(), b.getData());
  }

  bool execute(NodeKind n, const Node* a, const Node* b,
               TetraContext& context) {
    return (this->*compMap[n])(a, b, context);
  }

 private:
  std::map<NodeKind, bool (ComparisonList<T>::*)(T, T)> functionMap;
  std::map<NodeKind,
           bool (ComparisonList<T>::*)(const Node*, const Node*, TetraContext&)>
      compMap;

  // Define all the operations in function form, so they can be dynamically
  // called:

  bool lessThan(const Node* a, const Node* b, TetraContext& context) {
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return op1.getData() < op2.getData();
  }

  bool lessThanEq(const Node* a, const Node* b, TetraContext& context) {
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return op1.getData() <= op2.getData();
  }

  bool greaterThan(const Node* a, const Node* b, TetraContext& context) {
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return op1.getData() > op2.getData();
  }

  bool greaterThanEq(const Node* a, const Node* b, TetraContext& context) {
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return op1.getData() >= op2.getData();
  }

  bool equal(const Node* a, const Node* b, TetraContext& context) {
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return op1.getData() == op2.getData();
  }

  bool notEqual(const Node* a, const Node* b, TetraContext& context) {
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    TData<T> op2;
    evaluateNode<T>(b, op2, context);
    return op1.getData() != op2.getData();
  }
  // for unary operators, the second argument will alwys be ignored, but exists
  // so that the function signature matches
  // Since negaiton does not work for strings or arrays, we will offload it to a
  // specialized function

  bool logNot(const Node* a, const Node* b, TetraContext& context) {
    UNUSED(b)
    TData<T> op1;
    evaluateNode<T>(a, op1, context);
    return negator(op1.getData());
  }
  bool logNot(T a, T b) {
    UNUSED(b)
    return negator(a);
  }

  bool lessThan(T a, T b) { return a < b; }

  bool lessThanEq(T a, T b) { return a <= b; }

  bool greaterThan(T a, T b) { return a > b; }

  bool greaterThanEq(T a, T b) { return a >= b; }

  bool equal(T a, T b) { return a == b; }

  bool notEqual(T a, T b) { return a != b; }
};
