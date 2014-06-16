/*
 * This program interprets a tetra file by getting its tree representation
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include "frontend.hpp"
#include "tArray.h"
#include "tData.h"
#include "functionTable.h"
#include "nodeTable.h"
#include "operationMap.cpp"
#include "comparisonMap.cpp"
#include "variableContext.h"
#include "progContext.h"

//#define NDEBUG
#include <assert.h>

//temporary global TetraContext for testing purposes
//TetraContext vars;

using namespace std;

template <typename T>
T paste(const Node*,const Node*,TetraContext&);
template <typename T>
T paste(const Node*,const Node*,TetraScope&,TetraContext&);
template <typename T>
void evaluateNode(const Node*,TData<T>&,TetraContext&);
void runTest(TetraContext&);//function that contains test code
void aliasArray(const Node*, TArray&, TetraContext&);
template<typename T>
void evaluateAddress(const Node*, TData<T>&, TetraContext&);

//used for debugging purposes
void descNode(const Node* n) {
	cout << "--------" << endl;
	cout << n->getLine() << endl;
	cout << n->getString() << endl;
	cout << n->getInt() << endl;
	cout << n->getReal() << endl;
	cout << n->getBool() << endl;
	cout << n->kind() << endl;
	cout << ((n->type() == NULL)?"No type":typeToString(n->type())) << endl;
	cout << n->numChildren() << endl;
	//cout << "x: " << n->hasSymbol("x") <<endl;
}

//Simplifies an indexed vector value
//Here, T should be 
template<typename T>
void evaluateVecVal(const Node* node, TArray* vec, TData<T>& ret, TetraContext& context) {
	TData<int> indexNum;
	//assuming a node index MUST simplify to an integer
	evaluateNode<int>(node->child(0),indexNum,context);
		
	//Check if there are further idecies which must be evaluated
	if(node->numChildren() == 2){
		//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		//Get the vector pointed to by this index
		TArray* nextVec = static_cast<TArray*>(vec->elementAt(indexNum.getData()).getData());
		evaluateVecVal<T>(node->child(1),nextVec,ret,context);
	}
	else { //If we have accounted for all the indeces
		T value = *static_cast<T*>(vec->elementAt(indexNum.getData()).getData());
		ret.setData(value);
	
	}
	
}

//executes structural statementsi
template<typename T>
void evaluateStatement(const Node* node, TData<T>& ret, TetraContext& context) {
	switch(node->kind()) {
		case NODE_FUNCTION:
		{
			//If there were arguments, meaning that child(0) contains the body of code and child 1 is empty
			if(node->child(1) != NULL) {
				evaluateNode<T>(node->child(1),ret,context);
			} else {
				evaluateNode<T>(node->child(0),ret,context);
			}
			
		}
		break;
		case NODE_STATEMENT:
		{
			evaluateNode<T>(node->child(0),ret,context);
			//did the statement result in a break, continue, return, etc?
			ExecutionStatus status = context.queryExecutionStatus();
			if(status != RETURN && status != BREAK && status != CONTINUE) {
				evaluateNode<T>(node->child(1),ret,context);
			}
		}
		break;
		case NODE_IF:
		{
			TData<bool> conditional;
			//evaluate the condition
			evaluateNode<bool>(node->child(0),conditional,context);

			//if the condition was found to be true, execute the 2nd branch
			if(conditional.getData() == true) {
				evaluateNode<T>(node->child(1),ret,context);
			} else {
				//check for else block and execute it if it exists
				if(node->child(2) != NULL) {
					evaluateNode<T>(node->child(2),ret,context);
				}
			}
		}
		break;
		case NODE_WHILE:
		{
			TData<bool> conditional;
			//evaluate the condition
			evaluateNode<bool>(node->child(0),conditional, context);

			//if the condition was found to be true, execute the 2nd branch
			//Then, reevaluate the condition
			while(conditional.getData() == true) {
				evaluateNode<T>(node->child(1),ret,context);

				//Check is any flags are set, and take the appropriate action
				ExecutionStatus status = context.queryExecutionStatus();
				//If returning, stop the loop and return
				if(status == RETURN) {
					break;
				}
				//If breaking, notify the TetraContext that we have reached the outside of the loop, then  stop the loop
				if(status == BREAK) {
					//Note that is the status was formerly ELIF, the status flag was set to be normalized anyways (since we have to take a branch to encounter a break statement)
					context.normalizeStatus();
					break;
				}
				//If we are continuing, notify the context that we reached the end of the loop, then continue on (reevaluaate condition, etc)
				if(status == CONTINUE) {
					context.normalizeStatus();
				}

				//check the condition again
				evaluateNode<bool>(node->child(0),conditional,context);
			}		
		}
		break;
		case NODE_ELIF:
		{
			context.notifyElif();

	//		cout << context->queryExecutionStatus() << endl;

			//Traverse the first branch
			evaluateNode<T>(node->child(0),ret,context);

			//present problem
			ExecutionStatus status = context.queryExecutionStatus();

			//cout << "Status? " << status << " =ret? " << ( status == RETURN) << endl;

			//evaluate the if-else chain if the first elif clause was false
			if(status == ELIF) {
				evaluateNode<T>(node->child(1),ret,context);
			}
			
			//Again, check the flag to see if we need to execute the catchall else statement
			
			status = context.queryExecutionStatus();

			if(status == ELIF && node->child(2) != NULL) {
				evaluateNode<T>(node->child(2),ret,context);
			}
		}
		break;
		case NODE_ELIF_CHAIN:
		{
			evaluateNode<T>(node->child(0),ret,context);

			//After evaluating the clause, check the flag to see if we need to try the next clause
			ExecutionStatus status = context.queryExecutionStatus();

			if(status == ELIF) {
				evaluateNode<T>(node->child(1),ret,context);
			}
		}
		break;
		case NODE_ELIF_CLAUSE:
		{
			TData<bool> condition;
			evaluateNode<bool>(node->child(0),condition,context);
			//if condiiton is true ,execute the body
			if(condition.getData() == true) {
				context.normalizeStatus();
				evaluateNode<T>(node->child(1),ret,context);
			}
		}
		break;
		case NODE_VECREF:
		{
			//Here, T should be the type that needs to be returned, if not, then we really didn;t need it anyways
			TArray* lookupArray = context.lookupVar<TArray>(node->child(0)->getString());
			evaluateVecVal<T>(node->child(1),lookupArray,ret,context);
		}
		break;
		case NODE_FOR:
		{
			//Obtain the list of elements we need to loop over
			TData<TArray> actualCollection;//If the collection does not exist yet, then we will need to place it in memory
			TData<TArray*> collection;//This should not be NULL after the next statment
	//		cout << "Evaluating the collection" << endl;
	//		cout << &collection.getData() << endl;
			//Check to see if the array can be evaluated by reference, or see if it is a literal that must be evaluated before we can use it
			if(node->child(1)->kind() == NODE_IDENTIFIER || node->child(1)->kind() == NODE_VECREF) {
				cout << "Evaluating for collection by reference" << endl;
				//Put the address of the addressable vector into collection
				evaluateAddress<TArray*>(node->child(1), collection, context);
				//aliasArray(node->child(0), collection.getData(), context);
			}
			else {
				//If we must evaluate by value, we need 
				cout << "Evaluating for collection by value" << endl;
				evaluateNode<TArray>(node->child(1),actualCollection,context);
				//Cast required to acxtually p[erform assignment
				TArray* collection_ptr = &actualCollection.getData();
				collection.setData<TArray*>(collection_ptr);
			}
			//evaluateNode<TArray>(node->child(1),collection,context);
			cout << "Address of collection: " << collection.getData() << endl;
	//		cout << "Colleciton evaluated" << endl;
			//assert(collection != NULL);

			ExecutionStatus status = context.queryExecutionStatus();

			//For_each loop with some extra conditions tacked on
			for(std::vector< TData<void*> >::const_iterator iter = collection.getData()->begin(); iter != collection.getData()->end() && status != BREAK && status != RETURN; iter++) {
	//			cout << "Switcheroo" << endl;
				context.normalizeStatus();
//				assert (node->child(1)->type()->getSub()->getKind() != NULL);
				//Give the variable element in the for_each loop the proper value
				
				switch(node->child(0)->type()->getKind()/*collection.getData().elementAt(0).getPointedTo().getKind()*/) {
					case TYPE_INT:
						*(context.lookupVar<int>(node->child(0)->getString())) = *static_cast<int*>((*iter).getData());
						evaluateNode<T>(node->child(2), ret, context);
					break;
					case TYPE_REAL:
						*(context.lookupVar<double>(node->child(0)->getString())) = *static_cast<double*>((*iter).getData());
						evaluateNode<T>(node->child(2), ret, context);
					break;
					case TYPE_BOOL:
						*(context.lookupVar<bool>(node->child(0)->getString())) = *static_cast<bool*>((*iter).getData());
						evaluateNode<T>(node->child(2), ret, context);
					break;
					case TYPE_STRING:
	//					cout << "Case string" << endl;
						*(context.lookupVar<string>(node->child(0)->getString())) = *static_cast<string*>((*iter).getData());
						evaluateNode<T>(node->child(2), ret, context);
					break;
					case TYPE_VECTOR:
						//Alias the array with collection
						aliasArray(node->child(0), *static_cast<TArray*>((*iter).getData()), context);
					/*
						//Check if the array we passed in is already in memory (is an identifier or vector reference). If so, we will pass by reference, else we must pass by value
						if(node->child(1)->kind() == NODE_IDENTIFIER || node->child(1)->kind() == NODE_VECREF) {
							cout << "Evaluating for argument by reference" << endl;
							aliasArray(node->child(0), *static_cast<TArray*>((*iter).getData()), context);
						}
						else {
							cout << "Evaluating for reference by value" << endl;
							*(context.lookupVar<TArray>(node->child(0)->getString())) = *static_cast<TArray*>((*iter).getData());
						}*/
						evaluateNode<T>(node->child(2), ret, context);
					break;
					default:
						cout << "ERROR: attempting to assign to unknown DATA_TYPE: " << node->child(0)->type()->getKind() << "\n Aborting..." << endl;
						exit(EXIT_FAILURE);
				}
				//Check the current status flag to see if we have to break out
				status = context.queryExecutionStatus();
			}

			//If we broke out of the loop, or continued to the condition and the condiiton failed, normalize the flag
			if(status == BREAK || status == CONTINUE) {
				context.normalizeStatus();
			}

		}
		break;
		case NODE_PASS:
			//do nothing, just return
			return;
		break;
		default:
			cout << "Unsupported nodekind encountered in statement" << endl;
	}
}

