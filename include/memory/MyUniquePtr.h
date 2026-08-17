//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYUNIQUEPTR_H
#define MYCPPIMPLEMENT_MYUNIQUEPTR_H

#include <utility>

template<typename T>
class MyUniquePtr final {
public:
    MyUniquePtr();

    explicit MyUniquePtr(T *);

    MyUniquePtr(const MyUniquePtr &) = delete;

    MyUniquePtr(MyUniquePtr &&) noexcept;

    MyUniquePtr &operator=(const MyUniquePtr &) = delete;

    MyUniquePtr &operator=(MyUniquePtr &&) noexcept ;

    ~MyUniquePtr();

    T *get() const;

    T& operator*() const;

    T *operator->() const;

    T *release();

    void reset(T *);

    void reset();

    explicit operator bool() const; // Cast UniquePtr to bool

private:
    T *m_ptr;
};

template<typename T>
MyUniquePtr<T>::MyUniquePtr() : m_ptr(nullptr) {}

template<typename T>
MyUniquePtr<T>::MyUniquePtr(T *ptr) : m_ptr(ptr) {}

template<typename T>
MyUniquePtr<T>::MyUniquePtr(MyUniquePtr &&rhs) noexcept {
    m_ptr = rhs.release();
}

template<typename T>
MyUniquePtr<T>::~MyUniquePtr() {
    delete m_ptr;
    m_ptr = nullptr;
}

template<typename T>
MyUniquePtr<T> &MyUniquePtr<T>::operator=(MyUniquePtr<T> &&rhs) noexcept {
    if (this == &rhs) {
        return *this;
    }
    m_ptr = rhs.release();
    return *this;
}

template<typename T>
T &MyUniquePtr<T>::operator*() const {
    return *m_ptr;
}

template<typename T>
T *MyUniquePtr<T>::get() const {
    return m_ptr;
}

template<typename T>
T *MyUniquePtr<T>::release() {
    T *ret = m_ptr;
    m_ptr = nullptr;
    return ret;
}

template<typename T>
T *MyUniquePtr<T>::operator->() const{
    return m_ptr;
}

template<typename T>
void MyUniquePtr<T>::reset(T *ptr) {
    delete m_ptr; // delete nullptr no effect
    m_ptr = ptr;
}

template<typename T>
void MyUniquePtr<T>::reset() {
    delete m_ptr;
    m_ptr = nullptr;
}

template<typename T>
MyUniquePtr<T>::operator bool() const {
    return (m_ptr);
}

// my_make_unique

template<typename T, typename ...Args>
MyUniquePtr<T> my_make_unique(Args &&...args) { // 返回一个右值
    return MyUniquePtr<T>(new T(std::forward<Args>(args)...)); // std::forward current not be replaced by mine
}

#endif //MYCPPIMPLEMENT_MYUNIQUEPTR_H
