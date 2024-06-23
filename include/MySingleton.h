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
    // 如果使用 raw ptr,
    // 外部调用了 delete  释放了 single_ptr 指向的对象, 这里的 single_ptr 就是一个悬空指针了.
    // 如果不调用 delete, 会造成内存泄露.
    // 所以不能让外部调用 delete 释放空间,要自己处理避免内存泄露.
    // 所以使用 std::shared_ptr 进行管理, 或者写一个 delInstance 函数, 手动调用释放.
    static std::shared_ptr<T> getInstance();

private:
    SingleDemo() = default;

    SingleDemo(const SingleDemo &rhs) = default;

    SingleDemo &operator=(const SingleDemo &rhs) = default;

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
template<typename T>
class Singleton : private T {
private:
    Singleton() = default;

    ~Singleton() = default;

public:
    static T &getInstance();
};

template<typename T>
T &Singleton<T>::getInstance() {
    static T s_oT;
    return s_oT;
}

// implement3
template<typename T>
class OnceSingle {
public:
    OnceSingle() = delete;

    OnceSingle(const OnceSingle<T> &m) = delete;

    OnceSingle &operator=(const OnceSingle<T> &m) = delete;

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

    static T *getInstance() {
        std::call_once(s_flag, InitPtr);
        return m_ptr;
    }

private:
    static void InitPtr() {
        m_ptr = new T();
        static CGFunctionClass cg;
    }

private:
    static T *m_ptr;
    static std::once_flag s_flag;
};

template<typename T>
T *OnceSingle<T>::m_ptr = nullptr;

template<typename T>
std::once_flag OnceSingle<T>::s_flag;

// implement4
template<typename T>
class OnceSingleWithArgs final {
public:
    OnceSingleWithArgs() = delete;

    OnceSingleWithArgs(const OnceSingle<T> &m) = delete;

    OnceSingleWithArgs &operator=(const OnceSingle<T> &m) = delete;

    ~OnceSingleWithArgs() = default;

    class CGFunctionClass {
    public:
        ~CGFunctionClass() {
            if (m_ptr != nullptr) {
                delete m_ptr;
                m_ptr = nullptr;
            }
        }
    };

    template<typename... Args>
    static T *getInstance(Args &&... args) {
        std::call_once(s_flag, InitPtr < T > , std::forward<Args>(args)...);
        return m_ptr;
    }

private:
    template<typename... Args>
    static void InitPtr(Args &&... args) {
        m_ptr = new T(args...);
        static CGFunctionClass cg;
    }

private:
    static T *m_ptr;
    static std::once_flag s_flag;
};

template<typename T>
T *OnceSingleWithArgs<T>::m_ptr = nullptr;

template<typename T>
std::once_flag OnceSingleWithArgs<T>::s_flag;

// implement5
template<typename T>
class SingletonAtom {
private:
    static std::atomic<T *> instance;
    static std::mutex mutex;

    SingletonAtom() {}

public:
    class CGFunctionClass {
    public:
        ~CGFunctionClass() {
            if (instance != nullptr) {
                delete instance;
                instance = nullptr;
            }
        }
    };
    static T *getInstance() {
        if (instance.load(std::memory_order_relaxed) == nullptr) {
            std::lock_guard<std::mutex> lock(mutex);
            if (instance.load(std::memory_order_relaxed) == nullptr) {
                instance.store(new T(), std::memory_order_relaxed);
                static CGFunctionClass cg; // 避免内存泄露
            }
        }
        return instance.load(std::memory_order_acquire);
    }
};

template<typename T>
std::atomic<T *> SingletonAtom<T>::instance(nullptr);

template<typename T>
std::mutex SingletonAtom<T>::mutex;

// std::atomic::store 是 C++ 标准库中原子操作的一部分，用于在多线程环境中安全地存储（写入）数据到一个原子变量中

// std::memory_order_relaxed 表示原子操作不需要任何内存序的约束。
// 也就是说，使用这个选项的原子操作不会对其他线程中的内存操作产生任何同步效果
// 这通常用于性能敏感的代码中，因为不进行同步可以减少开销，但同时也要求程序员自己确保操作的安全性。

// std::memory_order_acquire 这个选项主要用于同步操作，确保在当前线程中
// 对该共享数据的读取操作（load）在所有之前的内存写入操作（store）之后发生
#endif //MYCPPIMPLEMENT_MYSINGLETON_H