//Used for aliasing arrays within the same scope (e.g. for loops)
//Makes it so that the address denoted by Node1 points to the array denoted by arrayArg
void aliasArray(const Node* node, TArray& arrayArg, TetraContext& context) {

	string formalParamName = node->getString();
	//cout << "Destinaiton: " << formalParamName << " Source: " << node2->getString() << endl;

	TData<void*>& alias = context.declareReference(formalParamName);
	//Get a pointer to the TData location of the destination scope
	
	//Generics needed so that the method recognizes void* and TArray* as the same data type
	//cout << "source_context address: " << sourceContext.lookupVar<TArray>(node2->getString()) << endl;;
	alias.setData<void*>(&arrayArg);
	cout << ">>>>>>>>>>>>>>>>>>alias points to: " << alias.getData() << endl;
}
//Makes it so that the address denoted by Node1 points to the array denoted by node2
//This version is used for function calls, where the aliasing takes place across scopes
void aliasArray(const Node* node1, const Node* node2, TetraScope& destinationContext, TetraContext& sourceContext) {

	string formalParamName = node1->getString();
	//cout << "Destinaiton: " << formalParamName << " Source: " << node2->getString() << endl;

	TData<void*>& alias = destinationContext.declareReference(formalParamName);
	//Get a pointer to the TData location of the destination scope
	
	cout << "source_context address: " << sourceContext.lookupVar<TArray>(node2->getString()) << endl;
	//evaluateNode<void*>(node2,alias,sourceContext);
	alias.setData<void*>(sourceContext.lookupVar<TArray>(node2->getString()));
	cout << "alias points to: " << alias.getData() << endl;
}

