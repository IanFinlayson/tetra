/*
 * This class represents the variables present at various scopes within a tetra program
 */

#include<iostream>
#include<map>
#include<string>

using std::string;

class VarTable {

public:
	
	VarTable() {
		//nothing needed at the moment
	}

	//still need to complete
	~VarTable() {
		std::cout << "destructor called" << std::endl;

		std::map<string,void*>::iterator iter;
		for(iter = varMap.begin(); iter != varMap.end(); iter++) {
			std::cout << iter->first << std::endl;
			std::cout << iter-> second << std::endl;
			//delete iter->second;
			//varMap.erase(iter);
		}
	}


	//returns a reference to the storage location of the variable. The interpreter supplies the expected type.
	template<typename T>
	T* lookupVar(string varName) {

		T* ret;

		//check whether an entry exists for this variable
		if(varMap.find(varName) == varMap.end()) {
			//Althought the container would insert a default void*, we need to allocate memory for the void* to point to!

			ret = new T;
			varMap[varName] = ret;
		}

		return static_cast<T*>(varMap[varName]);

	}

	//Test method (unnecessary for functionality)
	void insertTestValues() {
		string z = "x";
		double* x = lookupVar<double>(z);
		bool* y = lookupVar<bool>("y");
		*x = 42.4;
		*y = true;

		double* a = lookupVar<double>(z);
		bool* b = lookupVar<bool>("y");

		std::cout << *a << std::endl;
		std::cout << *b << std::endl;

		*lookupVar<double>(z) = 26.2;
		std::cout << *a << std::endl;
	}

private:

	std::map<string, void*> varMap;

};
/*
int main() {
	VarTable x;
	x.insertTestValues();
}*/
