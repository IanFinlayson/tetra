#include<string>
#include<iostream>
#include"frontend.hpp"
#include"backend.hpp"
/*#include"tData.h"
#include"tArray.h"
#include"progContext.h"
#include"variableContext.h"*/
#include <time.h>
#include<list>

#include<pthread.h>
//#include"threadEnvironment.h"



#include<assert.h>

using namespace std;//This will eventually be removed

/*
 * This class contains calls to parallel directives.
 * Although the methods are presently rtemplated, becuase it is an error to return something from a parallel segment
 * these methods could technically be untemplated and just have an int thrown as the datatype for the basic evaluateNode
 */


//Forward declare evaluateAddress
template<typename T>
void evaluateAddress(const Node*, TData<T>&, TetraContext& context);

void dummy(void* args) {

}

//This class wraps the arguments which must be passed to an evaluateNode statment so they can be passed as arguments to spawned threads
template <typename T>
struct evalArgs {
	const Node* node;
	TData<T>& ret;
	scope_ptr scope;
	scope_ptr* globalScope;

	evalArgs(const Node* pNode, TData<T>& pRet, scope_ptr pScope, scope_ptr* pGlobal) :
		node(pNode), ret(pRet), scope(pScope), globalScope(pGlobal)
	{
		//do nothing
	}
};

//This class wraps the arguments that must be passed to a worker thread for a parallel for loop
template <typename T>
struct evalForArgs {
	evalArgs<T>* args_ptr;
	const string* varName_ptr;
	pthread_mutex_t* count_mutex_ptr;
	int* countVal_ptr;
	TArray* values_ptr;

	evalForArgs(evalArgs<T>* pArgs, const string* pName, pthread_mutex_t* pMutex, int* pCount, TArray* pValues) :
		args_ptr(pArgs), varName_ptr(pName), count_mutex_ptr(pMutex), countVal_ptr(pCount), values_ptr(pValues)
	{
		//do nothing
	}
};

//New thread executes a tetra statement
template<typename T>
void wrapEvaluation(void* args) {
	//cout << "Thread Start time: " << time(0) << endl;
	evalArgs<T>& argList = *static_cast<evalArgs<T>*>(args);

	//Give the thread its own call stack
	TetraContext contextCopy(TetraEnvironment::obtainNewThreadID());
	contextCopy.branchOff(argList.scope, argList.globalScope);
//	cout << "One time thread start: " << contextCopy.getThreadID();	
	//Temporary error notificaiton
	//cout << "Thread starting execution time: " << time(0) << endl;

	//If debugging is enabled, notify the debugger a thread is under construction
	if(TetraEnvironment::isDebugMode()) {
		//std::cout << "Parallel for-loop thread created: " << contextCopy.getThreadID();
		TetraEnvironment::getObserver().threadCreated_E(contextCopy.getThreadID(), contextCopy);
		contextCopy.setRunStatus(RUNNING);
	}

	try {
		//Go into execution with a normalized status
		contextCopy.normalizeStatus();	
		evaluateNode<T>(argList.node, argList.ret, contextCopy);
	}
	catch(Error e) {
		cout << "The following error was encountered while executing a thread:" << endl;
		contextCopy.printStackTrace();
		
		cout << e << endl;
	}
	catch(std::exception e) {
		cout << "A standard exception was encountered while executing a thread: " << endl;
		cout << e.what() << endl; 
	}
	catch (...) {
		cout << "Unspecified error encountered in thread execution" << endl;
	}

	//Although the memory is allocated in a different thread, that thread might terminate before the new thread
	//As a result, it must be deleted when this thread terminates
	//This will need to be changed later to handle when this thread terminates unexpectedly
	
	//ThreadEnvironment::removeThread(pthread_self());

	if(TetraEnvironment::isDebugMode()) {
		TetraEnvironment::getObserver().threadDestroyed_E(contextCopy.getThreadID());
	}

	delete static_cast< evalArgs<T>* >(args);
//	cout << "Thread finished: " << time(0) << endl;
	//pthread_exit(NULL);
}

