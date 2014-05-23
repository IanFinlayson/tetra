/*
 * This file will contain a map of the operational nodes (+, %, &, etc) and map them to functions used to execute them
 * Presently, the methods that cause errors for certyain types wil be specialized to do nothing on the incompatible types
 * The comparison operators, since they have a different signature than these operators, are defined in seperately
 */

#include <cmath>
#include <string>
#include <map>
#include <iostream>
#include "frontend.hpp"

using std::string;


//overloaded calculate exoponential

template<typename T>
T calculateExponential(T a, T b) {
	return pow(static_cast<double>(a), static_cast<double>(b));
}

string calculateExponential(string a, string b) {
	return "";
}


template <class T>
class operationList
{
	public:
		//constructor fills table with all relevant operators
		operationList() {
			//functionMap[NODE_] = &operationList<T>::;
			functionMap[NODE_PLUS] = &operationList<T>::add;
			functionMap[NODE_MINUS] = &operationList<T>::subtract;
			functionMap[NODE_TIMES] = &operationList<T>::multiply;
			functionMap[NODE_DIVIDE] = &operationList<T>::divide;
			functionMap[NODE_MODULUS] = &operationList<T>::modulus;
			functionMap[NODE_EXP] = &operationList<T>::exponential;
			//functionMap[NODE_NOT] = &operationList<T>::logNot;
			functionMap[NODE_BITNOT] = &operationList<T>::bitNot;
			functionMap[NODE_OR] = &operationList<T>::logOr;
			functionMap[NODE_AND] = &operationList<T>::logAnd;
			//functionMap[NODE_LT] = &operationList<T>::lessThan;
			//functionMap[NODE_LTE] = &operationList<T>::lessThanEq;
			//functionMap[NODE_GT] = &operationList<T>::greaterThan;
			//functionMap[NODE_GTE] = &operationList<T>::greaterThanEq;
			//functionMap[NODE_EQ] = &operationList<T>::equal;
			//functionMap[NODE_NEQ] = &operationList<T>::notEqual;
			functionMap[NODE_BITXOR] = &operationList<T>::bitXOr;
			functionMap[NODE_BITAND] = &operationList<T>::bitAnd;
			functionMap[NODE_BITOR] = &operationList<T>::bitOr;
			functionMap[NODE_SHIFTL] = &operationList<T>::shiftLeft;
			functionMap[NODE_SHIFTR] = &operationList<T>::shiftRight;
		}

		//looks up the appropriate option and executes it
		T execute(NodeKind n, T a, T b) {
			return (this->*functionMap[n])(a,b);
		}

	private:

		std::map<NodeKind, T (operationList<T>::*)(T,T)> functionMap;

		//Define all the operations:
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

		//offload exponenitation to templated function
		T exponential(T a, T b) {
			return calculateExponential(a, b);
		}

		//LOGical operators
		T logOr(T a, T b) {
			return a || b;
		}

		T logAnd(T a, T b) {
			return a && b;
		}

		//for unary operators, a second formal parameter is needed to match the signiture of the other functions. In practice, NULL will be passed as the socnd argument and it will be ignored
		T logNot(T a, T b) {
			return !a;
		}
/*
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
*/
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

		//Second argument is present to match the signatures of other methods. In practice, the second argument will be ignored
		T bitNot(T a, T b) {
			return !a;
		}

		T shiftLeft(T a, T b) {
			return a << b;
		}

		T shiftRight(T a, T b) {
			return a >> b;
		}

};

//These operators are stubbed so that the operations exist, and can be modified later. also prevents us from having to make too many special cases for strings.
//By default, all "improper" operations will return null, since any other behavior would be non-intuitive for now.

string operator/(string a, string b) {
	return NULL;
}

string operator*(string a, string b) {
	return NULL;
}
string operator-(string a, string b) {
	return NULL;
}
string operator^(string a, string b) {
	return NULL;
}
string operator||(string a, string b) {
	return NULL;
}
string operator&&(string a, string b) {
	return NULL;
}
string operator|(string a, string b) {
	return NULL;
}
string operator&(string a, string b) {
	return NULL;
}
string operator<<(string a, string b) {
	return NULL;
}
string operator>>(string a, string b) {
	return NULL;
}
string operator!(string a) {
	return NULL;
}
string operator%(string a, string b) {
	return NULL;
}
/*
   int main(){

   operationList<int>* x = new operationList<int>();
   operationList<string>* y = new operationList<string>();

   cout << x->execute(NODE_PLUS,1,2) << endl;
   cout << y->execute(NODE_PLUS,"Hello ","world") << endl;


	cout << x->execute(NODE_EXP,1,2) << endl;
	cout << (y->execute(NODE_EXP,"2","1") == "") << endl;

return 0; 
}*/
