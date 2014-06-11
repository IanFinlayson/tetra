#ifndef VOIDLESS_TDATA_H
#define VOIDLESS_TDATA_H

#include <iostream>
#include <string>
#include "frontend.hpp"


using std::string;

template<typename T>
class TData{
	public:
		TData();

		TData(const T pData);

		//Copy Constructor
		TData(const TData<T>& other);

		//default destructor
		~TData();

	
		//If this TData object is storing a pointer to a dynamically allocated variable, it MUST USE THIS METHOD TO INFORM THE OBJECT THAT IT SHOULD DELETE SOMETHING WHEN IT IS DONE		
		template<typename R>
		void setDeletableType();

		const DataType getPointedTo() const;

		template<typename R>
		void setData(const R& pData);

		TData<T>& operator=(const TData<T>&);

		const T& getData() const;
	private:
		T data;
		//used for deleting dynamically allocated data if this object utilizes it;
		DataType pointedTo;
};

template<typename T>
TData<T>::TData() : pointedTo(TYPE_VOID) {
	data = T();
}

template<typename T>
TData<T>::TData(const T pData) : pointedTo(TYPE_VOID) {
	data = pData;
	//pointedTo = DataType(TYPE_VOID);
}

template<typename T>
TData<T>::TData(const TData<T>& other) : pointedTo(other.pointedTo.getKind()) {
	data = other.data;
	//pointedTo = other.pointedTo;
}

//default destructor
template<typename T>
TData<T>::~TData() {
}

template <typename T>
TData<T>& TData<T>::operator=(const TData<T>& other) {
	//unles we are  working with variables (see TDataspecializations), a naive copy shouild be fine
	pointedTo = other.pointedTo;
	data = other.data;
	return *this;
}


//If this TData object is storing a pointer to a dynamically allocated variable, it MUST USE THIS METHOD TO INFORM THE OBJECT THAT IT SHOULD DELETE SOMETHING WHEN IT IS DONE		
template<typename T> template<typename R>
void TData<T>::setDeletableType(){
	cout << "Error, attmepting to set unsupported type as deletable type.\nThis may result in a memory leak." << endl;
	//do nothing by default, as only in very special circumstances will we need to be able to fiund out what we are pointing to
}
template<typename T>
const DataType TData<T>::getPointedTo() const {
	return pointedTo.getKind();
}

template<typename T> template<typename R>
void TData<T>::setData(const R& pData) {
	//By default, this should not do anything, but we will specialize the tamplate for the few scenarios where this actually works
	std::cout << "Assignment failed" << std::endl;
}   

template<typename T>
const T& TData<T>::getData() const {
	return data;
}   
#endif
