#ifndef T_ARRAY_H
#define T_ARRAY_H

/*
 * This class wraps an array of tData<void*> used to make arrays and multi-dimensional arrays
 */

#include<iostream>
#include<string>
#include<cstdlib>
#include"tData.cpp"
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
//	TArray& operator=(const TArray& other);

	//For debugging purposes
	template<typename T>
	void outputElements() const{
		cout << "\n\nOutput:" << endl;
		for(unsigned int index = 0; index < elements.size(); index++) {
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
	
	vector< TData<void*> > elements;
};


#endif
