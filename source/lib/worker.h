/* worker.h
 * this class defines a worker thread object using Qt */

#ifndef PARALLEL_WORKER
#define PARALLEL_WORKER

#include <QThread>

#include "node.h"
#include "environment.h"
#include "eval.h"

/* dummy class for having statement nodes evaluated in separate threads */
class ParallelWorker : public QThread {
    Q_OBJECT

   public:
    ParallelWorker(Node* node, Context* context) {
        /* store the node to evaluate it once we start */
        this->node = node;

        /* make a copy of the context */
        this->context = new Context(*context);

        /* assume no return */
        this->returnValue = NULL;

        /* get a new thread id */
        this->threadId = Environment::getNextThreadId();
    }

    virtual ~ParallelWorker() {
        /* remove the context clone */
        delete context;
    }

    unsigned int getThreadId() const {
        return threadId;
    }

    /* this function is called in the new thread */
    void run() Q_DECL_OVERRIDE {
        /* run evaluate our node, and save the return value */
        returnValue = evaluateStatement(node, context, threadId);
    }

    /* provides the return value of the node we run */
    Data* getReturn() const {
        return returnValue;
    }

   private:
    Node* node;
    Context* context;
    Data* returnValue;
    unsigned int threadId;
};

#endif
