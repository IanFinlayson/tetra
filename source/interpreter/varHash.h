//This class is a bare bones hash table for variable lookup
/*
#include<string>
#include<iostream>

class VarHash{
private:
	typedef std::pair<std::string, void*> keyVal;
	//std::pair<std::string, void*>* table;
	keyVal* table;
	int hashSize;

	int hash(std::string) {
		return 0;
	}

public:
	VarHash(int size) {
		hashSize = 3 * size;
		table = new keyVal[hashSize];
	}

	void*& operator[](std::string name) {

		//Get the hash key for the var name, and the associated pair
		int hashVal = hash(name);
		keyVal comp = table[hashVal];
		//Check that the value in the bucket is actually the pair requested
		while(comp.first != name) {
			//If the bucket is actually empty, then the requested pair does not exist
			//Therefore, we will need to add it, and we can stop looking in the table
			if(comp.second == NULL) {
				table[hashVal].first = name;
				std::cout << "Emplaced new variable" << std::endl;
				comp = table[hashVal];
			}
			//otherwise, continue looking towards the next value in the array
			else{
				hashVal = (1 + hashVal) % hashSize;

		 		comp = table[hashVal];
			}
		}
		
		return table[hashVal].second;
	}
//	const void*& operator[](const std::string name) {
//		return const_cast<const void*&>((*this)[name]);
//	}
};*/