void pasteArgList(const Node* node1, const Node* node2, TetraScope& destinationContext, TetraContext& sourceContext) {
	//If there are further parameters to pass
	if(node1->kind() == NODE_FORMAL_PARAM_LIST) { //must have 2 children, as does the corresponding tree of NODE_ACTUAL_PARAM_LIST
		pasteArgList(node1->child(0),node2->child(0),destinationContext,sourceContext);
		pasteArgList(node1->child(1),node2->child(1),destinationContext,sourceContext);
	}
	else {
		assert (node1->type() != NULL);
		switch(node1->type()->getKind()) {
			case TYPE_INT:
				 paste<int>(node1,node2,destinationContext,sourceContext);
			break;
			case TYPE_REAL:
				paste<double>(node1,node2,destinationContext,sourceContext);
			break;
			case TYPE_BOOL:
				paste<bool>(node1,node2,destinationContext,sourceContext);
			break;
			case TYPE_STRING:
				paste<string>(node1,node2,destinationContext,sourceContext);
			break;
			case TYPE_VECTOR:
				//attempting to pass array
	//			cout << "Attempting to pass array" << endl;
				//Check if the array exists in memory by the fact that it has a name. If it was built on the fly (i.e. array literal) then we will have to bind it the old fashioned way
				if(node2->kind() == NODE_IDENTIFIER) {
	//				cout << "Pass by reference" << endl;
					aliasArray(node1,node2,destinationContext,sourceContext);
				}
				else {
	//				cout << "Pass by value" << endl;
					paste<TArray>(node1,node2,destinationContext,sourceContext);
				}
			break;
			default:
				cout << "Unknown NodeDataType: " << node1->type()->getKind() << " encountered when attempting to paste arguments.\nAborting...";
				exit(EXIT_FAILURE);
			}
	}
}

