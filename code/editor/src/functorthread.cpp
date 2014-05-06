#include "functorthread.h"

QMutex                FunctorThread::mutex;
QList<FunctorThread*> FunctorThread::functor_threads;
