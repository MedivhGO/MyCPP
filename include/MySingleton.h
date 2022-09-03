//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYSINGLETON_H
#define MYCPPIMPLEMENT_MYSINGLETON_H

#include <memory>

#include "MutexLock.h"

template<typename T>
class SingleDemo {
public:
    // 如果使用raw ptr,
    // 外部调用了delete释放了singleptr指向的对象, 这里的singleptr就是一个悬空指针了.
    // 如果不调用delete, 会造成内存泄露.
    // 所以不能让外部调用delete释放空间,要自己处理避免内存泄露.
    // 所以使用std::shared_ptr进行管理, 或者写一个delInstance函数, 收到释放.
    static std::shared_ptr<T> getInstance();
private:
    SingleDemo() = default;
    SingleDemo(const SingleDemo& rhs) = default;
    SingleDemo& operator=(const SingleDemo& rhs) = default;
    static std::shared_ptr<T> singleptr; // 保证指针类型的赋值操作是原子的
    static MutexLock ml;
};

template<typename T>
std::shared_ptr<T> SingleDemo<T>::singleptr = nullptr;

template<typename T>
MutexLock SingleDemo<T>::ml;

template<typename T>
std::shared_ptr<T> SingleDemo<T>::getInstance() {
    if (singleptr == nullptr) {
        MutexLockGuard lg(ml);
        if (singleptr == nullptr) {
            singleptr = std::make_shared<T>();
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
