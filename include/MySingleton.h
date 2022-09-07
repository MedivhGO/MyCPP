//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYSINGLETON_H
#define MYCPPIMPLEMENT_MYSINGLETON_H

#include <memory>
#include <mutex>

#include "MutexLock.h"

// implement1
template<typename T>
class SingleDemo {
public:
    // 如果使用raw ptr,
    // 外部调用了delete释放了single_ptr指向的对象, 这里的single_ptr就是一个悬空指针了.
    // 如果不调用delete, 会造成内存泄露.
    // 所以不能让外部调用delete释放空间,要自己处理避免内存泄露.
    // 所以使用std::shared_ptr进行管理, 或者写一个delInstance函数, 手动调用释放.
    static std::shared_ptr<T> getInstance();
private:
    SingleDemo() = default;
    SingleDemo(const SingleDemo& rhs) = default;
    SingleDemo& operator=(const SingleDemo& rhs) = default;
    static std::shared_ptr<T> single_ptr; // 保证指针类型的赋值操作是原子的
    static MutexLock ml;
};

template<typename T>
std::shared_ptr<T> SingleDemo<T>::single_ptr = nullptr;

template<typename T>
MutexLock SingleDemo<T>::ml;

template<typename T>
std::shared_ptr<T> SingleDemo<T>::getInstance() {
    if (single_ptr == nullptr) {
        MutexLockGuard lg(ml);
        if (single_ptr == nullptr) {
            single_ptr = std::make_shared<T>();
        }
    }
    return single_ptr;
}

// implement2
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

// implement3
template <typename T>
class OnceSingle {
public:
    OnceSingle() = delete;
    OnceSingle(const OnceSingle<T>& m) = delete;
    OnceSingle& operator=(const OnceSingle<T>& m) = delete;

    ~OnceSingle() = default;
    class CGFunctionClass {
        public:
            ~CGFunctionClass() {
                if (m_ptr != nullptr) {
                    delete m_ptr;
                    m_ptr = nullptr;
                }
            }
    };

    static T* getInstance() {
        std::call_once(s_flag, InitPtr);
        return m_ptr;
    }
private:
    static void InitPtr() {
        m_ptr = new T();
        static CGFunctionClass cg;
    }

private:
    static T* m_ptr;
    static std::once_flag s_flag;
};

template<typename T>
T* OnceSingle<T>::m_ptr = nullptr;

template<typename T>
std::once_flag OnceSingle<T>::s_flag;
#endif //MYCPPIMPLEMENT_MYSINGLETON_H
