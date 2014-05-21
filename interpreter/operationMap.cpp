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
	//constructor fills table with all relevant 
	operationList() {
		//functionMap[NODE_] = &operationList<T>::;
		try {
			functionMap[NODE_PLUS] = &operationList<T>::add;
		} catch(...) {
			//operation not supported
		}
		try {
			functionMap[NODE_MINUS] = &operationList<T>::subtract;
		} catch(...) {
			//operation not supported
		}
		try {
			functionMap[NODE_TIMES] = &operationList<T>::multiply;
		} catch(...) {
			//operation not supported
		}
		try {
			functionMap[NODE_DIVIDE] = &operationList<T>::divide;
		} catch(...) {
			//operation not supported
		}
		try {
			functionMap[NODE_MODULUS] = &operationList<T>::modulus;
		} catch(...) {
			//operation not supported
		}
		try {
	//		functionMap[NODE_EXP] = &operationList<T>::exponential;
		} catch(...) {
			//operation not supported
		}
		try {
			//functionMap[NODE_NOT] = &operationList<T>::logNot;
			functionMap[NODE_BITNOT] = &operationList<T>::bitNot;
		} catch(...) {
			//operation not supported
		}
		try {
			functionMap[NODE_OR] = &operationList<T>::logOr;
		} catch(...) {
			//operation not supported
		}
		try {
			functionMap[NODE_AND] = &operationList<T>::logAnd;
		} catch(...) {
			//operation not supported
		}
		try {
			//functionMap[NODE_LT] = &operationList<T>::lessThan;
			//functionMap[NODE_LTE] = &operationList<T>::lessThanEq;
			//functionMap[NODE_GT] = &operationList<T>::greaterThan;
			//functionMap[NODE_GTE] = &operationList<T>::greaterThanEq;
			//functionMap[NODE_EQ] = &operationList<T>::equal;
			//functionMap[NODE_NEQ] = &operationList<T>::notEqual;
			functionMap[NODE_BITXOR] = &operationList<T>::bitXOr;
		} catch(...) {
			//operation not supported
		}
		try {
			functionMap[NODE_BITAND] = &operationList<T>::bitAnd;
		} catch(...) {
			//operation not supported
		}
		try {
			functionMap[NODE_BITOR] = &operationList<T>::bitOr;
		} catch(...) {
			//operation not supported
		}
		try {
			functionMap[NODE_SHIFTL] = &operationList<T>::shiftLeft;
		} catch(...) {
			//operation not supported
		}
		try {
			functionMap[NODE_SHIFTR] = &operationList<T>::shiftRight;
		} catch(...) {
			//operation not supported
		}

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
/*
	//This one is somewhat wierd, but ideally this will only be valid on int and real types for now, for which the cast should bring no issues. This has no problem with bools either, a specialization for strings is below
	T exponential(T a, T b) {
		return static_cast<T>(pow(static_cast<double>(a), static_cast<double>(b)));
	}

	//String specialization for exponential function
	//Obviously, it makes no sense to exponentiate string functions, so we'll just return null
	template <>
	string operationList<string>::exponential(string a, string b) {
		return NULL;
	}
*/
	//offload exponenitation to templated function
	T exponential(T a, T b) {
		return calaculateExponential(a, b);
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

	//Second argument is resent to match the signatures of other methods. In practice, the second argument will be ignored
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

//overloaded calculate exoponential

template<typename T>
T calculateExponential(T a, T b) {
	return pow(static_cast<double>(a), static_cast<double>(b));
}

string calculateExponential(string a, string b) {
	return NULL;
}

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

int main(){

	operationList<int>* x = new operationList<int>();
	operationList<string>* y = new operationList<string>();

 	cout << x->execute(NODE_PLUS,1,2) << endl;
	cout << y->execute(NODE_PLUS,"Hello ","world") << endl;
	

//	cout << x->add(1,2);
//	cout << x->exponential(2,1) << endl;
		
	return 0; 
}
