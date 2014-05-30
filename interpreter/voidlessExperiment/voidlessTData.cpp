#ifndef TDAT
#define TDAT

#include <iostream>
#include <string>
#include "frontend.hpp"

using std::string;

template<typename T>
class TData{
	public:
		TData() {
			pointedTo = TYPE_VOID;
		}

		TData(T pData) {
			data = pData;
			pointedTo = TYPE_VOID;
		}

		//default destructor
		~TData(){}

	
		//If this TData object is storing a pointer to a dynamically allocated variable, it MUST USE THIS METHOD TO INFORM THE OBJECT THAT IT SHOULD DELETE SOMETHING WHEN IT IS DONE		
		template<typename R>
		void setDeletableType(){
			//do nothing by default, as only in very special circumstances will we need to be able to fiund out what we are pointing to
		}

		DataTypeKind getPointedTo() {
			return pointedTo;
		}

		template<typename R>
		void setData(R pData) {
			//By default, this should not do anything, but we will specialize the tamplate for the few scenarios where this actually works
			std::cout << "Assignment failed" << std::endl;
		}   

		T getData() {
			return data;
		}   
	private:
		T data;
		//used for deleting dynamically allocated data if this object utilizes it;
		DataTypeKind pointedTo;


};
//Specialized set deletable type stores the appropriate DataTypeKind accortding to what is passed into the template
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



//Explicitely specialized templates for setData so we can store primitive values and addresses for primitive values
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

//pointers to values (used for address lookups)
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
#endif