//calls a function, returns the return value
template<typename T>
void evaluateFunction(const Node* node, TData<T>& ret, TetraContext& context) {
	//gets the node where the body of the called funciton begins
	const Node* callNode = FunctionMap::getFunctionNode(FunctionMap::getFunctionSignature(node));

	//cout << FunctionMap::getFunctionSignature(node) << endl;

	//descNode(node);
	//descNode(callNode);	

	//check if there are parameters to be passed, and do so if needed
	//This call will have arguments if and only if the calling node has children
	//cout << "___________________________________________"<< node->numChildren() << endl;	

	if(node->child(0) != NULL) {

		//When copying arg list, we must have handles to both scopes
		//Calling scope handle
		TetraScope destScope;
	
		//new scope handle		
	
		pasteArgList(callNode->child(0),node->child(0), destScope, context);
		
		context.initializeNewScope(destScope);
	}
	else { //if there are no args, we still need to initialize a new scope!
		context.initializeNewScope();
	}

	//transfer control to the function
	evaluateNode<T>(callNode,ret,context);

	//area where the ret value is determined but all called function vars still exist	
	

	//returns to the old scope once the function has finished evaluating
	context.exitScope();
}

//evaluates operations on data types
template <typename T>
void evaluateExpression(const Node* node, TData<T>& ret, TetraContext& context) {

	//Static tables containing what actions to perform given which operation node	
	static OperationList<T> execData;

	TData<T> op1;
	evaluateNode<T>(node->child(0),op1,context);
	TData<T> op2;
	if(node->child(1) != NULL) {
		evaluateNode<T>(node->child(1),op2,context);
	}
	ret.setData(execData.execute(node->kind(),op1,op2));

}


//Simplifies a vector referenct
//Note that T is going to be a pointer type
template<typename T>
void evaluateVecRef(const Node* node, TArray* vec, TData<T>& ret, TetraContext& context) {
	TData<int> indexNum;
	//assuming a node index MUST simplify to an integer
	evaluateNode<int>(node->child(0),indexNum,context);	
	//Check if there are further idecies which must be evaluated
	if(node->numChildren() == 2){
		//Get the vector pointed to by this index
		TArray* nextVec = static_cast<TArray*>(vec->elementAt(indexNum.getData()).getData());
		evaluateVecRef<T>(node->child(1),nextVec,ret,context);
	}
	else { //If we have accounted for all the indeces
		T value = static_cast<T>(vec->elementAt(indexNum.getData()).getData());
		ret.setData(value);
	}
	
}

