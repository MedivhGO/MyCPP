//
// Created by Lee on 2022/9/1.
//

#ifndef MYCPPIMPLEMENT_MySharedPtr_H
#define MYCPPIMPLEMENT_MySharedPtr_H

#include <iostream>

template <typename T>
class MyWeakPtr;

template <typename T>
class MySharedPtr;


template <typename T>
class MySharedPtr {
    T* element;

    //the number of uses
    size_t* ptr_number;
    friend class MyWeakPtr<T>;
public:
    constexpr MySharedPtr(): element(nullptr), ptr_number(new size_t(0)){}

    explicit MySharedPtr(T* ptr): element(ptr), ptr_number(new size_t(1)){}

    MySharedPtr(const MySharedPtr<T>& ptr);

    MySharedPtr(MySharedPtr&& ptr);

    explicit MySharedPtr(const MyWeakPtr<T>& ptr );

    MySharedPtr(const MySharedPtr<T>& ptr, T* new_ptr);

    ~MySharedPtr();

    MySharedPtr<T>& operator=(const MySharedPtr& ptr);

    MySharedPtr<T>& operator=(MySharedPtr&& ptr);

    //get pointer to element
    inline T* get() const {return element;}

    //get element itself
    inline T& operator*() const {return *element;}

    //give access to fields of element
    inline T* operator->() const {return element;}

    //swap two shared pointers
    void swap(MySharedPtr<T>& ptr);

    //return the number of uses
    inline size_t use_count() {return *ptr_number;}

    //check if pointer has 1 use
    inline bool unique() {return *ptr_number == 1;}

    //convert element to bool
    explicit operator bool() {return element != nullptr;}
};

template <typename T>
class MyWeakPtr {
    T* element;

    //number of uses
    size_t* ptr_number;
    friend class MySharedPtr<T>;
public:
    constexpr MyWeakPtr(): element(nullptr), ptr_number(new size_t(0)){}

    MyWeakPtr(const MySharedPtr<T>& ptr);

    MyWeakPtr(const MyWeakPtr& ptr);

    ~MyWeakPtr();

    MyWeakPtr<T>& operator=(const MySharedPtr<T>& ptr);

    MySharedPtr<T>& operator=(const MyWeakPtr& ptr);

    //swap two weak pointers
    void swap(MyWeakPtr<T>& ptr);

    //check of number of uses equals 0
    inline bool expired() const { return *ptr_number == 0; }

    //return number of uses
    inline size_t use_count() const {return *ptr_number;}

    //check if number of uses equals 1
    inline bool unique() const {return *ptr_number == 1;}

    //create shared pointer with this element
    inline MySharedPtr<T> lock() const { return expired() ? MySharedPtr<T>() : MySharedPtr<T>(*this); }
};



//Shared_Ptr
template <typename T>
MySharedPtr<T>::MySharedPtr(const MySharedPtr<T> &ptr)
{
    element = ptr.element;

    //if use number doesn't equal 0 increase it to 1
    if(*ptr.ptr_number != 0)
        ++(*ptr.ptr_number);
    ptr_number = ptr.ptr_number;
}

template <typename T>
MySharedPtr<T>::MySharedPtr(MySharedPtr&& ptr)
{
    element = std::move(ptr.element);

    //if use number doesn't equal 0 increase it to 1
    if(*ptr.ptr_number != 0)
        ++(*ptr.ptr_number);
    ptr_number = std::move(ptr.ptr_number);
}

template <typename T>
MySharedPtr<T>::MySharedPtr(const MyWeakPtr<T>& ptr)
{
    element = ptr.element;

    //if use number doesn't equal 0 increase it to 1
    if(ptr.ptr_number != 0)
        ++(*ptr.ptr_number);
    ptr_number = std::move(ptr.ptr_number);
}

template <typename T>
MySharedPtr<T>::MySharedPtr(const MySharedPtr<T> &ptr, T* new_ptr)
{
    element = new_ptr;

    //if use number doesn't equal 0 increase it to 1
    if(ptr.ptr_number != 0)
        ++(*ptr.ptr_number);
    ptr_number = ptr.ptr_number;
}

template <typename T>
MySharedPtr<T>& MySharedPtr<T>::operator=(const MySharedPtr& ptr)
{
    MySharedPtr<T>(ptr).swap(*this);
    return *this;
}

template <typename T>
MySharedPtr<T>& MySharedPtr<T>::operator=(MySharedPtr&& ptr)
{
    MySharedPtr<T>(std::move(ptr)).swap(*this);
    return *this;
}

template <typename T>
MySharedPtr<T>::~MySharedPtr()
{
    //if use number greater than 1 decrease by 1 and delete content otherwise
    if(--(*ptr_number) <= 0)
    {
        delete element;
        delete ptr_number;
    }

}


template <typename T>
void MySharedPtr<T>::swap(MySharedPtr<T> &ptr)
{
    std::swap(element, ptr.element);
    std::swap(ptr_number, ptr.ptr_number);
}


template<typename T>
inline bool operator==(const MySharedPtr<T>& ptr1, const MySharedPtr<T>& ptr2)
{
    //compare two pointers that shared pointers contain
    return ptr1.get() == ptr2.get();
}

template<typename T>
inline bool operator!=(const MySharedPtr<T>& ptr1, const MySharedPtr<T>& ptr2)
{
    //compare two pointers that shared pointers contain
    return !(ptr1 == ptr2);
}

template<typename T>
inline bool operator==(std::nullptr_t null_ptr, const MySharedPtr<T>& ptr)
{
    //true if pointer is nullptr
    return null_ptr == ptr.get();
}

template<typename T>
inline bool operator==(const MySharedPtr<T>& ptr, std::nullptr_t null_ptr)
{
    //true if pointer is nullptr
    return null_ptr == ptr.get();
}

template<typename T>
inline bool operator!=(const MySharedPtr<T>& ptr, std::nullptr_t null_ptr)
{
    //true if pointer is not nullptr
    return !(null_ptr == ptr);
}

template<typename T>
inline bool operator!=(std::nullptr_t null_ptr, const MySharedPtr<T>& ptr)
{
    //true if pointer is not nullptr
    return !(null_ptr == ptr);
}

//Weak_ptr

template <typename T>
MyWeakPtr<T>::MyWeakPtr(const MySharedPtr<T>& ptr)
{
    element = ptr.element;
    ptr_number = ptr.ptr_number;
}

template <typename T>
MyWeakPtr<T>::MyWeakPtr(const MyWeakPtr& ptr)
{
    element = ptr.element;
    ptr_number = ptr.ptr_number;
}

template <typename T>
MyWeakPtr<T>::~MyWeakPtr()
{
    //if use number greater than 1 decrease by 1 and delete content otherwise
    if(--(*ptr_number) <= 0)
    {
        delete element;
        delete ptr_number;
    }
}

template <typename T>
MyWeakPtr<T>& MyWeakPtr<T>::operator=(const MySharedPtr<T>& ptr)
{
    MyWeakPtr<T>(ptr).swap(*this);
    return *this;
}


template <typename T>
MySharedPtr<T>& MyWeakPtr<T>::operator=(const MyWeakPtr& ptr)
{
    MySharedPtr<T>(ptr).swap(*this);
    return *this;
}


template <typename T>
void MyWeakPtr<T>::swap(MyWeakPtr<T>& ptr)
{
    std::swap(ptr.element, element);
    std::swap(ptr.ptr_number, ptr_number);
}
#endif //MYCPPIMPLEMENT_MySharedPtr_H
