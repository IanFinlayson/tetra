/*
 * This file will contain a map of the comparison nodes (==, >, <, etc) and map them to functions used to execute them
 * Presently, the methods that cause errors for certyain types wil be specialized to do nothing on the incompatible types
 * These operators are defined seperately from the others because they have a different signature (i.e. they return booleans)
 */

#include <cmath>
#include <string>
#include <map>
#include <iostream>
#include "tData.h"
#include "frontend.hpp"
#include "tArray.h"

using std::string;


//Offloaded negation methods
//For all types except for which a specialization is declared, classic negation will do
template<typename T>
bool negator(T a) {
	return !a;
}

//The string negation will check for an empty string, which is as close to NULL as the language presently becomes
template<> 
bool negator<string>(string a) {
	return a != "";
}

//Specialization for bitwise array negation for now will just return null
template<>
bool negator<TArray>(TArray a) {
	return false;
}

template <class T>
class ComparisonList
{
	public:
		//constructor fills table with all relevant operators (defined by functions)
		ComparisonList() {
			//functionMap[NODE_] = &ComparisonList<T>::;
			compMap[NODE_NOT] = &ComparisonList<T>::logNot;
			compMap[NODE_LT] = &ComparisonList<T>::lessThan;
			compMap[NODE_LTE] = &ComparisonList<T>::lessThanEq;
			compMap[NODE_GT] = &ComparisonList<T>::greaterThan;
			compMap[NODE_GTE] = &ComparisonList<T>::greaterThanEq;
			compMap[NODE_EQ] = &ComparisonList<T>::equal;
			compMap[NODE_NEQ] = &ComparisonList<T>::notEqual;

		}

		//looks up the appropriate operator according to the given NODE_KIND and executes it
		bool execute(NodeKind n, TData<T>& a, TData<T>& b) {
			return (this->*functionMap[n])(a.getData(),b.getData());
		}

		bool execute(NodeKind n, Node* a, Node* b, TetraContext& context) {
			return (this->*compMap[n])(a,b,context);
		}
	
	private:

		std::map<NodeKind, bool (ComparisonList<T>::*)(T,T)> functionMap;
		std::map<NodeKind, bool (ComparisonList<T>::*)(Node*,Node*,TetraContext&)> compMap;

		//Define all the operations in function form, so they can be dynamically called:
		
		bool lessThan(Node* a, Node* b, TetraContext& context) {
			TData<T> op1;
			evaluateNode<T>(a,op1,context);
			TData<T> op2;
			evaluateNode<T>(b,op2,context);
			return op1.getData() < op2.getData();
		}

		bool lessThanEq(Node* a, Node* b, TetraContext& context) {
			TData<T> op1;
			evaluateNode<T>(a,op1,context);
			TData<T> op2;
			evaluateNode<T>(b,op2,context);
			return op1.getData() <= op2.getData();
		}
		
		bool greaterThan(Node* a, Node* b, TetraContext& context) {
			TData<T> op1;
			evaluateNode<T>(a,op1,context);
			TData<T> op2;
			evaluateNode<T>(b,op2,context);
			return op1.getData() > op2.getData();
		}

		bool greaterThanEq(Node* a, Node* b, TetraContext& context) {
			TData<T> op1;
			evaluateNode<T>(a,op1,context);
			TData<T> op2;
			evaluateNode<T>(b,op2,context);
			return op1.getData() >= op2.getData();
		}
		
		bool equal(Node* a, Node* b, TetraContext& context) {
			TData<T> op1;
			evaluateNode<T>(a,op1,context);
			TData<T> op2;
			evaluateNode<T>(b,op2,context);
			return op1.getData() == op2.getData();
		}
		
		bool notEqual(Node* a, Node* b, TetraContext& context) {
			TData<T> op1;
			evaluateNode<T>(a,op1,context);
			TData<T> op2;
			evaluateNode<T>(b,op2,context);
			return op1.getData() != op2.getData();
		}		
		//for unary operators, the second argument will alwys be ignored, but exists so that the function signature matches
		//Since negaiton does not work for strings or arrays, we will offload it to a specialized function
		
		bool logNot(Node* a, Node* b, TetraContext& context) {
			TData<T> op1;
			evaluateNode<T>(a,op1,context);
			return negator(op1.getData());
		}
		bool logNot(T a, T b) {
			return negator(a);
		}

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
};
