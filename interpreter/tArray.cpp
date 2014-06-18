/*
 * This class wraps an array of tData<void*> used to make arrays and multi-dimensional arrays
 */

#include<iostream>
#include<string>
#include<cstdlib>
#include<sstream>
#include"tData.h"
#include"frontend.hpp"
#include "tArray.h"

using std::string;
using std::vector;

/*TArray::TArray() : elements(new vector< TData<void*> >){

}

TArray::TArray(const TArray& other) :elements(new vector< TData<void*> >) {
	*elements = *(other.elements);
}
*/
//Code for when elements was a pointer
TArray::TArray() {
	//default constructs elements and initialize vector pointer
}


TArray::TArray(const TArray& other) {
	//Initialize the pointer, then copy construct the containing vector it
	//elements = new vector< TData<void*> >(other.elements->size());
	elements = other.elements;
}

TArray& TArray::operator=(const TArray& other) {
	if(this != &other) {
		//Release current memory, then copy the underlying vector
		//delete elements;
		elements = other.elements;
	}
	return *this;
}

//If the TData objects added to this object are set as deletable, they must be destroyed along with this object
TArray::~TArray() {
	//cout << "destroying elements of array: " << endl;
/*	for(std::vector< TData<void*> >::iterator iter = elements->begin(); iter < elements->end(); iter++) {
		switch((*iter).getPointedTo().getKind()) {
			case TYPE_INT:
				std::cout << "Had value: " << *static_cast<int*>((*iter).getData()) << std::endl;
				delete static_cast<int*>((*iter).getData());
				break;
			case TYPE_REAL:
				std::cout << "Had value: " << *static_cast<double*>((*iter).getData()) << std::endl;    
				delete static_cast<double*>((*iter).getData());
				break;
			case TYPE_BOOL:
				std::cout << "Had value: " << *static_cast<bool*>((*iter).getData()) << std::endl;    
				delete static_cast<bool*>((*iter).getData());
				break;
			case TYPE_STRING:
				std::cout << "Had value: " << *static_cast<string*>((*iter).getData()) << std::endl;    
				delete static_cast<string*>((*iter).getData());
				break;
			case TYPE_VECTOR:
				std::cout << "Deleting vector...Had values" << endl;
				delete static_cast<TArray*>((*iter).getData());
				break;
			default:
				std::cout << "nothing deleted, possible mem leak" << std::endl;
				break;

		}
	}*/
}
/*
TArray& TArray::operator=(const TArray& other) {
	elements = vector<TData <void*> >();
	cout << "!!" << endl;
	cout << elements.size() << endl;
	elements = other.elements;
*//*	for(unsigned int x = 0; x < other.elements.size(); x++) {
		cout << "Adding element in copy assignment..." << endl;
		cout << elements.size() << endl;
		cout << other.elementAt(x).getData() << "<<<<" << endl;
		TData<void*> inserter(other.elementAt(x));
		cout << "SUCCESS: " << &elements << endl;
		addElement(inserter);
		cout << "Success2" << endl;
	}*/
//	return *this;
//}

TData<void*>& TArray::elementAt(unsigned int index) {
	//check for out-of-bounds access (note that parameter is unsigned, cannot be < 0
//	cout << &elements << "<<<<<<!!!!!" << endl;
//	cout << "Size: " << elements.size();
	if(index < elements->size()) {
		return (*elements)[index];
	}
	else {
		//Print error message and terminate
		std::stringstream message;
		message << "Attempted to access an array out of bounds.\nRequested element " << index << " for array of size " << elements->size();
		Error e(message.str(),0);
		throw e;
	}
	//std::cout << "Error, attempted to access an array out of bounds.\nRequested element #" << index << " for array of size " << elements.size() << endl; 
	//exit(EXIT_FAILURE);
}

const TData<void*>& TArray::elementAt(unsigned int index) const {
	//check for out-of-bounds access (note that parameter is unsigned, cannot be < 0
	if(index < elements->size()) {
		return (*elements)[index];
	}
	else {
		std::stringstream message;
		message << "Attempted to access an array out of bounds.\nRequested element " << index << " for array of size " << elements->size();
		Error e(message.str(),0);
		throw e;
	
	}
	//Print error message and terminate
	//std::cout << "Error, attempted to access an array out of bounds.\nRequested element #" << index << " for array of size " << elements->size() << endl; 
	//exit(EXIT_FAILURE);
}

//NOTE: due to the nature of TData<void*> copy constructor allocating new memory, elements added must have whatever they are pointing at deleted!
void TArray::addElement(const TData<void*>& pElement) {
	//cout << (&elements) << endl;
	elements->push_back(pElement);
}

const std::vector< TData<void*> >::const_iterator TArray::begin() const {
	return elements->begin();
}


const std::vector< TData<void*> >::const_iterator TArray::end() const {
	return elements->end();
}
