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
        pthread_mutex_init(&mutex_, NULL);
    }

    ~MutexLock() {
        pthread_mutex_lock(&mutex_);
        pthread_mutex_destroy(&mutex_);
    }

    void Lock() {
        pthread_mutex_lock(&mutex_);
    }

    void Unlock() {
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t *Get() {
        return &mutex_;
    }

private:
    pthread_mutex_t mutex_{};

private:
    friend class Condition;
};

class MutexLockGuard {
public:
    explicit MutexLockGuard(MutexLock &_mutex) : m_(_mutex) {
        m_.Lock();
    }

    ~MutexLockGuard() {
        m_.Unlock();
    }

private:
    MutexLock &m_;
};

#endif //MUTEXLOCK_H