//used to get references to variables (for assignment, and possibly other features in the future)
template<typename T>
void evaluateAddress(const Node* node, TData<T>& ret, TetraContext& context) {
	//Right now, we are assuming that the only type of node for which this will occur is a NODE_IDENTIFIER and NODE_FORMAL_PARAM
	//If the node is a Vecref or a possible future type, then we must evaluate the LHS first
	//Must check type so we can cast the info from lookupVar to the appropriate type
	if(node->kind() == NODE_IDENTIFIER || node->kind() == NODE_FORMAL_PARAM) {

		assert (node->type() != NULL);

		switch(node->type()->getKind()) {
			case TYPE_INT:
				ret.setData(context.lookupVar<int>(node->getString()));
			break;
			case TYPE_REAL:
				ret.setData(context.lookupVar<double>(node->getString()));
			break;
			case TYPE_BOOL:
				ret.setData(context.lookupVar<bool>(node->getString()));
			break;
			case TYPE_STRING:
				ret.setData(context.lookupVar<string>(node->getString()));
			break;
			case TYPE_VECTOR:
				ret.setData(context.lookupVar<TArray>(node->getString()));
			break;
			default:
				cout << "Unknown NodeDataType: " << node->type()->getKind() << " encountered.\nAborting...";
				exit(EXIT_FAILURE);
		}
	}
	else if (node->kind() == NODE_VECREF) {
		TArray* vec = context.lookupVar<TArray>(node->child(0)->getString());
		//Note that node->shild(1) MUST be a NODE_INDEX
		//cout << "Node veccing" << endl;
		evaluateVecRef<T>(node->child(1), vec, ret, context);
	}

}

//puts the value of op2 into the address denoted by op1
//used for performing assignment
//Returns the value of the assignment, with an eye towards chaining assignments (x = y = 2)
//Assumes that node1 is a NODE_IDENTIFIER, and node2 is an appropriate value
template<typename T>
T paste(const Node* node1, const Node* node2, TetraContext& context) {

	TData<T*> op1;
	TData<T> op2;
	evaluateAddress<T*>(node1,op1,context);
	evaluateNode<T>(node2,op2,context);
	T ret = op2.getData();
	*(op1.getData()) = ret;
	return ret;
}

//This method overloads the basic paste(Node*, Node*, TetraContext*) function, except that it is made for cross-scope assignment (i.e. function calls)
//node1 is assumed to be a NODE_FORMAL_PARAM while node2 is a NODE_ACTUAL_PARAM
//Because of this, we don;t have to check for NODE_VECREF or the likes that evaluateAddress provides us with
template<typename T>
T paste(const Node* node1, const Node* node2, TetraScope& destinationContext, TetraContext& sourceContext) {
	TData<T*> op1;
	TData<T> op2;
	op1.setData(destinationContext.lookupVar<T>(node1->getString()));
//	evaluateAddress<T*>(node1,&op1,destinationContext);
	evaluateNode<T>(node2,op2,sourceContext);
	//cout << "Nodes evaluated" << endl;
	T ret = op2.getData();
	*(op1.getData()) = ret;
	return ret;
}


