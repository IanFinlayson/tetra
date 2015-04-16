#ifndef MICRO_STACK_H
#define MICRO_STACK_H
//A simple little astack class, because I am unhappy with std::stack's ridiculous memory sizes

template <typename T>
class MicroStack {

private:
	struct Node {

		Node* next;
		T data;

		Node(T pData) : next(NULL), data(pData){}
	};



	Node* head;

public:

	MicroStack();
	~MicroStack();
	MicroStack(const MicroStack&);
	MicroStack<T>& operator=(const MicroStack&);
	void push(T);
	T pop();
	const T& top() const;
	T& top();
	bool empty() const;
	int size() const;
};


template<typename T>
MicroStack<T>::MicroStack() {
	head = NULL;
}

template<typename T>
MicroStack<T>::~MicroStack() {
	while(!empty()) pop();
}

template<typename T>
MicroStack<T>& MicroStack<T>::operator=(const MicroStack<T>& other) {
	if(&other == this) return *this;
	//Destroy self
	while(!empty())pop();

	if(other.empty())return *this;

	MicroStack<T> temp;
	Node* listy = other.head;
	while(listy != NULL) {
		temp.push(listy->data);
		listy = listy->next;
	}
	while(!temp.empty()){
		push(temp.pop());
	}
	return *this;
}

template<typename T>
MicroStack<T>::MicroStack(const MicroStack<T>& other) {
	head = NULL;
	if(other.empty())return;

	MicroStack<T> temp;
	Node* listy = other.head;
	while(listy != NULL) {
		temp.push(listy->data);
		listy = listy->next;
	}
	while(!temp.empty()){
		push(temp.pop());
	}
}
#include<iostream>
template<typename T>
void MicroStack<T>::push(T newData) {
	//static long x = 0;
	//x++;
	//std::cout<<x<<std::endl;
	Node* newNode = new Node(newData);
	Node* temp = head;
	head = newNode;
	newNode->next = temp;
}

template<typename T>
T MicroStack<T>::pop() {
	Node* temp = head;
	head = head->next;
	T ret = temp->data;
	delete temp;
	return ret;
}

template<typename T>
const T& MicroStack<T>::top() const {
	return head->data;
}

template<typename T>
T& MicroStack<T>::top() {
	return head->data;
}

template<typename T>
bool MicroStack<T>::empty() const {
	return head == NULL;
}

template<typename T>
int MicroStack<T>::size() const {
	int ret = 0;
	Node* temp = head;
	while (temp != NULL) {
		temp = temp->next;
		ret++;
	}
	return ret;
}

#endif
