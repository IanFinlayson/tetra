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
            this->node = node;
            this->context = context;
        }

        /* this function is called in the new thread */
        void run() Q_DECL_OVERRIDE {
            returnValue = evaluateStatement(node, context);
        }

        virtual ~ParallelWorker() {}

    private:
        Node* node;
        Context* context;
        Data* returnValue;
};

#endif

