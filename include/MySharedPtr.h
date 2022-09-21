//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYSHAREDPTR_H
#define MYCPPIMPLEMENT_MYSHAREDPTR_H

#include <utility>

#include "MyUniquePtr.h"
#include "MyError.h"

template<typename T>
class MyUniquePtr;

template<typename T>
class MySharedPtr;

template<typename T>
class MyWeakPtr;

class MyRefCountBase {
private:
    virtual void Destroy() noexcept = 0;

    virtual void Delete_This() noexcept = 0;

    long m_use = 1;  // atomic_counter_t
    long m_weak = 1; // atomic_counter_t

protected:
    constexpr MyRefCountBase() noexcept = default;

public:
    MyRefCountBase(const MyRefCountBase &) = delete;

    MyRefCountBase &operator=(const MyRefCountBase &) = delete;

    virtual ~MyRefCountBase() noexcept = default;

    void Incref() noexcept {
        m_use++;
    }

    void Incwref() noexcept {
        m_weak++;
    }

    bool Decref() noexcept {
        bool zero_flag = false;
        if ((--m_use) == 0) {
            Destroy();
            zero_flag = Decwref();
        }
        return zero_flag;
    }

    bool Decwref() noexcept {
        if ((--m_weak) == 0) {
            Delete_This();
            return true;
        }
        return false;
    }

    [[nodiscard]] long Use_Count() const noexcept {
        return m_use;
    }
};

template<typename T>
class MyRefCount : public MyRefCountBase {
public:
    explicit MyRefCount(T *ptr) : MyRefCountBase(), m_ptr(ptr) {

    }

private:
    void Destroy() noexcept override {
        delete m_ptr;
    }

    void Delete_This() noexcept override {
        delete this;
    }

private:
    T *m_ptr;
};

// MyBaseSmartPtr Implement

template<typename T>
class MyBaseSmartPtr {
public:
    MyBaseSmartPtr(const MyBaseSmartPtr<T> &) = delete;

    MyBaseSmartPtr<T> operator=(const MyBaseSmartPtr<T> &) = delete;

protected:
    [[nodiscard]] long Use_Count() const {
        return m_refcount ? m_refcount->Use_Count() : 0;
    }

    T *Get() const {
        return this->m_ptr;
    }

    MyBaseSmartPtr() = default;

    ~MyBaseSmartPtr() = default;

    template<class T2>
    void Raw_Construct(T2 *ptr) {
        m_refcount = new MyRefCount(ptr);
        m_ptr = ptr;
    }

    template<class T2>
    void Move_Construct(MyBaseSmartPtr<T2> &&rhs) {
        this->m_ptr = rhs.m_ptr;
        this->m_refcount = rhs.m_refcount;
        rhs.m_ptr = nullptr;
        rhs.m_refcount = nullptr;
    }

    template<class T2>
    void Copy_Construct_From(const MySharedPtr<T2> &rhs) {
        rhs.Incref();
        this->m_ptr = rhs.m_ptr;
        this->m_refcount = rhs.m_refcount;
    }

    template<class T2>
    void Construct_From_Weak(const MyWeakPtr<T2> &rhs) {
        this->m_ptr = rhs.m_ptr;
        this->m_refcount = rhs.m_refcount;
        Incref();
    }

    template<class T2>
    void Weakly_Construct_From(const MyBaseSmartPtr<T2> &other) {
        if (other.m_refcount) {
            m_ptr = other.m_ptr;
            m_refcount = other.m_refcount;
            Incwref();
        } else {
            throw MyError("Construct MyWeakPtr From MySharedPtr Failed");
        }
    }

    void Swap(MyBaseSmartPtr<T> &right) {
        std::swap(this->m_ptr, right.m_ptr);
        std::swap(this->m_refcount, right.m_refcount);
    }

    void Incref() const {
        if (m_refcount) {
            m_refcount->Incref();
        }
    }

