/*
 * This file will contain a map of the comparison nodes (==, >, <, etc) and map them to functions used to execute them
 * Presently, the methods that cause errors for certyain types wil be specialized to do nothing on the incompatible types
 * These operators are defined seperately from the others because they have a different signature (i.e. they return booleans)
 */

#include <cmath>
#include <string>
#include <map>
#include <iostream>
#include "frontend.hpp"

using std::string;


//offloaded negation methods

template<typename T>
bool negator(T a, T b) {
	return !a;
}

//The string negation will check for an empty string, which is as close to NULL as the language presently becomes
template<> 
bool negator<string>(string a, string b) {
	return a != "";
}


template <class T>
class operationList
{
	public:
		//constructor fills table with all relevant operators
		operationList() {
			//functionMap[NODE_] = &operationList<T>::;
			functionMap[NODE_NOT] = &operationList<T>::logNot;
			functionMap[NODE_LT] = &operationList<T>::lessThan;
			functionMap[NODE_LTE] = &operationList<T>::lessThanEq;
			functionMap[NODE_GT] = &operationList<T>::greaterThan;
			functionMap[NODE_GTE] = &operationList<T>::greaterThanEq;
			functionMap[NODE_EQ] = &operationList<T>::equal;
			functionMap[NODE_NEQ] = &operationList<T>::notEqual;

		}

		//looks up the appropriate option and executes it
		bool execute(NodeKind n, T a, T b) {
			return (this->*functionMap[n])(a,b);
		}

	private:

		std::map<NodeKind, bool (operationList<T>::*)(T,T)> functionMap;

		//Define all the operations:
		
		//for unary operators, the second argument will alwys be ignored, but exists so that the function signature matches
		//Since this does not work for strings, we will offload it to a specialized function
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
