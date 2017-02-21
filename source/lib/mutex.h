/* mutex.h
 * the Mutex class represents a tetra mutex object, it is implemented in terms of
 * the QMutex class */

#ifndef MUTEX_H
#define MUTEX_H

#include <QMutex>
#include "values.h"

class Mutex : public Value {
   public:
    Mutex() {}

    /* just lock or unlock the underlying mutex */
    void lock() {
        mut.lock();
    }
    void unlock() {
        mut.unlock();
    }

    virtual String toString() const {
        return "<mutex>";
    }

    /* mutexes cannot really be copied */
    virtual void copyValue(const Value& other) {
        (void) (other);
        throw Error("Cannot copy a mutex object!");
    }

   private:
    QMutex mut;
};

#endif
