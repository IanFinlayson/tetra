/*
 * This file contains specializations for the TData class
 */

//#ifndef T_DATA_SPECS_H
//#define T_DATA_SPECS_H

#include "tData.h"
#include "tArray.h"

//Specialization of copy constructor for types looking at dynamic memory
template <>
TData<void*>::TData(const TData<void*>& other) : pointedTo(other.pointedTo.getKind()) {
	//if the data points to something, we must perform a deep copy
	if(other.pointedTo != TYPE_VOID) {
		//std::cout << " Deep data Copy Constructor " << std::endl;
		switch(other.pointedTo.getKind()) {
			case TYPE_INT:
				data = new int;
				*(static_cast<int*>(data)) = *static_cast<int*>(other.data);
			break;
			case TYPE_REAL:
				data = new double;
				*(static_cast<double*>(data)) = *static_cast<double*>(other.data);
			break;
			case TYPE_BOOL:
				data = new bool;
				*(static_cast<bool*>(data)) = *static_cast<bool*>(other.data);
			break;
			case TYPE_STRING:
				data = new string;
				*(static_cast<string*>(data)) = *static_cast<string*>(other.data);
			break;
			case TYPE_VECTOR:
				data = new TArray;
				*(static_cast<TArray*>(data)) = *static_cast<TArray*>(other.data);
			break;
			default:
				std::cout << "Warning, attempted to deep copy unsupported primitive type ID: " << other.pointedTo.getKind() << std::endl;
		}
	}
	else {
			//If we are not actually working with dynamic memory, ordinary assignment will do
			data = other.data;
	}
	pointedTo = other.pointedTo;
}

//Destructor for TData<ovid*> with possible ownership of dynamically allocated memory
template <>
TData<void*>::~TData() {
	//if the data points to something, we must delete it
	if(pointedTo != TYPE_VOID) {
		switch(pointedTo.getKind()) {
			case TYPE_INT:
				delete (static_cast<int*>(data));
			break;
			case TYPE_REAL:
				delete (static_cast<double*>(data));
			break;
			case TYPE_BOOL:
				delete (static_cast<bool*>(data));
			break;
			case TYPE_STRING:
				delete (static_cast<string*>(data));
			break;
			case TYPE_VECTOR:
				delete (static_cast<TArray*>(data));
			break;
			default:
				std::cout << "Warning, attempted to delete unsupported type ID: " << pointedTo.getKind() << std::endl;
		}
	}
}

//Used to actually store data when using setData
//The reason these are templated like this is to allow for later definition of what should happen when attempting to assign cross datatypes
//These can be defined within the language merely by defioning a proper specialization
template<> template<>
bool TData<int>::setData<int>(const int& pData) {
	        data = pData;
		return true;
}

template<> template<>
bool TData<double>::setData<double>(const double& pData) {
	        data = pData;
		return true;
}

template<> template<>
bool TData<bool>::setData<bool>(const bool& pData) {
	        data = pData;
		return true;
}

template<> template<>
bool TData<string>::setData<string>(const string& pData) {
	        data = pData;
		return true;
}

template<> template<>
bool TData<TArray>::setData<TArray>(const TArray& pData) {
	        data = pData;
		return true;
}

//Pointer specializations (used for address lookup)
template<> template<>
bool TData<int*>::setData<int*>(int* const &pData) {
	        data = pData;
		return true;
}

template<> template<>
bool TData<double*>::setData<double*>(double* const &pData) {
	        data = pData;
		return true;
}

template<> template<>
bool TData<bool*>::setData<bool*>(bool* const &pData) {
	        data = pData;
		return true;
}

template<> template<>
bool TData<string*>::setData<string*>(string* const &pData) {
	        data = pData;
		return true;
}

template<> template<>
bool TData<TArray*>::setData<TArray*>(TArray* const &pData) {
	        data = pData;
		return true;
}

//This is used when evaluating TArrays, as the addresses of data need to be assigned to TData<void*> objects
template<> template<>
bool TData<void*>::setData<void*>(void* const &pData) {
	        data = pData;
		return true;
}

//Specializations allowing the interpreter to notify TData object that it is holding onto memory that will need to be released
//Used for insuring there are no memory leaks when allocating data

template<> template<>
void TData<void*>::setDeletableType<int>(){
	        pointedTo = DataType(TYPE_INT);
}

template<> template<>
void TData<void*>::setDeletableType<double>(){
	        pointedTo = DataType(TYPE_REAL);
}

template<> template<>
void TData<void*>::setDeletableType<bool>(){
	        pointedTo = DataType(TYPE_BOOL);
}

template<> template<>
void TData<void*>::setDeletableType<string>(){
	        pointedTo = DataType(TYPE_STRING);
}

//Denotes array type
template<> template<>
void TData<void*>::setDeletableType<TArray>(){
	        pointedTo = DataType(TYPE_VECTOR);
}


template<> template<>
void TData<void*>::setDeletableType<void>(){
	        pointedTo = DataType(TYPE_VOID);
}

//Specialization of assignment when data is a pointer type

template <>
TData<void*>& TData<void*>::operator=(const TData<void*>& other) {

	//Check self assignment
	if(&other == this) {
		return *this;
	}

	//if the data points to something, we must perform a deep copy
	if(other.pointedTo != TYPE_VOID) {
		switch(other.pointedTo.getKind()) {
			case TYPE_INT:
				delete static_cast<int*>(data);
				data = new int;
				*(static_cast<int*>(data)) = *static_cast<int*>(other.data);
			break;
			case TYPE_REAL:
				delete static_cast<double*>(data);
				data = new double;
				*(static_cast<double*>(data)) = *static_cast<double*>(other.data);
			break;
			case TYPE_BOOL:
				delete static_cast<bool*>(data);
				data = new bool;
				*(static_cast<bool*>(data)) = *static_cast<bool*>(other.data);
			break;
			case TYPE_STRING:
				delete static_cast<string*>(data);
				data = new string;
				*(static_cast<string*>(data)) = *static_cast<string*>(other.data);
			break;
			case TYPE_VECTOR:
				delete static_cast<TArray*>(data);
				data = new TArray;
				*(static_cast<TArray*>(data)) = *static_cast<TArray*>(other.data);
			break;
			default:
				std::cout << "Warning, attempted to deep copy unsupported primitive type ID: " << other.pointedTo.getKind() << std::endl;
		}
	}
	else {
			//if not working with memory, ordinary assignment will do
			data = other.data;
	}
	pointedTo = other.pointedTo;
	return *this;
}

//#endif
