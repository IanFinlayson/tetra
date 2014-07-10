#include<string>
#include<iostream>
#include"frontend.hpp"
#include"tData.h"
#include"progContext.h"
#include <time.h>

#include<pthread.h>
#include"threadEnvironment.h"

#include<assert.h>

using namespace std;//This will eventually be removed

//This class wraps the arguments which must be passed to an evaluateNode statment so they can be passed as arguments to spawned threads
template <typename T>
struct evalArgs {
	const Node* node;
	TData<T>& ret;
	scope_ptr scope;

	evalArgs(const Node* pNode, TData<T>& pRet, scope_ptr pScope) :
		node(pNode), ret(pRet), scope(pScope)
	{
		//do nothing
	}
};

template<typename T>
void wrapEvaluation(void* args) {
	cout << "Thread Start time: " << time(0) << endl;
	evalArgs<T>& argList = *static_cast<evalArgs<T>*>(args);

	//Give the thread its own call stack
	TetraContext contextCopy;
	contextCopy.branchOff(argList.scope);
	
	//Temporary error notificaiton
	cout << "Thread starting execution time: " << time(0) << endl;
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

	delete static_cast< evalArgs<T>* >(args);
	cout << "Thread finished: " << time(0) << endl;
	pthread_exit(NULL);
}

//Spawns a thread to execute a given node by branching off the given context
template <typename T>
pthread_t spawnThread(Node* node, TData<T>& ret, TetraContext& context) {
	pthread_t newThread;
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	evalArgs<T>* args = new evalArgs<T>(node,ret,context.getScopeRef());
	pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
	int success = pthread_create(&newThread, &attributes,(void*(*)(void*))wrapEvaluation<T>,(void*)(args));
	assert(success == 0);//For now, we will assume that thread creations are correct
	return newThread;
}

template <typename T>
void evaluateParallel(const Node* node, TData<T>& ret, TetraContext& context) {

	switch(node->kind()) {
		case NODE_PARFOR:
		{
			cout << "PARFOR" << endl;
  /*                      //Obtain the list of elements we need to loop over
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
*/
//			cout << "Have collection of size: " << collection.getData()->size() << endl;
		}
		break;
		case NODE_PARALLEL:
		{
			context.notifyParallel();
			context.setupParallel();
			evaluateNode(node->child(0),ret,context);

			//Join with all other threads
			context.endParallel();

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
