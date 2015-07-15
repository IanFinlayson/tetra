/*
 * This class wraps all data types used in a tetra program.
 * A note about the comments, an "ordinary case" for using a TData object involves all situations EXCEPT when:
 * The template for the type is void* and...
 * the void* is pointing at a dynamically allocated piece of memory for which the TData has ownership
 */ 
/*
#ifndef VOIDLESS_TDATA_H
#define VOIDLESS_TDATA_H

#include <iostream>
#include <string>
#include "frontend.hpp"

//#define NDEBUG
#include <assert.h>

using std::string;

template<typename T>
class TData{
	public:
		TData();

		TData(const T& pData);

		//Copy Constructor
		TData(const TData<T>& other);

		//default destructor
		~TData();

	
		//If this TData object is storing a pointer to a dynamically allocated variable, it MUST USE THIS METHOD TO INFORM THE OBJECT THAT IT SHOULD DELETE SOMETHING WHEN IT IS DONE
		//In essence, this method notifies the object that it is no longer being used in an ordinary case, but has ownership of some dynamically allocated memory		
		template<typename R>
		void setDeletableType();

		const DataType getPointedTo() const;

		template<typename R>
		bool setData(const R& pData);

		TData<T>& operator=(const TData<T>&);

		const T& getData() const;
		T& getData();
	private:
		T data;
		//used for deleting dynamically allocated data if this object utilizes it;
		DataType pointedTo;
};


template<typename T>
TData<T>::TData() : data(), pointedTo(TYPE_VOID) {
}

template<typename T>
TData<T>::TData(const T& pData) : data(pData),  pointedTo(TYPE_VOID) {
}

//Default copy constructor is fine for ordinary case
template<typename T>
TData<T>::TData(const TData<T>& other) : data(other.data), pointedTo(other.pointedTo.getKind()) {
}

//default destructor, in ordinary cases no cleanup is needed
template<typename T>
TData<T>::~TData() {
}

//Naive copt is adequate under ordinary circumstances
template <typename T>
TData<T>& TData<T>::operator=(const TData<T>& other) {
	//unles we are  working with variables (see TDataspecializations), a naive copy shouild be fine
	pointedTo = other.pointedTo;
	data = other.data;
	return *this;
}

//Used to denote that the TData should treat itself as if it had a dynamically allocated member
//When set, TData will perform deep copies, and delete what it is pointing to when it is done
//If this TData object is storing a pointer to a dynamically allocated variable, it MUST USE THIS METHOD TO INFORM THE OBJECT THAT IT SHOULD DELETE SOMETHING WHEN IT IS DONE		
//'Default' behavior occurs when user calls this method with an unexpected variable type. This should bever happen, hence the assertion
template<typename T> template<typename R>
void TData<T>::setDeletableType(){
	//A non-specialized version of this method should never be called
	assert(false);
	//cout << "Error, attmepting to set unsupported type as deletable type.\nThis may result in a memory leak." << endl;
}

template<typename T>
const DataType TData<T>::getPointedTo() const {
	return pointedTo.getKind();
}

template<typename T> template<typename R>
bool TData<T>::setData(const R& pData) {
	//By default, this should not do anything, but we will specialize the tamplate for the few scenarios where this actually works
	//Note that this is occasionbally called on mismatched types. This is expected behavior for the interpreter.
	//As an exampole, a statement node may be expecting an int return type, because it was called within a function returning an int,
	//it then sees s = "abc" which returns a string. The interpreter will call TData<int>::setData<string>(...), and do nothing.
	return false;
}   

//Get a reference to what is stored within the object
template<typename T>
const T& TData<T>::getData() const {
	return data;
}

template<typename T>
T& TData<T>::getData() {
	return data;
}

#endif
*/