    void Decref() {
        if (m_refcount) {
            if (m_refcount->Decref()) {
                m_refcount = nullptr;
                m_ptr = nullptr;
            }
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
    MyRefCountBase *m_refcount = nullptr;
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

    explicit MySharedPtr(const MyWeakPtr<T> &wp);

    MySharedPtr &operator=(const MySharedPtr<T> &sp);

    MySharedPtr &operator=(MySharedPtr<T> &&sp) noexcept;

    MySharedPtr &operator=(MyUniquePtr<T> &&x);

    ~MySharedPtr();

    void swap(MySharedPtr<T> &x) {
        this->Swap(x);
    }

    [[nodiscard]] long use_count() const noexcept;

    T *get() const {
        return this->Get();
    }

    T &operator*() const {
        return *(this->Get());
    }

    T *operator->() const {
        return this->Get();
    }

    [[nodiscard]] bool unique() const noexcept {  // Empty pointers are never unique (as they do not own any pointers).
        return use_count() == 1;
    }

    void reset();

    void reset(T *);

    explicit operator bool();

    friend class MyWeakPtr<T>;

    friend bool operator==(const MySharedPtr<T> &lhs, const MySharedPtr<T> &rhs) {
        return lhs.get() == rhs.get();
    }

    friend bool operator!=(const MySharedPtr<T> &lhs, const MySharedPtr<T> &rhs) {
        return !(lhs == rhs); // NOLINT
    }

    friend bool operator==(const MySharedPtr<T> &lhs, const T *rhs) {
        return lhs.get() == rhs;
    }

    friend bool operator!=(const MySharedPtr<T> &lhs, const T *rhs) {
        return !(lhs == rhs); // NOLINT
    }

    friend void swap(MySharedPtr<T> &x, MySharedPtr<T> &y) {
        x.swap(y);
    }
};


template<typename T>
MySharedPtr<T>::MySharedPtr() = default;

template<typename T>
MySharedPtr<T>::MySharedPtr(T *ptr) {
    this->Raw_Construct(ptr);
}

template<typename T>
MySharedPtr<T>::MySharedPtr(const MySharedPtr<T> &sp) {
    this->Copy_Construct_From(sp);
}

template<typename T>
MySharedPtr<T>::MySharedPtr(MySharedPtr<T> &&sp) noexcept {
    this->Move_Construct(sp);
}

template<typename T>
MySharedPtr<T>::MySharedPtr(const MyWeakPtr<T> &wp) {
    this->Construct_From_Weak(wp);
}

template<typename T>
MySharedPtr<T> &MySharedPtr<T>::operator=(const MySharedPtr<T> &sp) {
    MySharedPtr<T>(sp).swap(*this);
    return *this;
}

template<typename T>
MySharedPtr<T> &MySharedPtr<T>::operator=(MySharedPtr<T> &&sp) noexcept {
    MySharedPtr<T>(std::move(sp)).swap(*this);
    return *this;
}

template<typename T>
MySharedPtr<T> &MySharedPtr<T>::operator=(MyUniquePtr<T> &&x) {
    MySharedPtr<T>(std::move(x)).swap(*this);
    return *this;
}

template<typename T>
MySharedPtr<T>::~MySharedPtr() {
    this->Decref();
}


template<typename T>
MySharedPtr<T>::operator bool() {
    return MyBaseSmartPtr<T>::Get() != nullptr;
}

template<typename T>
void MySharedPtr<T>::reset() {
    MySharedPtr<T>().swap(*this);
}

template<typename T>
void MySharedPtr<T>::reset(T *ptr) {
    MySharedPtr<T>(ptr).swap(*this);
}

template<typename T>
long MySharedPtr<T>::use_count() const noexcept {
    return this->Use_Count();
}


// MyWeakPtr Implement

template<typename T>
class MyWeakPtr final : public MyBaseSmartPtr<T> {
public:
    MyWeakPtr();

    MyWeakPtr(const MyWeakPtr<T> &x);

    MyWeakPtr(const MySharedPtr<T> &x) noexcept;

    MyWeakPtr &operator=(const MyWeakPtr<T> &x) noexcept;

    MyWeakPtr &operator=(const MySharedPtr<T> &x) noexcept;

    ~MyWeakPtr();

    [[nodiscard]] bool expired() const noexcept;

    void swap(MyWeakPtr<T> &x) noexcept;

    friend void swap(MyWeakPtr<T> &x, MyWeakPtr<T> &y) {
        x.swap(y);
    }

    MySharedPtr<T> lock() const noexcept;

    void reset() noexcept;

    [[nodiscard]] long use_count() const noexcept {
        return MyBaseSmartPtr<T>::Use_Count();
    }

};

template<typename T>
MyWeakPtr<T>::MyWeakPtr() = default;

template<typename T>
MyWeakPtr<T>::MyWeakPtr(const MyWeakPtr<T> &x) {
    this->Weakly_Construct_From(x);
}

template<typename T>
MyWeakPtr<T>::MyWeakPtr(const MySharedPtr<T> &x) noexcept {
    this->Weakly_Construct_From(x);
}

template<typename T>
MyWeakPtr<T> &MyWeakPtr<T>::operator=(const MyWeakPtr<T> &x) noexcept {
    MyWeakPtr<T>(x).swap(*this);
    return *this;
}

template<typename T>
MyWeakPtr<T> &MyWeakPtr<T>::operator=(const MySharedPtr<T> &x) noexcept {
    MyWeakPtr<T>(x).swap(*this);
    return *this;
}

template<typename T>
MyWeakPtr<T>::~MyWeakPtr() {
    this->Decwref();
}

template<typename T>
bool MyWeakPtr<T>::expired() const noexcept {
    return this->Use_Count() == 0;
}

template<typename T>
MySharedPtr<T> MyWeakPtr<T>::lock() const noexcept {
    return expired() ? MySharedPtr<T>(nullptr) : MySharedPtr<T>(*this);
}

template<typename T>
void MyWeakPtr<T>::reset() noexcept {
    MyWeakPtr<T>().swap(*this);
}

template<typename T>
void MyWeakPtr<T>::swap(MyWeakPtr<T> &x) noexcept {
    this->Swap(x);
}

// my_make_unique
template<typename T, typename ...Args>
MySharedPtr<T> my_make_shared(Args &&...args) {
    return MySharedPtr<T>(new T(std::forward<Args>(args)...)); // std::forward current not be replaced by mine
}

// #include "MySharedPtr.hpp" // 可以将实现放到这个hpp文件夹下, 需要重新配置cmakelist, 暂不使用

#endif //MYCPPIMPLEMENT_MYSHAREDPTR_H