//At the moment, ret will point to whatever value was copied for the assignment
template<typename T>
void performAssignment(const Node* node, TData<T>& ret, TetraContext& context) {

	cout << "assigning to: " << (node->child(0)->getString())  << endl;

	assert(node->type() != NULL);

	//We must typecheck here because parent node does not know what we are trying to assign
	switch(node->type()->getKind()) {
		case TYPE_INT:
			//Performs the assignment and sets up the return
			ret.setData(paste<int>(node->child(0), node->child(1), context));
		break;
		case TYPE_REAL:
			ret.setData(paste<double>(node->child(0), node->child(1), context));
		break;
		case TYPE_BOOL:
			ret.setData(paste<bool>(node->child(0), node->child(1), context));
		break;
		case TYPE_STRING:
			ret.setData(paste<string>(node->child(0), node->child(1), context));
		break;
		case TYPE_VECTOR:
			cout << "Assigning an array" << endl;
			ret.setData(paste<TArray>(node->child(0), node->child(1), context));
			cout << "Enda array assignment" << endl;
		break;
		default:
			cout << "Unknown NodeDataType: " << node->child(0)->type()->getKind() << " encountered.\nAborting...";
			exit(EXIT_FAILURE);
	}

}
//evaluates boolean operations
template<typename T>
void evaluateCondition(const Node* node, TData<T>& ret, TetraContext& context) {

	//Static tables containing what type of comparison to perform given which operation node	
	static ComparisonList<int> compInt;
	static ComparisonList<double> compReal;
	static ComparisonList<bool> compBool;
	static ComparisonList<string> compString;
	static ComparisonList<TArray> compList;

	assert (node->child(0)->type() != NULL);

		//fetch operands, must perform type checking here, as the parent node has absolutely no idea what is being compared
	switch(node->child(0)->type()->getKind()) {
		case TYPE_INT:
		{
			TData<int> op1;
			evaluateNode<int>(node->child(0),op1,context);
			TData<int> op2;
			evaluateNode<int>(node->child(1),op2,context);
			ret.setData(compInt.execute(node->kind(),op1,op2));
		}
		break;
		case TYPE_REAL:
		{
			TData<double> op1;
			evaluateNode<double>(node->child(0),op1,context);
			TData<double> op2;
			evaluateNode<double>(node->child(1),op2,context);
			ret.setData(compReal.execute(node->kind(),op1,op2));
		}
		break;
		case TYPE_BOOL:
		{
			TData<bool> op1;
			evaluateNode<bool>(node->child(0),op1,context);
			TData<bool> op2;
			if(node->child(1) != NULL) {
				evaluateNode<bool>(node->child(1),op2,context);
			}
			ret.setData(compBool.execute(node->kind(),op1,op2));
		}
		break;
		case TYPE_STRING:
		{
			TData<string> op1;
			evaluateNode<string>(node->child(0),op1,context);
			TData<string> op2;
			evaluateNode<string>(node->child(1),op2,context);
			ret.setData(compString.execute(node->kind(),op1,op2));
		}
		break;
		case TYPE_VECTOR:
		{
			TData<TArray> op1;
			evaluateNode<TArray>(node->child(0),op1,context);
			TData<TArray> op2;
			evaluateNode<TArray>(node->child(1),op2,context);
			ret.setData(compList.execute(node->kind(),op1,op2));
		}
		break;
		default:
			std::cout << "Error: unexpected NODE_DATA_TYPES being compared" << std::endl;	
	}


}

//Takes a reference to a NODE_VECVAL, and adds it to the vector represented by TArray retVal, then recursively adds further NODE_VECVAL nodes if there are any
//From "evaluate immediate" T should be the subtype of the vector (i.e. the type of data this vector will hold)
template<typename T>
void evaluateVectorComponent(const Node* node, TetraContext& context, TArray& arrayVal) {
	//evaluate the first child node, and store its value in this array
	TData<T> x;
	//cout << "Evaluating node" << endl;
	evaluateNode<T>(node->child(0), x, context);

	//Used to insert the data into the array
	TData<void*> insertValue;

	//Used as a placeholder address to the actual value ret->getData so it can be deep copied
	//At the moment, this is criminally inefficient
	T placeHolder = x.getData();
	//cout << "Placeholder address: " << &placeHolder << endl;	
	//Use of the explicit void* is required to actually paste in the value of the pointer
	insertValue.setData<void*>(&placeHolder);
	insertValue.setDeletableType<T>();
	//cout << "Adding: (" << placeHolder << ") at address " << insertValue.getData()  << endl;
	arrayVal.addElement(insertValue);
	//cout << "after add" << endl;

	//Check for additional elements, and add them if they exist
	if(node->numChildren() == 2) {
		evaluateVectorComponent<T>(node->child(1), context, arrayVal);
	}
}


