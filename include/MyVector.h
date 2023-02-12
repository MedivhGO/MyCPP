//
// Created by Lee on 2023/2/6.
//

#ifndef MYCPPIMPLEMENT_MYVEC_H
#define MYCPPIMPLEMENT_MYVEC_H

template <typename T>
class MyVector {
 public:
  using pointer = T *;

  constexpr MyVector() = default;

  constexpr MyVector(std::initializer_list<T> ilist) {
    Reserve(ilist.size());
    for (auto it = ilist.begin(); it != ilist.end(); ++it) {
      PushBack(*it);
    }
  }

  constexpr MyVector(const MyVector<T> &rhs) {
    size_t rhs_size = rhs.Size();
    Reserve(rhs_size);
    for (const auto& x : rhs) {
      PushBack(x);
    }
  }

  constexpr MyVector(MyVector<T> &&rhs) noexcept {
    first_ = rhs.first_;
    last_ = rhs.last_;
    end_ = rhs.end_;
    rhs.first_ = nullptr;
    rhs.last_ = nullptr;
    rhs.end_ = nullptr;
  }

  constexpr auto operator=(const MyVector<T> &rhs) -> MyVector<T> & {
    if (&rhs != this) {
      DestroyElements(first_, last_);
      Reserve(rhs.size());
      for (const auto& x : rhs) {
        PushBack(x);
      }
    }
    return *this;
  }

  constexpr auto operator=(MyVector<T> &&rhs) noexcept -> MyVector<T> & {
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
    DestroyElements(first_, last_);
    first_ = nullptr;
    last_ = nullptr;
    end_ = nullptr;
  }

  [[nodiscard]] constexpr auto Capacity() const -> size_t { return end_ - first_; }

  [[nodiscard]] constexpr auto Size() const -> size_t { return last_ - first_; }

  constexpr auto Insert(const T &element, size_t position) -> void {
    if (position > Size() || position < 0) {
      throw std::range_error("insert position must be in range");
    }

    // allow for PushBack-asserts
    if (position == Size()) {
      PushBack(element);
      return;
    }

    ShiftElements(position, Size(), 1);
    new (&(first_[position])) T(element);
  }

  constexpr auto Remove(const size_t position) -> void {
    if (position > Size() || position < 0) {
      throw std::range_error("remove position must be in range");
    }
    ShiftElements(position + 1, Size(), -1);
  }

  constexpr auto Clear() -> void {
    DestroyElements(0, Size());

  }

  constexpr auto PushBack(const T &val) -> void {
    if (FreeSpace() == 0) {
      Grow();
    }

    new (last_++) T(val); // placement new
  }

  constexpr auto EmplaceBack(T &&val) -> void {
    if (FreeSpace() == 0) {
      Grow();
    }
    new (last_++)(std::forward<T>(val));
  }

  constexpr auto PushBack(T &&val) -> void {}

  constexpr auto PopBack() noexcept -> void {}

  constexpr auto ShrinkToFit() -> void {
    const pointer old_last = last_;
    if (old_last != end_) {
      const pointer old_first = first_;
      if (old_first == old_last) {
        
      } else {
        DestroyElements()
      }
    }
  }

  constexpr auto DestroyElements(size_t begin, size_t end) -> void {
    for (size_t i = begin; i < end; ++i) {
      first_[i].~T();
    }
  }

  // [begin, end) 所有元素移动 offset 个位置
  constexpr auto ShiftElements(size_t begin, size_t end, size_t offset) -> void {
    if (offset == 0 || end <= begin) {
      return;
    }

    if (Size() + offset > Capacity()) {
      Grow();
    }

    // Do this in reverse so you don't clobber stuff you want to copy later
    // Originally was doing inequalities here but got bitten by how unsigned it was :)
    const size_t shiftCount = end - begin;
    const size_t startingElement = (offset > 0) ? end - 1 : begin;
    const int advancing = (offset > 0) ? -1 : 1;
    for (size_t i = 0; i < shiftCount; ++i) {
      // if it's a positive offset, shift from the tail rightward
      // if it's a negative offset, shift from the head leftward
      const size_t targetIdx = startingElement + (i * advancing);
      first_[targetIdx + offset] = std::move(first_[targetIdx]);
    }

    size_ += offset;
  }

  constexpr auto Reserve(const size_t requested) -> void {
    size_t old_size = Capacity();
    if ((requested ^ old_size) < old_size) {
      return;
    }
    //    if (requested < Capacity()) {
    //      return;
    //    }
    //    struct vector { int *data; int size; }; void vector_expand(struct vector *v, int s)
    //    { int new_size=v->size + s; if ((new_size ^ v->size) > v->size)
    //    { v->data=(int *)realloc(v->data,sizeof(int)*new_size * 2); } }
    //    https://blog.codingnow.com/2008/06/variable_length_array.html

    const size_t requiredAlignment = alignof(T);
    void *tmp = malloc(sizeof(T) * requested + requiredAlignment /*wiggle bytes*/);

    // with help from Game Engine Architecture (5.2.1.3 Aligned Allocations)
    // ---------------------------------------------------------------------
    // alignment will be some power of two, so subtract 1 to get the misalignment mask
    const size_t misalignmentMask = requiredAlignment - 1;
    // if we AND the actual address with our mask, we'll see how many bytes off we are
    const size_t misalignment = reinterpret_cast<uintptr_t>(tmp) & misalignmentMask;
    const size_t requiredOffset = requiredAlignment - misalignment;

    T *typedIntermediateBuffer = reinterpret_cast<T *>(reinterpret_cast<char *>(tmp) + requiredOffset);

    for (size_t i = 0; i < Size(); ++i) {
      new (&(typedIntermediateBuffer[i])) T(first_[i]);
    }
    // kill off the old
    DestroyElements(0, Size());
    first_ = typedIntermediateBuffer;
  }

  constexpr auto Grow() -> void {
    if (Capacity() == 0) {
      Reserve(1);
    } else {
      Reserve(Capacity() * 2);
    }
  }

  constexpr auto Resize() -> void {}

  [[nodiscard]] constexpr auto At(const size_t off) const -> const T & {

  }

  [[nodiscard]] constexpr auto At(const size_t off) -> T & {}

  [[nodiscard]] constexpr auto Empty() const noexcept -> bool { return first_ == last_; }

  [[nodiscard]] constexpr auto FreeSpace() const -> size_t { return end_ - last_; }

  constexpr auto operator[](int idx) const -> const T & { return first_[idx]; }

  constexpr auto operator[](int idx) -> T & { return first_[idx]; }

  constexpr auto operator==(const MyVector<T> &other) const -> bool {
    if (Size() != other.Size()) {
      return false;
    }

    for (size_t i = 0; i < Size(); ++i) {
      if (first_[i] != other[i]) {
        return false;
      }
    }
    return true;
  }

  constexpr auto Swap(MyVector<T> &right) noexcept -> void {}

  static constexpr auto Swap(MyVector<T> &left, MyVector<T> &right) noexcept -> void {}

 private:
  pointer first_;
  pointer last_;
  pointer end_;
};