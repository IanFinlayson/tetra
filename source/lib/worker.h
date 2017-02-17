/* worker.h
 * this class defines a worker thread object using Qt */

#ifndef PARALLEL_WORKER
#define PARALLEL_WORKER

#include <QThread>

#include "node.h"
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
    }

    /* this function is called in the new thread */
    void run() Q_DECL_OVERRIDE {
        /* run evaluate our node, and save the return value */
        returnValue = evaluateStatement(node, context);

        /* remove the context clone */
        delete context;
    }

    /* provides the return value of the node we run */
    Data* getReturn() const {
        return returnValue;
    }

    virtual ~ParallelWorker() {}

   private:
    Node* node;
    Context* context;
    Data* returnValue;
};

#endif