//Evaluates immediate expressions
template<typename T>
void evaluateImmediate(const Node* node, TData<T>& ret, TetraContext& context) {


	//if we need to lookup a variable, do so, Note that "lookupVar" handles if we have not seen the string before
	//However, we must check the type so that we know what to allocate if needed (optimizaiton point for later?)
	if(node->kind() == NODE_IDENTIFIER) {
		ret.setData(*(context.lookupVar<T>(node->getString())));
	}

	else {
		switch(node->kind()) {
			case NODE_INTVAL:
				ret.setData(node->getInt());
			break;
			case NODE_REALVAL:
				ret.setData(node->getReal());
			break;
			case NODE_BOOLVAL:
				ret.setData(node->getBool());
			break;
			case NODE_STRINGVAL:
				ret.setData(node->getString());
			break;
			case NODE_VECVAL:
			{
				TArray returnArray;
				//Check that array is nonempty
				if(node->numChildren() > 0) {
					switch(node->child(0)->type()->getKind()) {
						case TYPE_INT:
							evaluateVectorComponent<int>(node, context, returnArray);
							//returnArray.outputElements<int>();
						break;
						case TYPE_REAL:
							evaluateVectorComponent<double>(node, context, returnArray);
							//returnArray.outputElements<double>();
						break;
						case TYPE_BOOL:
							evaluateVectorComponent<bool>(node, context, returnArray);
							//returnArray.outputElements<bool>();
						break;
						case TYPE_STRING:
							evaluateVectorComponent<string>(node, context, returnArray);
							//returnArray.outputElements<string>();
						break;
						case TYPE_VECTOR:
							evaluateVectorComponent<TArray>(node, context, returnArray);
						break;
						default:
							cout << "Error: Attempting to initialize array with unknown subtype. Aborting..." << endl;
							exit(EXIT_FAILURE);
						
					}
				}
	//			cout << &returnArray << "<<!<!<!<!<!<!<!<!<" << endl;
	//			cout << "Switch evaluarted" << endl;
				//returnArray.outputElements();
				ret.setData(returnArray);
	//			cout << "Data set" << endl;
			}				
			break;
			default:
				cout << "Unknown Immediate NodeType: " << node->kind() << " encountered.\nAborting...";
				exit(EXIT_FAILURE);
		}
			
	}
}

template<typename T>
void evaluateFlag(const Node* node, TData<T>& ret, TetraContext& context) {
	switch(node->kind()) {
		case NODE_BREAK:
			context.notifyBreak();
		break;
		case NODE_CONTINUE:
			context.notifyContinue();
		break;
		case NODE_RETURN:
			//Check if returning a value, or nothing
			if(node->child(0) != NULL) {
				evaluateNode<T>(node->child(0),ret,context);
				//cout << "Returning: " << ret->getData() << endl;
			}
			context.notifyReturn();
		break;
		default:
			cout << "Unexpected flag type encountered: " << node->kind() << " Aborting...";
			exit(EXIT_FAILURE);
		break;
	}
}

