/*
 * This file will contain a map of the operational nodes (+, %, &, etc) and map them to functions used to execute them
 * for now, it is templated so we might use multiple data types. This relies on the assumption that the parser catches illegal operations before we try them here (like "a" * "b" for strings)
 */

#include <cmath>
#include <string>
#include <map>
#include <iostream>
#include "frontend.hpp"

using namespace std;

template <class T>
class operationList
{
public:
	operationList() {
		//functionMap[NODE_] = &operationList<T>::;
		functionMap[NODE_PLUS] = &(operationList<T>::add);
	}

	T execute(NodeKind n, T a, T b) {
		return (this->*functionMap[n])(a,b);
	}

private:
		
	map<NodeKind, T (operationList<T>::*)(T,T)> functionMap;

	T add(T a, T b) {
		return a + b;
	}

	T subtract(T a, T b) {
		return a - b;
	}

	T multiply(T a, T b) {
		return a * b;
	}

	T divide(T a, T b) {
		return a / b;
	}

	T modulus(T a, T b) {
		return a % b;
	}

	//This one is somewhat wierd, but ideally this will only be valid on int and real types for now, for which the cast should bring no issues
	T exponential(T a, T b) {
		return static_cast<T>(pow(static_cast<double>(a), static_cast<double>(b)));
	}

	//LOGical operators
	T logOr(T a, T b) {
		return a || b;
	}

	T logAnd(T a, T b) {
		return a && b;
	}

	T logNot(T a) {
		return !a;
	}

	//These ones will always return booleans, regardless of the types involved
	
	bool lessThan(T a, T b) {
		return a < b;
	}

	bool lessThanEq(T a, T b) {
		return a <= b;
	}

	bool greaterThan(T a, T b) {
		return a > b;
	}

	bool greaterThanEq(T a, T b) {
		return a >= b;
	}

	bool equal(T a, T b) {
		return a == b;
	}

	bool notEqual(T a, T b) {
		return a != b;
	}

	//bit operations
	
	T bitXOr(T a, T b) {
		return a ^ b;
	}

	T bitAnd(T a, T b) {
		return a & b;
	}

	T bitOr(T a, T b) {
		return a | b;
	}

	T bitNot(T a) {
		return !a;
	}

	T shiftLeft(T a, T b) {
		return a << b;
	}

	T shiftRight(T a, T b) {
		return a >> b;
	}

};

int main(){

	operationList<int>* x = new operationList<int>();
	operationList<string>* y = new operationList<string>();

 	cout << x->execute(NODE_PLUS,1,2) << endl;
	cout << y->execute(NODE_PLUS,"Hello ","world") << endl;
	

//	cout << x->add(1,2);
//	cout << x->exponential(2,1) << endl;
		
	return 0; 
}
