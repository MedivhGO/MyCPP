//
// Created by Lee on 2023/2/6.
//

#ifndef MYCPPIMPLEMENT_MYVEC_H
#define MYCPPIMPLEMENT_MYVEC_H

#include <forward_list>

#include "MyError.h"

template<typename T>
class MyVector {
public:
    typedef T *iterator;
    typedef const T *const_iterator;
    typedef T value_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;

    constexpr MyVector() = default;

    constexpr MyVector(std::initializer_list<T> ilist) {
        Reserve(ilist.size());
        for (auto it = ilist.begin(); it != ilist.end(); ++it) {
            PushBack(*it);
        }
    }

    constexpr MyVector(const MyVector &rhs) {
        size_type rhs_size = rhs.Size();
        Reserve(rhs_size);
        for (const auto &x: rhs) {
            PushBack(x);
        }
    }

    constexpr MyVector(MyVector &&rhs) noexcept {
        first_ = rhs.first_;
        last_ = rhs.last_;
        end_ = rhs.end_;
        rhs.first_ = nullptr;
        rhs.last_ = nullptr;
        rhs.end_ = nullptr;
    }

    constexpr auto operator=(const MyVector &rhs) -> MyVector & {
        if (&rhs != this) {
            DestroyElements(first_, last_);
            Reserve(rhs.size());
            for (const auto &x: rhs) {
                PushBack(x);
            }
        }
        return *this;
    }

    constexpr auto operator=(MyVector &&rhs) noexcept -> MyVector & {
        if (&rhs != this) {
            first_ = rhs.first_;
            last_ = rhs.last_;
            end_ = rhs.end_;
            rhs.first_ = nullptr;
            rhs.last_ = nullptr;
            rhs.end_ = nullptr;
        }
        return *this;
    }

    constexpr ~MyVector() noexcept {
        DestroyElements(0, Size());
        free(first_);
        first_ = nullptr;
        last_ = nullptr;
        end_ = nullptr;
    }

    [[nodiscard]] constexpr auto Capacity() const -> size_type { return end_ - first_; }

    [[nodiscard]] constexpr auto Size() const -> size_type { return last_ - first_; }

    constexpr auto Insert(const T &element, size_type position) -> void {

    }

    constexpr auto Remove(const size_type position) -> void {
    }

    constexpr auto Clear() -> void {
        DestroyElements(0, Size());

    }

    constexpr auto PushBack(const T &val) -> void {
        if (FreeSpace() == 0) {
            Grow();
        }

        new(last_++) T(val); // placement new
    }

    constexpr auto EmplaceBack(T &&val) -> void {

    }

    constexpr auto PushBack(T &&val) -> void {}

    constexpr auto PopBack() noexcept -> void {}

    constexpr auto ShrinkToFit() -> void {
        if (Empty()) {

            return;
        }
        const iterator old_last = last_;
        if (old_last != end_) {
            const iterator old_first = first_;
            if (old_first == old_last) {


            } else {
                DestroyElements(0, Size());
            }
        }

    }

    constexpr auto DestroyElements(size_type begin, size_type end) -> void {
        for (size_type i = begin; i < end; ++i) {
            first_[i].~T();
        }
    }

    constexpr auto Reserve(const size_type requested) -> void {
    }

    constexpr auto Grow() -> void {
        if (Capacity() == 0) {
            Reserve(1);
        } else {
            Reserve(Capacity() * 2);
        }
    }

    constexpr auto Resize() -> void {}

    [[nodiscard]] constexpr auto At(const size_type off) const -> const_reference {
        if (off >= Size()) {
            throw MyError("myvector: index is greater than size");
        }
        return first_[off];
    }

    [[nodiscard]] constexpr auto At(const size_type off) -> reference {
        if (off >= Size()) {
            throw MyError("myvector: index is greater than size");
        }
        return first_[off];
    }

    [[nodiscard]] constexpr auto Empty() const noexcept -> bool { return first_ == last_; }

    [[nodiscard]] constexpr auto FreeSpace() const -> size_type { return end_ - last_; }

    constexpr auto operator[](int idx) const -> const_reference { return first_[idx]; }

    constexpr auto operator[](int idx) -> reference { return first_[idx]; }

    constexpr auto operator==(const MyVector<T> &other) const -> bool {
        if (Size() != other.Size()) {
            return false;
        }

        for (size_type i = 0; i < Size(); ++i) {
            if (first_[i] != other[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr auto Swap(MyVector<T> &right) noexcept -> void {}

    static constexpr auto Swap(MyVector<T> &left, MyVector<T> &right) noexcept -> void {}

private:
    iterator first_;
    iterator last_;
    iterator end_;
};

#endif