//This function is executed by parallel for worker threads to repeatedly perform a given task as per a for loop
template <typename T>
void wrapMultiEvaluation(void* args) {

	evalForArgs<T>& argList = *static_cast<evalForArgs<T>* >(args);	


	//Initializes a call stack for this thread, having as its base a pointer
	//to the current call stack 
	//(i.e. the stack frame that initialized this thread)
	TetraContext contextCopy(TetraEnvironment::obtainNewThreadID());
	contextCopy.branchOff(argList.args_ptr->scope, argList.args_ptr->globalScope);	
	
	//If debugging is enabled, notify the debugger a thread is under construction
	if(TetraEnvironment::isDebugMode()) {
		//std::cout << "Parallel for-loop thread created: " << contextCopy.getThreadID();
		TetraEnvironment::getObserver().threadCreated_E(contextCopy.getThreadID(), contextCopy);
		contextCopy.setRunStatus(RUNNING);
	}
	
	//int arraySize = argList.values_ptr->size();

	pthread_mutex_lock(argList.count_mutex_ptr);

	//possible optimization: if the array willdefinitely not change size during the parForLoop execution, then we can pre-calculate the size once
	while(*(argList.countVal_ptr) < /*arraySize*/argList.values_ptr->size()) {


		//TODO fix race condition as of 11/10/14
		//This is expected to be a parallel variable
		*(argList.args_ptr->scope->template lookupVar<T>(*(argList.varName_ptr))) = *static_cast<T*>((argList.values_ptr)->elementAt(*(argList.countVal_ptr)).getData());

		//string x = *(argList.varName_ptr
		//T* temp = argList.args_->ptr->scope->template lookupVar<T>(namer);

		
		(*(argList.countVal_ptr))++;

		//cout << "Thread Val: " << *argList.countVal_ptr << endl;
		
		//TODO put this back in
		pthread_mutex_unlock(argList.count_mutex_ptr);

	

		try {
			//Executes the for loop body
			//wrapEvaluation<T>(argList.args_ptr);
			contextCopy.normalizeStatus();	
			evaluateNode<T>(argList.args_ptr->node, argList.args_ptr->ret, contextCopy);		}
		catch (Error e) {
			cout << "Error in parallel for: " << e << endl;
		}
		catch (...) {
			cout << "Unspecified error encountered" << endl;
		}

		//Lock the mutex before checking the condiiton again and overwriting the value
		////TODO put this back in
		pthread_mutex_lock(argList.count_mutex_ptr);

		//If the partallel for loopencountered a break statement, make it so that theother threads will stop on the next go-around
		if(contextCopy.queryExecutionStatus() == BREAK) {
			//Now the while condition will be false for all threads
			*(argList.countVal_ptr) = argList.values_ptr->size();
		}
	}

	//Release the mutex obtained in the while loop
	pthread_mutex_unlock(argList.count_mutex_ptr);

	//If in debug mode, notify that the thread has is about to be destroyed
	if(TetraEnvironment::isDebugMode()) {
		TetraEnvironment::getObserver().threadDestroyed_E(contextCopy.getThreadID());
	}

	//Delete the memory allocated for the arguments of this thread
	//Note that the args needed for execution (argList.args_ptr) get deleted in wrapEvaluation
	delete argList.args_ptr;
	delete &argList;
	//cout << "parfor finished" << endl;
}

//Spawns a worker thread that iwll work on iterations of a parFor loop
//The varName must be passed so the thread can initialize its own copy of the variable
template <typename T>
pthread_t spawnWorker(const Node* node, TData<T>& ret, TetraContext& context,
		 const string& varName, TArray* loopValues, int* nextJob, pthread_mutex_t* nextJob_mutex) {
	pthread_t newThread;
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	evalArgs<T>* execArgs = new evalArgs<T>(node,ret,context.getScopeRef(), &(context.getGlobalScopeRef()));
	evalForArgs<T>* args = new evalForArgs<T>(execArgs, &varName, nextJob_mutex, nextJob, loopValues);
	pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);

	int success = pthread_create(&newThread, &attributes,(void*(*)(void*))wrapMultiEvaluation<T>,(void*)(args));
	assert(success == 0);//For now, we will assume that thread creations are correct
	return newThread;
}

//Spawns a thread to execute a given node by branching off the given context
template <typename T>
pthread_t spawnThread(Node* node, TData<T>& ret, TetraContext& context) {
	pthread_t newThread;
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	evalArgs<T>* args = new evalArgs<T>(node,ret,context.getScopeRef(), &(context.getGlobalScopeRef()));
	pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
	//cout <<"!!\n"<<endl;
	int success = pthread_create(&newThread, &attributes,(void*(*)(void*))wrapEvaluation<T>,(void*)(args));
	assert(success == 0);//For now, we will assume that thread creations are correct
	return newThread;
}

