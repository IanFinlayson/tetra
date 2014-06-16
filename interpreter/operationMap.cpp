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
//By default, all "improper" operations will return null, since any other behavior would be non-intuitive for now.

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

template<typename T>
T calculateExponential(T a, T b) {
	return pow(static_cast<double>(a), static_cast<double>(b));
}
string calculateExponential(string a, string b) {
	return "";
}
TArray calculateExponential(TArray& a, TArray& b) {
	return a;
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
			//functionMap[NODE_NOT] = &OperationList<T>::logNot;
			functionMap[NODE_BITNOT] = &OperationList<T>::bitNot;
			functionMap[NODE_OR] = &OperationList<T>::logOr;
			functionMap[NODE_AND] = &OperationList<T>::logAnd;
			//functionMap[NODE_LT] = &OperationList<T>::lessThan;
			//functionMap[NODE_LTE] = &OperationList<T>::lessThanEq;
			//functionMap[NODE_GT] = &OperationList<T>::greaterThan;
			//functionMap[NODE_GTE] = &OperationList<T>::greaterThanEq;
			//functionMap[NODE_EQ] = &OperationList<T>::equal;
			//functionMap[NODE_NEQ] = &OperationList<T>::notEqual;
			functionMap[NODE_BITXOR] = &OperationList<T>::bitXOr;
			functionMap[NODE_BITAND] = &OperationList<T>::bitAnd;
			functionMap[NODE_BITOR] = &OperationList<T>::bitOr;
			functionMap[NODE_SHIFTL] = &OperationList<T>::shiftLeft;
			functionMap[NODE_SHIFTR] = &OperationList<T>::shiftRight;
		}

		//looks up the appropriate option and executes it
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

		//offload exponenitation to templated function
		T exponential(T a, T b) {
			return calculateExponential(a, b);
		}

		//LOGical operators (Yes, these return their own types!
		T logOr(T a, T b) {
			return a || b;
		}

		T logAnd(T a, T b) {
			return a && b;
		}

		//bit operations
		//because these are not defined for doubles, we must specialize 
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


/*
//Stubs for double operators that otherwise might be invalid
//At the moment, these all return 0
double operator%(double a, double b) {
	return 0;
}
double operator^(double a, double b) {
	return 0;
}
double operator&(double a, double b) {
	return 0;
}
double operator|(double a, double b) {
	return 0;
}
double operator<<(double a, double b) {
	return 0;
}
double operator>>(double a, double b) {
	return 0;
}
*/
/*
   int main(){

   OperationList<int>* x = new OperationList<int>();
   OperationList<string>* y = new OperationList<string>();

   cout << x->execute(NODE_PLUS,1,2) << endl;
   cout << y->execute(NODE_PLUS,"Hello ","world") << endl;


	cout << x->execute(NODE_EXP,1,2) << endl;
	cout << (y->execute(NODE_EXP,"2","1") == "") << endl;

return 0; 
}*/
