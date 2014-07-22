//This class defines Tetra Standard Library functions

#include<iostream>
#include"frontend.hpp"
#include<string.h>
//#include"tArray.h"
//#include"tData.h"
//#include"progContext.h"
#include"backend.hpp"

template <typename T>
void evaluateNode(const Node*,TData<T>&,TetraContext&);


//Prints (recursively) the expression(s) denoted by args
//Presently, this method has a circular dependancy with the evaluateNode() method of main.cpp
void print(const Node* args, TetraContext& context) {
	
	//Check if args is a structual node
	if (args->kind() == NODE_ACTUAL_PARAM_LIST) {
		print(args->child(0),context);
		//Check if there are further things to print
		if(args->numChildren() == 2) {
			print(args->child(1),context);
		}
	}
	//If args is an expression, actually print it!
	else{
		switch(args->type()->getKind()) {
			case TYPE_INT:
			{
				TData<int> value;
				evaluateNode<int>(args,value,context);
				//Prints the value, flushes the buffer WITHOUT a new line
				TetraEnvironment::getOutputStream() << value.getData() << std::flush;
			}
			break;
			case TYPE_REAL:
			{   
				TData<double> value;
				evaluateNode<double>(args,value,context);
				TetraEnvironment::getOutputStream() << value.getData() << std::flush;
			} 
			break;
			case TYPE_BOOL:
			{   
				TData<bool> value;
				evaluateNode<bool>(args,value,context);
				TetraEnvironment::getOutputStream() << value.getData() << std::flush;
			} 
			break;
			case TYPE_STRING:
			{   
				TData<string> value;
				evaluateNode<string>(args,value,context);
				TetraEnvironment::getOutputStream() << value.getData() << std::flush;
			} 
			break;
			case TYPE_VECTOR:
			{   
				TData<TArray> value;
				evaluateNode<TArray>(args,value,context);
				TetraEnvironment::getOutputStream() << value.getData() << std::flush;
			} 
			break;
			default:
			//print that we did not recognize whatever it is we are supposed to print
				TetraEnvironment::getOutputStream() << "??" << std::flush;
		}
	}
}

int readInt() {
	int ret = 0;
	while(!(cin >> ret)) {
		//Moves down the stream until it finds a readable number
		cin.clear();
		cin.ignore();
	}
	return ret;
}

double readReal() {
	double ret = 0;
	while(!(cin >> ret)) {
		//moves along the buffer until it finds a readable value
		cin.clear();
		cin.ignore();
	}
	return ret;
}

bool readBool() {
	//Returns false on some variation of 'false', 'no', or 0
	string input;
	cin >> input;
	//Compare input against the recognized values for false
	if(input == "false" || input == "no" || input == "0") {
		return false;
	}
	else return true;
}

string readString() {
	string ret = "";
	//For now, we will assume that this is always successful
	cin >> ret;

	return ret;
}

int len(TArray& arg) {
	return arg.size();
}

int len(string& arg) {
	return arg.size();
}