//evaluates generic nodes
template<typename T>
void evaluateNode(const Node* node, TData<T>& ret, TetraContext& context) {
//	cout << "_-_-_-_-_-_-_-_-_-\n";
//	descNode(node);
//	cout << "_-_-_-_-_-_-_-_-_-_-" << endl;
	switch(NodeTable::classifyNode(node)) {
		case CONDITION:
			evaluateCondition<T>(node,ret,context);
		break;
		case CONTROL:
			evaluateFunction<T>(node,ret,context);
		break;
		case IMMEDIATE:
			evaluateImmediate<T>(node,ret,context);
		break;
		case OPERATION:
			evaluateExpression<T>(node,ret,context);
		break;
		case STRUCTURE:
			evaluateStatement<T>(node,ret,context);
		break;
		case ASSIGNMENT:
			performAssignment<T>(node,ret,context);
			//cout << "after assignment evaluation" << endl;
		break;
		case FLAG:
			evaluateFlag<T>(node,ret,context);
		break;
		default:
			cout << "Warnming: unexpected node kind encountered: " << node->kind() << "\n Aborting..." << endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv) {
	
	//check that the proper commands were passed
	if(argc < 2) {
		cout << "Please pass a file name!" << endl;
		return 0;
	}

	bool test = false;

	Node* tree;
	if(string(argv[1]) == "test"){
		test = true;
	}
	else {
		try {
			tree = parseFile(argv[1]);
		} catch(Error e) {
			cout << "The following error was detected in your program:\n" << e << "\nExecution aborted" <<endl;
			exit(EXIT_FAILURE);
		}
	}

	if(!test) {
		FunctionMap::build(tree);

		const Node* start = FunctionMap::getFunctionNode("main#");


		TData<int> retVal(0);

		cout << "Running " << argv[1] << "..." << endl;

		TetraContext tContext;
		tContext.initializeNewScope();


		evaluateNode<int>(start, retVal, tContext);

		tContext.exitScope();
		cout << "+------------------------------------------------" << endl;
		cout << "|Main returned: " << retVal.getData() << endl;
		cout << "+------------------------------------------------" << endl;

		cout << "Finished interpreting" << endl;
		return retVal.getData();	}
	else {
		TetraContext testContext;
		testContext.initializeNewScope();
		runTest(testContext);
		testContext.exitScope();
	}

}

void runTest(TetraContext& context) {
/* PLEASE DON"T DELETE THIS TEST CODE!
 * it will be modified for further test cases
 */

	cout << "\n Running tests..." << endl;

	DataType* intVal;
	DataType intV = TYPE_INT;
	intVal = &intV;

	DataType* realVal;
	DataType realV = TYPE_REAL; 
	realVal = &realV;

	DataType* boolVal;
	DataType boolV = TYPE_BOOL; 
	boolVal = &boolV;

	DataType* stringVal;
	DataType stringV = TYPE_STRING; 
	stringVal = &stringV;

	Node testInt(NODE_INTVAL);
	testInt.setDataType(intVal);
	testInt.setIntval(27);

	Node testInt2(NODE_INTVAL);
	testInt2.setDataType(intVal);
	testInt2.setIntval(39);

	Node testBool(NODE_BOOLVAL);
	testBool.setDataType(boolVal);
	testBool.setBoolval(true);

	Node testReal(NODE_REALVAL);
	testReal.setDataType(realVal);
	testReal.setRealval(-13.6);

	Node testString(NODE_STRINGVAL);
	testString.setDataType(stringVal);	
	testString.setStringval("It works maybe!");

	TData<int> i(0);
	evaluateNode<int>(&testInt,i,context);

	TData<string> s("");
	evaluateNode<string>(&testString,s,context);

	TData<double> d(0);
	evaluateNode<double>(&testReal,d,context);

	TData<bool> b(false);
	evaluateNode<bool>(&testBool,b,context);
	
	cout << "Bool: " << b.getData() << endl;
	cout << "Int: " << i.getData() << endl;
	cout << "Real: " << d.getData() << endl;
	cout << "String: " << s.getData() << endl;

	Node testAdd(NODE_PLUS);
	testAdd.setDataType(intVal);
	testAdd.addChild(&testInt);
	testAdd.addChild(&testInt);

	Node testAdd2(NODE_PLUS);
	testAdd2.setDataType(realVal);
	testAdd2.addChild(&testReal);
	testAdd2.addChild(&testReal);

	TData<double> a;
	evaluateNode<double>(&testAdd2,a,context);

	cout << "double + double: " << a.getData() << endl;

	Node testAddS(NODE_EQ);
	testAddS.setDataType(boolVal);
	testAddS.addChild(&testInt);
	testAddS.addChild(&testInt2);

	TData<bool> s2(false);
	evaluateNode<bool>(&testAddS,s2,context);

	cout << "27 cmp 39: " << s2.getData() << endl;

	Node varX(NODE_IDENTIFIER);
	varX.setStringval("GHRUAAAHHH");
	varX.setDataType(stringVal);
	*(context.lookupVar<string>("GHRUAAAHHH")) = "oink";

	cout << "here" << endl;

	Node assignX(NODE_ASSIGN);
	assignX.setDataType(stringVal);
	assignX.addChild(&varX);
	assignX.addChild(&testString);

	cout << "here2" << endl;
	
	evaluateNode<string>(&assignX,s,context);
	evaluateNode<string>(&varX,s,context);
	cout << "X(?): " << s.getData() << endl;
	cout << "fin" << endl;
}
