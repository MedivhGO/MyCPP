//
// Created by Lee on 2018/6/29.
// Modify time 2022年9月2日01:25:35
//

#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include <cstdio>

#include <pthread.h>

#include "noncopyable.h"

class MutexLock : noncopyable {
public:
    MutexLock() {
        pthread_mutex_init(&mutex, NULL);
    }

    ~MutexLock() {
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }

    void lock() {
        pthread_mutex_lock(&mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_t *get() {
        return &mutex;
    }

private:
    pthread_mutex_t mutex;

private:
    friend class Condition;
};

class MutexLockGuard {
public:
    explicit MutexLockGuard(MutexLock &_mutex) : m(_mutex) {
        m.lock();
    }

    ~MutexLockGuard() {
        m.unlock();
    }

private:
    MutexLock &m;
};

#endif //MUTEXLOCK_H
