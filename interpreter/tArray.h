#ifndef T_ARRAY_H
#define T_ARRAY_H

/*
 * This class wraps an array of tData<void*> used to make arrays and multi-dimensional arrays
 * 
 */

#include<iostream>
#include<string>
#include<cstdlib>
#include"tData.h"
#include"frontend.hpp"

using std::string;
using std::vector;

class TArray {

public:

	TArray();
	TArray(const TArray&);
	~TArray();


	//gets the element pointed to by
	//(const and non-const versions)
	TData<void*>& elementAt(unsigned int);
	const TData<void*>& elementAt(unsigned int) const;

	//Add an element using copy constructor
	void addElement(const TData<void*>&);
	
	//Utility methods used for iterating over the vector in for (-each) loops, and vector cleanup	
	const std::vector< TData<void*> >::const_iterator begin() const;
	const std::vector< TData<void*> >::const_iterator end() const;

	int size() const;

	//Copy assignment operator
	TArray& operator=(const TArray& other);

	//All operations must be defined (or at least stubbed) to work with our operators!
	TArray operator||(TArray& other) {
		return *this;
	}
	TArray operator&&(TArray& other) {
		return *this;
	}
	bool operator<(TArray& other) {
		return false;
	}
	bool operator<=(TArray& other) {
		return false;
	}
	bool operator>(TArray& other) {
		return false;
	}
	bool operator>=(TArray& other) {
		return false;
	}
	bool operator==(TArray& other) {
		return false;
	}
	bool operator!=(TArray& other) {
		return false;
	}
	TArray operator^(TArray& other) {
		return *this;
	}
	TArray operator|(TArray& other) {
		return *this;
	}
	TArray operator&(TArray& other) {
		return *this;
	}
	TArray operator<<(TArray& other) {
		return *this;
	}
	TArray operator>>(TArray& other) {
		return *this;
	}
	TArray operator+(TArray& other) {
		return *this;
	}
	TArray operator-(TArray& other) {
		return *this;
	}
	TArray operator*(TArray& other) {
		return *this;
	}
	TArray operator/(TArray& other) {
		return *this;
	}
	TArray operator%(TArray& other) {
		return *this;
	}
	TArray operator!() {
		return *this;
	}
	//The specialization for TArray EXP TArray is presently in the operationMap class
	
private:

	//Implementation of smart pointer for vector that uses simple reference counting
	class vec_ptr {
	public:
		vec_ptr() {
			ptr = new vector< TData<void*> >;
			refCount = new int();//Zero initialized
			*refCount = 0;
			addReference();
		}

		//Copy constructor aliases this TArray to the other, rather than performing a deep copy
		//Note that this is largely desired behavior
		vec_ptr(const vec_ptr& other) {
			ptr = other.ptr;
			refCount = other.refCount;
			addReference();	
		}

		~vec_ptr() {
			removeReference();

			//Check to see if we must delete the underlying object
			if(*refCount == 0) {
				delete refCount;
				delete ptr;
			}
		}

		//Assignment operator aliases the pointer
		//Note that this means that the copy assignment operator/copy constructor will perform a SHALLOW copy
		//For the purposes of the interpreter, however, this is the desired behavior
		vec_ptr& operator=(const vec_ptr& other) {
			if(&other != this) {
				removeReference();
				//Check to see if we must delete the vector that this used to point to
				if(*refCount == 0) {
					delete refCount;
					delete ptr;
				}
				ptr = other.ptr;
				refCount = other.refCount;
				addReference();
			}
			return *this;
		}

		//Methods to simulate pointer functionality
		vector< TData<void*> >& operator*() const {
			return *ptr;
		} 
		vector< TData<void*> >* operator->() const {
			return ptr;
		}
		
	private:
		void addReference() {
			(*refCount)++;
		}
		void removeReference() {
			(*refCount)--;
		}

		vector< TData<void*> >* ptr;
		int* refCount;
	};
	
	//A smart pointer to a vector of void*
	vec_ptr elements;

	//Implementation of stream insertion allows for printing of array
	friend std::ostream& operator<<(std::ostream& outStream, TArray obj);
};


template<> TData<void*>::TData(const TData<void*>& other);
template<> TData<void*>::~TData();
template<> TData<void*>& TData<void*>::operator=(const TData<void*>&);
template<> template<> void TData<void*>::setDeletableType<TArray>();

//Template specializations for setData and setDeletable type
//Declared here because TData<TArray> must be defined before all declarations may occur
template<> template<> bool TData<int>::setData<int>(const int&);
template<> template<> bool TData<double>::setData<double>(const double&);
template<> template<> bool TData<bool>::setData<bool>(const bool&);
template<> template<> bool TData<string>::setData<string>(const string&);
template<> template<> bool TData<int*>::setData<int*>(int* const &);
template<> template<> bool TData<double*>::setData<double*>(double* const &);
template<> template<> bool TData<string*>::setData<string*>(string* const &);
template<> template<> bool TData<bool*>::setData<bool*>(bool* const &);
template<> template<> bool TData<TArray*>::setData<TArray*>(TArray*const &);
template<> template<> bool TData<void*>::setData<void*>(void* const &);
template<> template<> void TData<void*>::setDeletableType<int>();
template<> template<> void TData<void*>::setDeletableType<double>();
template<> template<> void TData<void*>::setDeletableType<bool>();
template<> template<> void TData<void*>::setDeletableType<string>();
template<> template<> void TData<void*>::setDeletableType<void>();
template<> template<> bool TData<TArray>::setData<TArray>(const TArray&);


#endif
