//
// Created by Lee on 2018/6/29.
// Modify time 2022年9月2日01:25:35
//

#ifndef MYCPPIMPLEMENT_MYMUTEXLOCK_H
#define MYCPPIMPLEMENT_MYMUTEXLOCK_H

#include <cstdio>

#include <pthread.h>

#include "util/noncopyable.h"

class MutexLock : noncopyable {
 public:
  MutexLock() { pthread_mutex_init(&mutex_, nullptr); }

  ~MutexLock() {
    pthread_mutex_lock(&mutex_);
    pthread_mutex_destroy(&mutex_);
  }

  void Lock() { pthread_mutex_lock(&mutex_); }

  void Unlock() { pthread_mutex_unlock(&mutex_); }

  auto Get() -> pthread_mutex_t * { return &mutex_; }

 private:
  pthread_mutex_t mutex_{};

 private:
  friend class Condition;
};

class MutexLockGuard {
 public:
  explicit MutexLockGuard(MutexLock &_mutex) : m_(_mutex) { m_.Lock(); }

  ~MutexLockGuard() { m_.Unlock(); }

 private:
  MutexLock &m_;
};

#endif  // MYCPPIMPLEMENT_MYMUTEXLOCK_H
