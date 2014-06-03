/*
 * This file will contain a map of the comparison nodes (==, >, <, etc) and map them to functions used to execute them
 * Presently, the methods that cause errors for certyain types wil be specialized to do nothing on the incompatible types
 * These operators are defined seperately from the others because they have a different signature (i.e. they return booleans)
 */

#include <cmath>
#include <string>
#include <map>
#include <iostream>
#include "tData.cpp"
#include "frontend.hpp"

using std::string;


//offloaded negation methods

template<typename T>
bool negator(T a) {
	return !a;
}

//The string negation will check for an empty string, which is as close to NULL as the language presently becomes
template<> 
bool negator<string>(string a) {
	return a != "";
}


template <class T>
class ComparisonList
{
	public:
		//constructor fills table with all relevant operators
		ComparisonList() {
			//functionMap[NODE_] = &ComparisonList<T>::;
			functionMap[NODE_NOT] = &ComparisonList<T>::logNot;
			functionMap[NODE_LT] = &ComparisonList<T>::lessThan;
			functionMap[NODE_LTE] = &ComparisonList<T>::lessThanEq;
			functionMap[NODE_GT] = &ComparisonList<T>::greaterThan;
			functionMap[NODE_GTE] = &ComparisonList<T>::greaterThanEq;
			functionMap[NODE_EQ] = &ComparisonList<T>::equal;
			functionMap[NODE_NEQ] = &ComparisonList<T>::notEqual;

		}

		//looks up the appropriate option and executes it
		bool execute(NodeKind n, TData<T>* a, TData<T>* b) {
			return (this->*functionMap[n])(a->getData(),b->getData());
		}

	private:

		std::map<NodeKind, bool (ComparisonList<T>::*)(T,T)> functionMap;

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
/*
int main() {
	ComparisonList<int> opInt;
	ComparisonList<bool> opBool;
	ComparisonList<string> opString;
	ComparisonList<double> opReal;

	std::cout << opInt.execute(NODE_LT,2,2) << std::endl;
	std::cout << opBool.execute(NODE_GTE, true, true) << std::endl;
	std::cout << opString.execute(NODE_EQ, "lady","ldy") << std::endl;
	std::cout << opReal.execute(NODE_NEQ,12,12) << std::endl;
}*/
