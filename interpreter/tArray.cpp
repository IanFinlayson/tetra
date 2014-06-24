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

//Code for when elements was a pointer
TArray::TArray() {
	//default constructs elements and initialize vector pointer
}

//Because default behavior of arrays in tetra is aliasing, the copy constructor and copy assignment will also alias rather than deep copy
TArray::TArray(const TArray& other)  {
	//Initialize the pointer, then alias it
	elements = other.elements;
}

TArray& TArray::operator=(const TArray& other) {
	if(this != &other) {
		elements = other.elements;
	}
	return *this;
}

//Stubbed The only thing that needs to be destructed is the pointerm which is destructed by default
TArray::~TArray() {

}

//Returns a reference to a TData containing a pointer to the data associated with index. Note that this can be used for reading AND writing
TData<void*>& TArray::elementAt(unsigned int index) {
	//check for out-of-bounds access (note that parameter is unsigned, cannot be < 0
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
}

//The const version of elementAt
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
}

//Adds the giveen element to the end of the array. Note that is it is pointing at allocated memory, it should have had its setDeletableType method called
void TArray::addElement(const TData<void*>& pElement) {
	elements->push_back(pElement);
}

//Get the start and end iterators to the underlying vector container
const std::vector< TData<void*> >::const_iterator TArray::begin() const {
	return elements->begin();
}

const std::vector< TData<void*> >::const_iterator TArray::end() const {
	return elements->end();
}
