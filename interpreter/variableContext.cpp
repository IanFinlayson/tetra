/*
 * This class represents the variables present at various scopes within a tetra program
 */

#include<iostream>
#include<map>
#include<string>


using namespace std;

class varTable {

public:
	
	varTable() {
		//nothing needed at the moment
	}

	~varTable() {
		//Must delete everything allocated for the map (coming soon)!
	}


	//returns a reference to the storage location of the variable. The interpreter supplies the expected type.
	template<typename T>
	T* lookupVar(string varName) {

		T* ret;

		//check whether an entry exists for this variable
		if(varMap.find(varName) == varMap.end()) {
			//Althought the container would insert a default void*, we need to allocate memory for the void* to point to!

			ret = new T;
		}

		return static_cast<T*>(varMap[varName]);

	}

private:

	map<string, void*> varMap;

};

