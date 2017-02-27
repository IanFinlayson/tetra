/* task.h
 * the Task class represents a tetra task object */

#ifndef TASK_H
#define TASK_H

#include "worker.h"
#include "values.h"

class Task : public Value {
   public:
    Task() {}

    /* set the worker this task is employing */
    void setWorker(ParallelWorker* worker) {
        this->worker = worker;
    }

    /* wait for this task to finish up */
    void wait() {
        worker->wait();
    }

    virtual String toString() const {
        return "<task>";
    }

    /* mutexes cannot really be copied */
    virtual void copyValue(const Value& other) {
        (void) (other);
        throw Error("Cannot copy a task object!");
    }

   private:
    ParallelWorker* worker;
};

#endif
