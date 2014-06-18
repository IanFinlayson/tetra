#ifndef T_ARRAY_H
#define T_ARRAY_H

/*
 * This class wraps an array of tData<void*> used to make arrays and multi-dimensional arrays
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
	//When elements are added to the vector, they are allocated with new (as per TData copy constructor), so they must be released when this gets destroyed
	~TArray();


	//gets the element pointed to by
	TData<void*>& elementAt(unsigned int);
	const TData<void*>& elementAt(unsigned int) const;
	void addElement(const TData<void*>&);	
	const std::vector< TData<void*> >::const_iterator begin() const;
	const std::vector< TData<void*> >::const_iterator end() const;

	//Copy assignment operator
	TArray& operator=(const TArray& other);

	//For debugging purposes
	template<typename T>
	void outputElements() const{
		cout << "\n\nOutput:" << endl;
		for(unsigned int index = 0; index < elements->size(); index++) {
			cout << "Element" << index << ": " << *static_cast<T*>(elementAt(index).getData()) << endl;
		}
	}

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
			addReference();
			cout << "Ref count: " << *refCount << endl;
		}
		~vec_ptr() {
			removeReference();
			if(*refCount == 0) {
				cout << "Deleting in destructor: " << ptr << endl;
				delete refCount;
				delete ptr;
				cout << "Smart pointer deleted array" << endl;
			}
		}
		//Aliases the pointer
		vec_ptr& operator=(const vec_ptr& other) {
			if(&other != this) {
				removeReference();
				if(*refCount == 0) {
					cout << "Deleting: " << ptr << endl;
					delete refCount;
					delete ptr;
					cout << "Smart pointer deletecd array in copy assignment" << endl;
				}
				ptr = other.ptr;
				refCount = other.refCount;
				addReference();
				cout << "Ref count: " << *refCount << endl;
			}
			return *this;
		}
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
	
	//vector< TData<void*> >* elements;
	vec_ptr elements;
};


#endif
