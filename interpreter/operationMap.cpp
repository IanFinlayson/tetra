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
#include "tData.h"
#include "tArray.h"

using std::string;


//These operators are stubbed so that the operations exist, and can be modified later. also prevents us from having to make too many special cases for strings.
//By default, all "improper" operations will return a blank string, since any other behavior would be non-intuitive for now.
string operator/(string a, string b) {
	return "";
}

string operator*(string a, string b) {
	return "";
}
string operator-(string a, string b) {
	return "";
}
string operator^(string a, string b) {
	return "";
}
string operator||(string a, string b) {
	return "";
}
string operator&&(string a, string b) {
	return "";
}
string operator|(string a, string b) {
	return "";
}
string operator&(string a, string b) {
	return "";
}
string operator<<(string a, string b) {
	return "";
}
string operator>>(string a, string b) {
	return "";
}
string operator!(string a) {
	return "";
}
string operator%(string a, string b) {
	return "";
}

//overloaded calculate exoponential
//Types that can be cast to doubles get cast to doubles
//Other types return stubbed values
template<typename T>
T calculateExponential(T a, T b) {
	return pow(static_cast<double>(a), static_cast<double>(b));
}
string calculateExponential(string a, string b) {
	return "";
}
TArray calculateExponential(TArray& a, TArray& b) {
	//return default TArray
	TArray x;
	return x;
}

//overloaded funcitons for bitwise operators (to make the compiler happy that our templated class might allow for 1.1 << 2.2, etc
template<typename T>
T bitLeftShift(T a, T b) {
	return a << b;
}
double bitLeftShift(double a, double b) {
	return 0;
}

template<typename T>
T bitRightShift(T a, T b) {
	return a >> b;
}
double bitRightShift(double a, double b) {
	return 0;
}

template<typename T>
T bitwiseOr(T a, T b) {
	return a | b;
}
double bitwiseOr(double a, double b) {
	return 0;
}

template<typename T>
T bitwiseAnd(T a, T b) {
	return a & b;
}
double bitwiseAnd(double a, double b) {
	return 0;
}

template<typename T>
T bitwiseXOr(T a, T b) {
	return a ^ b;
}
double bitwiseXOr(double a, double b) {
	return 0;
}

template<typename T>
T remainderDivision(T a, T b) {
	return a % b;
}
double remainderDivision(double a, double b) {
	return 0;
}

template <class T>
class OperationList
{
	public:
		//constructor fills table with all relevant operators
		OperationList() {
			//functionMap[NODE_] = &OperationList<T>::;
			functionMap[NODE_PLUS] = &OperationList<T>::add;
			functionMap[NODE_MINUS] = &OperationList<T>::subtract;
			functionMap[NODE_TIMES] = &OperationList<T>::multiply;
			functionMap[NODE_DIVIDE] = &OperationList<T>::divide;
			functionMap[NODE_MODULUS] = &OperationList<T>::modulus;
			functionMap[NODE_EXP] = &OperationList<T>::exponential;
			functionMap[NODE_BITNOT] = &OperationList<T>::bitNot;
			functionMap[NODE_OR] = &OperationList<T>::logOr;
			functionMap[NODE_AND] = &OperationList<T>::logAnd;
			functionMap[NODE_BITXOR] = &OperationList<T>::bitXOr;
			functionMap[NODE_BITAND] = &OperationList<T>::bitAnd;
			functionMap[NODE_BITOR] = &OperationList<T>::bitOr;
			functionMap[NODE_SHIFTL] = &OperationList<T>::shiftLeft;
			functionMap[NODE_SHIFTR] = &OperationList<T>::shiftRight;
		}

		//looks up the appropriate operation (via node's NodeKind) and executes it
		T execute(NodeKind n, TData<T>& a, TData<T>& b) {
			return (this->*functionMap[n])(a.getData(),b.getData());
		}

	private:

		std::map<NodeKind, T (OperationList<T>::*)(T,T)> functionMap;

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
			return remainderDivision(a,b);
		}

		//offload exponenitation to templated function, as it only makes sense for certain types
		T exponential(T a, T b) {
			return calculateExponential(a, b);
		}

		//LOGical operators (Yes, these return their own types!)
		T logOr(T a, T b) {
			return a || b;
		}

		T logAnd(T a, T b) {
			return a && b;
		}

		//bit operations
		//because these are not defined for doubles, we must offload these methods so they can be specialized for non-supported types 
		T bitXOr(T a, T b) {
			return bitwiseXOr(a,b);
		}

		T bitAnd(T a, T b) {
			return bitwiseAnd(a,b);
		}

		T bitOr(T a, T b) {
			return bitwiseOr(a,b);
		}

		//Second argument is present to match the signatures of other methods. In practice, the second argument will be ignored
		T bitNot(T a, T b) {
			return !a;
		}

		T shiftLeft(T a, T b) {
			return bitLeftShift(a,b);
		}

		T shiftRight(T a, T b) {
			return bitRightShift(a,b);
		}

};
