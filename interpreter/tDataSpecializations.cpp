/*
 * This file contains specializations for the TData class
 */

#ifndef T_DATA_SPECS_H
#define T_DATA_SPECS_H

#include "tData.cpp"


//Specialization of copy constructor for types looking at dynamic memory

template <>
TData<void*>::TData(const TData<void*>& other) {
	//if the data points to something, we must perform a deep copy
	if(other.pointedTo != TYPE_VOID) {
		//std::cout << " Deep data Copy Constructor " << std::endl;
		switch(other.pointedTo) {
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
			default:
				std::cout << "Warning, attempted to deep copy unsupported primitive type ID: " << other.pointedTo << std::endl;
		}
	}
	else {
			//if not working with memory, ordinary assignment will do
			data = other.data;
	}
	pointedTo = other.pointedTo;
}


//Used to actually store data when storing
template<> template<>
void TData<int>::setData<int>(int pData) {
	        data = pData;
}

template<> template<>
void TData<double>::setData<double>(double pData) {
	        data = pData;
}

template<> template<>
void TData<bool>::setData<bool>(bool pData) {
	        data = pData;
}

template<> template<>
void TData<string>::setData<string>(string pData) {
	        data = pData;
}

//Pointer specializations (used for address lookup)

template<> template<>
void TData<int*>::setData<int*>(int* pData) {
	        data = pData;
}

template<> template<>
void TData<double*>::setData<double*>(double* pData) {
	        data = pData;
}

template<> template<>
void TData<bool*>::setData<bool*>(bool* pData) {
	        data = pData;
}

template<> template<>
void TData<string*>::setData<string*>(string* pData) {
	        data = pData;
}

//Specializations for insuring there are no memory leaks when allocating data

template<> template<>
void TData<void*>::setDeletableType<int>(){
	        pointedTo = TYPE_INT;
}

template<> template<>
void TData<void*>::setDeletableType<double>(){
	        pointedTo = TYPE_REAL;
}

template<> template<>
void TData<void*>::setDeletableType<bool>(){
	        pointedTo = TYPE_BOOL;
}

template<> template<>
void TData<void*>::setDeletableType<string>(){
	        pointedTo = TYPE_STRING;
}


//Specialization of assignment when data is a pointer type

template <>
TData<void*>& TData<void*>::operator=(TData<void*>& other) {
	//if the data points to something, we must perform a deep copy
	if(other.pointedTo != TYPE_VOID) {
		//std::cout << " Deep data CC " << std::endl;
		switch(other.pointedTo) {
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
			default:
				std::cout << "Warning, attempted to deep copy unsupported primitive type ID: " << other.pointedTo << std::endl;
		}
	}
	else {
			//if not working with memory, ordinary assignment will do
			data = other.data;
	}
	pointedTo = other.pointedTo;
	return *this;
}



#endif