template <typename T>
void evaluateParallel(const Node* node, TData<T>& ret, TetraContext& context) {

	switch(node->kind()) {
		case NODE_PARFOR:
		{
                        //Obtain the list of elements we need to loop over
                        TData<TArray> actualCollection;//If the collection does not exist yet, then we will need to place it in memory
                        TData<TArray*> collection;

                        //Check to see if the array can be evaluated by reference, or see if it is a literal that must be evaluated before we can use it
                        if(node->child(1)->kind() == NODE_IDENTIFIER || node->child(1)->kind() == NODE_VECREF) {
                                //Put the address of the addressable vector into collection
                                evaluateAddress<TArray*>(node->child(1), collection, context);
                                //aliasArray(node->child(0), collection.getData(), context);
                        }
                        else {
                                //If we must evaluate by value, we need to evaluate what we are iterating over
                                evaluateNode<TArray>(node->child(1),actualCollection,context);

                                //Set collection to point to where the actual collection is in memory
                                TArray* collection_ptr = &actualCollection.getData();
                                collection.setData<TArray*>(collection_ptr);
                        }

			const int NUM_THREADS = TetraEnvironment::getMaxThreads();

			std::vector<pthread_t> workers;

			//Note that dataqueue is a handle to the actual array in the Variable Table
			std::list<std::pair<pthread_t, TData<void*> > > dataQueue = context.declareThreadSpecificVariable(node->child(0)->getString());

			//The debugger must be informed that we are using threadSpecific variables
			if(TetraEnvironment::isDebugMode()) {
				TetraEnvironment::getObserver().notifyThreadSpecificVariable_E(node->child(0)->getString());
				context.registerParallelForVariable(node->child(0)->getString());
			}

			int currentIteration = 0;//Used to determine which iteratio nshould be tackled next
			pthread_mutex_t iter_mutex;
			pthread_mutex_init(&iter_mutex, NULL);
			
			//Locking this mutex here may in fact be unnecessary
			//However, helgrind complains if this is not here, and this thread onl;y holds the lock until all the threads are created
			//TODO: confirm whther this lock is necessary
			pthread_mutex_lock(&iter_mutex);

			for(int x = 0; x < NUM_THREADS && x < collection.getData()->size(); x++) {
				pthread_t temp;
				workers.push_back(temp);
				//cout << "worker before: " << workers[x] << endl;
				switch(node->child(0)->type()->getKind()) {
				case TYPE_INT:
				{
					TData<int> stub;
					workers[x] = spawnWorker<int>(node->child(2), stub, context, node->child(0)->getString(), collection.getData(), &currentIteration, &iter_mutex);
				}
				break;
				case TYPE_REAL:
				{
					TData<double> stub;
					workers[x] = spawnWorker<double>(node->child(2), stub, context, node->child(0)->getString(), collection.getData(), &currentIteration, &iter_mutex);
				}
				break;
				case TYPE_BOOL:
				{
					TData<bool> stub;
					workers[x] = spawnWorker<bool>(node->child(2), stub, context, node->child(0)->getString(), collection.getData(), &currentIteration, &iter_mutex);
				}
				break;
				case TYPE_STRING:
				{
					TData<std::string> stub;
					workers[x] = spawnWorker<std::string>(node->child(2), stub, context, node->child(0)->getString(), collection.getData(), &currentIteration, &iter_mutex);
				}
				break;
				case TYPE_VECTOR:
				{
					TData<TArray> stub;
					workers[x] = spawnWorker<TArray>(node->child(2), stub, context, node->child(0)->getString(), collection.getData(), &currentIteration, &iter_mutex);
				}
				break;
				default:
					std::stringstream message;
					message << "Error attempting to deduce type for parallel for. ID: " << node->child(0)->type()->getKind();
					Error e(message.str(),node->getLine());
					pthread_mutex_unlock(&iter_mutex);
					throw e;
				//cout << "worker after: " << workers[x] << endl;
				}
			}

			pthread_mutex_unlock(&iter_mutex);

			//If debugging, set this thread to waiting
			if(TetraEnvironment::isDebugMode()) {
				context.setRunStatus(WAITING);
			}

			//Wait for all the worker threads to terminate
			for(int index = 0; index < NUM_THREADS && index < collection.getData()->size(); index++) {
				pthread_join(workers[index], NULL);
			}
			//If debugging, reset to running!
			if(TetraEnvironment::isDebugMode()) {
				context.setRunStatus(RUNNING);	
			}

		}
		break;
		case NODE_PARALLEL:
		{
			context.notifyParallel();
			context.setupParallel();
			evaluateNode(node->child(0),ret,context);

			//If we are in debug mode, set status to waiting
			if(TetraEnvironment::isDebugMode()) {
				context.setRunStatus(WAITING);
			}

			//Join with all other threads
			context.endParallel();
			
			//If debug mode, return status to running
			if(TetraEnvironment::isDebugMode()) {
				context.setRunStatus(RUNNING);
			}

			//Note that it is an error to encounter some type of flag statement within a parallel block
			//i.e. breaks/continues/returns will be ignored if their change in control would bring them outside the parallel block
			context.normalizeStatus();
		}
		break;
		case NODE_BACKGROUND:
		{
			pthread_t newThread = spawnThread(node->child(0), ret, context);

			ThreadEnvironment::addThread(newThread);
		}
		break;
		case NODE_LOCK:
		{
			//Get the mutex associated with this lock. or create a new one if it does not exist
			pthread_mutex_t* lock = ThreadEnvironment::identifyMutex(node->child(0)->getString());
			pthread_mutex_lock(lock);
			
			//Execute the commands under the safety of the lock
			evaluateNode<T>(node->child(1), ret, context);

			pthread_mutex_unlock(lock);
		}
		break;
		default:
			cout << "Unsupported parallel nodekind encountered" << endl;
	}

	return;
}
