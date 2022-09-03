//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYSINGLETON_H
#define MYCPPIMPLEMENT_MYSINGLETON_H

#include "MutexLock.h"

template<typename T>
class SingleDemo {
public:
    static T* getInstance();
private:
    SingleDemo() = default;
    SingleDemo(const SingleDemo& rhs) = default;
    SingleDemo& operator=(const SingleDemo& rhs) = default;
    static T* singleptr; // 保证指针类型的赋值操作是原子的
    static MutexLock ml;
};

template<typename T>
T* SingleDemo<T>::singleptr = nullptr;

template<typename T>
MutexLock SingleDemo<T>::ml;

template<typename T>
T* SingleDemo<T>::getInstance() {
    if (singleptr == nullptr) {
        MutexLockGuard lg(ml);
        if (singleptr == nullptr) {
            singleptr = new T;
        }
    }
    return singleptr;
}

template <typename T>
class Singleton : private T {
private:
    Singleton() = default;
    ~Singleton() = default;
public:
    static T& getInstance();
};

template<typename T>
T &Singleton<T>::getInstance() {
    static T s_oT;
    return s_oT;
}

#endif //MYCPPIMPLEMENT_MYSINGLETON_H
