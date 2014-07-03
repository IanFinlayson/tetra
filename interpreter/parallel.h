#include<string>
#include<iostream>
#include"frontend.hpp"
#include"tData.h"
#include"progContext.h"

#include<pthread.h>
#include"threadEnvironment.h"

#include<assert.h>

using namespace std;//This will eventually be removed

//This class wraps the arguments which must be passed to an evaluateNode statment so they can be passed as arguments to spawned threads
template <typename T>
struct evalArgs {
	const Node* node;
	TData<T>& ret;
	TetraContext& context;
	pthread_t* thisThread;

	evalArgs(const Node* pNode, TData<T>& pRet, TetraContext& pContext, pthread_t* thready) :
		node(pNode), ret(pRet), context(pContext), thisThread(thready)
	{
		//do nothing
	}
};

template<typename T>
void wrapEvaluation(void* args) {
	evalArgs<T>& argList = *static_cast<evalArgs<T>*>(args);
	TetraContext contextCopy;
	contextCopy.branchOff(argList.context);
	ThreadEnvironment::addThread(pthread_self());
	//Temporary error notificaiton
	try {
		//Go into execution with a normalized status
		argList.context.normalizeStatus();	
		evaluateNode(argList.node, argList.ret, contextCopy);
	}
	catch (...) {
		cout << "Error encountered" << endl;
	}

	//Although the memory is allocated in a different thread, that thread might terminate before the new thread
	//As a result, it must be deleted when this thread terminates
	//This will need to be changed later to handle when this thread terminates unexpectedly
	
	ThreadEnvironment::removeThread(pthread_self());

	delete static_cast< evalArgs<T>* >(args);
	pthread_exit(NULL);
}

template <typename T>
void evaluateParallel(const Node* node, TData<T>& ret, TetraContext& context) {

	switch(node->kind()) {
		case NODE_PARFOR:
		{
			cout << "PARFOR" << endl;
		}
		break;
		case NODE_PARALLEL:
		{
			cout << "Parallel" << endl;
		}
		break;
		case NODE_BACKGROUND:
		{
			pthread_t backgroundThread;
			pthread_attr_t attributes;
			pthread_attr_init(&attributes);

			evalArgs<T>* args = new evalArgs<T>(node->child(0),ret,context, &backgroundThread);
			pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
			int success = pthread_create(&backgroundThread, &attributes,(void*(*)(void*))wrapEvaluation<T>,(void*)(args));
			assert(success == 0);//For now, we will assume that thread creations are correct

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
