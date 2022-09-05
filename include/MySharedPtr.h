//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYSHAREDPTR_H
#define MYCPPIMPLEMENT_MYSHAREDPTR_H

class MyRefCount {
public:
    MyRefCount(const MyRefCount &) = delete;

    MyRefCount &operator=(const MyRefCount &) = delete;

    ~MyRefCount();

public:
    void Incref();

    void Incwref();

    void Decref();

    void Decwref();

    long UseCount();

protected:
    MyRefCount() = default;

private:
    long m_use = 1;
    long m_weak = 1;
};

MyRefCount::~MyRefCount() {

}

void MyRefCount::Incref() {

}

void MyRefCount::Incwref() {

}

void MyRefCount::Decref() {

}

void MyRefCount::Decwref() {

}

long MyRefCount::UseCount() {
    return 0;
}

template<typename T>
class MyBaseSmartPtr {
public:
    MyBaseSmartPtr(const MyBaseSmartPtr<T> &) = delete;

    MyBaseSmartPtr<T> operator=(const MyBaseSmartPtr<T> &) = delete;

    long use_count() {
        return m_refcount ? m_refcount->UseCount() : 0;
    }

protected:
    const T *get() const {
        return m_ptr;
    }

    MyBaseSmartPtr() = default;

    ~MyBaseSmartPtr() = default;

    void Incref() {
        if (m_refcount) {
            m_refcount->Incref();
        }
    }

    void Decref() {
        if (m_refcount) {
            m_refcount->Decref();
        }
    }

    void Incwref() {
        if (m_refcount) {
            m_refcount->Incwref();
        }
    }

    void Decwref() {
        if (m_refcount) {
            m_refcount->Decwref();
        }
    }

private:
    T *m_ptr = nullptr;
    MyRefCount *m_refcount = nullptr;
};

template<typename T>
class MyWeakPtr;

template<typename T>
class MySharedPtr final : public MyBaseSmartPtr<T> {
public:
    MySharedPtr();

    explicit MySharedPtr(T *);

    MySharedPtr(const MySharedPtr<T> &sp);

    MySharedPtr(MySharedPtr<T> &&sp) noexcept;

    MySharedPtr &operator=(const MySharedPtr<T> &sp);

    MySharedPtr &operator=(MySharedPtr<T> &&sp) noexcept;

    ~MySharedPtr();

public:
    const T *get() const {
        return MyBaseSmartPtr<T>::get();
    }

    const T &operator*() const {
        return *MyBaseSmartPtr<T>::get();
    }

    const T *operator->() const {
        return MyBaseSmartPtr<T>::get();
    }

    void reset();

    void reset(T *);

    explicit operator bool();

    friend class MyWeakPtr<T>;

    friend bool operator==(const MySharedPtr<T>& lhs, const MySharedPtr<T>& rhs) {
        return lhs.get() == rhs.get();
    }

    friend bool operator!=(const MySharedPtr<T>& lhs, const MySharedPtr<T>& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator==(const MySharedPtr<T>& lhs, const T* rhs) {
        return lhs.get() == rhs;
    }

    friend bool operator!=(const MySharedPtr<T>& lhs, const T* rhs) {
        return !(lhs == rhs);
    }
};


// MyWeakPtr Implement

template<typename T>
class MyWeakPtr final : public MyBaseSmartPtr<T> {
public:
    MyWeakPtr();

    [[maybe_unused]] explicit MyWeakPtr(T *);

    MyWeakPtr(MySharedPtr<T>& );

    ~MyWeakPtr();


    const T *get() const {
        return MyBaseSmartPtr<T>::get();
    }

    MySharedPtr<T> lock();

};

template<typename T>
MySharedPtr<T>::operator bool() { return MyBaseSmartPtr<T>::get() != nullptr; }

template<typename T>
void MySharedPtr<T>::reset() {

}

template<typename T>
void MySharedPtr<T>::reset(T *) {

}

template<typename T>
[[maybe_unused]] MyWeakPtr<T>::MyWeakPtr(T *) {

}

template<typename T>
MySharedPtr<T> MyWeakPtr<T>::lock() {
    return MySharedPtr<T>();
}

// my_make_unique
template<typename T, typename ...Args>
MySharedPtr<T> my_make_shared(Args &&...args) {
    return MySharedPtr<T>(new T(std::forward<Args>(args)...)); // std::forward current not be replaced by mine
}

// #include "MySharedPtr.hpp" // 可以将实现放到这个hpp文件夹下, 需要重新配置cmakelist, 暂不使用

#endif //MYCPPIMPLEMENT_MYSHAREDPTR_H
