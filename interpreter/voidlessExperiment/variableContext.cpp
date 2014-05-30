
/*
 * This class represents the variables present at various scopes within a tetra program
 */

#include<iostream>
#include<map>
#include<string>
#include"voidlessTData.cpp"

using std::string;

class VarTable {

public:
	
	VarTable() {
		//nothing needed at the moment
	}

	//Release allocated data
	~VarTable() {
		std::cout << "destructor called" << std::endl;

		std::map<string,TData<void*> >::iterator iter;
		for(iter = varMap.begin(); iter != varMap.end();iter++) {
			std::cout << "deleting: " << iter->first << std::endl;
			TData<void*> next = iter->second;
			switch(next.getPointedTo()) {
				case TYPE_INT:
					cout << "Had value: " << *static_cast<int*>(next.getData()) << endl;
					delete static_cast<int*>(next.getData());
				break;
				case TYPE_REAL:
					cout << "Had value: " << *static_cast<double*>(next.getData()) << endl;			
					delete static_cast<double*>(next.getData());
				break;
				case TYPE_BOOL:
					cout << "Had value: " << *static_cast<bool*>(next.getData()) << endl;			
			
					delete static_cast<bool*>(next.getData());
				break;
				case TYPE_STRING:
					cout << "Had value: " << *static_cast<string*>(next.getData()) << endl;			
			
					delete static_cast<string*>(next.getData());
				break;
				default:
					cout << "nothing deleted, possible mem leak" << endl;
				break;

			}
			varMap.erase(iter);
		}
	}


	//returns a reference to the storage location of the variable. The interpreter supplies the expected type. At present, this results in a memory leak until we do a little extra design
	template<typename T>
	T* lookupVar(string varName) {

		//check whether an entry exists for this variable
		if(varMap.find(varName) == varMap.end()) {
			//Althought the container would insert a default TData, we need to allocate memory for the TData to point to!
			//T released manually in destructor
			T* newData_ptr = new T;

			TData<void*> insertable(newData_ptr);
			insertable.setDeletableType<T>();

			varMap[varName] = insertable;
		}

		return static_cast<T*>(varMap[varName].getData());

	}

	//Test method (unnecessary for functionality)
/*	void insertTestValues() {
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
*/
private:

	std::map<string, TData<void*> > varMap;

};
/*
int main() {
	VarTable x;
	x.insertTestValues();
}*/
