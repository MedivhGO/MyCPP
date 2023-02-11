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

  constexpr MyVector(const MyVector<T> &rhs) {}

  constexpr MyVector(MyVector<T> &&rhs) noexcept {}

  constexpr auto operator=(const MyVector<T> &other) -> MyVector<T> & { return *this; }

  constexpr auto operator=(MyVector<T> &&other) noexcept -> MyVector<T> & { return *this; }

  constexpr ~MyVector() noexcept {
    Clear();
    free(mRawBuffer);
    if (first_) {
      first_ = nullptr;
      last_ = nullptr;
      end_ = nullptr;
    }
  }

  [[nodiscard]] constexpr auto Capacity() const -> size_t { return end_ - first_; }

  [[nodiscard]] constexpr auto Size() const -> size_t { return size_; }

  constexpr auto Insert(const T &element, size_t position) -> void {
    if (position > Size() || position < 0) {
      throw std::range_error("insert position must be in range");
    }

    // allow for PushBack-asserts
    if (position == Size()) {
      PushBack(element);
      return;
    }

    // TODO allow for multi-inserts later, just do single value for now
    ShiftElements(position, Size(), 1);
    new (&(first_[position])) T(element);
  }

  constexpr auto Remove(const size_t position) -> void {
    if (position > Size() || position < 0) {
      throw std::range_error("remove position must be in range");
    }

    // TODO allow for multi-deletes later, just do single value for now
    ShiftElements(position + 1, Size(), -1);
  }

  constexpr auto Clear() -> void {
    DestroyElements(0, Size());
    size_ = 0;
  }

  constexpr auto PushBack(const T &val) -> void {
    if (FreeSpace() == 0) {
      Grow();
    }

    new (&(first_[size_++])) T(element);
  }

  constexpr auto EmplaceBack(T&& val) -> void {
    
    
  }

  constexpr auto PushBack(T &&val) -> void {}

  constexpr auto ShrinkToFit() -> void {}

  constexpr auto DestroyElements(size_t begin, size_t end) -> void {
    for (size_t i = begin; i < end; ++i) {
      first_[i].~T();
    }
  }

  constexpr auto ShiftElements(size_t begin, size_t end, size_t offset) -> void {
    if (offset == 0 || end <= begin) {
      return;
    }

    if (Size() + offset > Capacity()) {
      // Make space if needed. TODO there's some optimization
      // that could be done here to only do the shift-copy once
      // (right now we do the grow-copy into the new buffer and
      // then still go ahead and do the shift-copy, which is dumb)
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
    if (requested < Capacity()) {
      return;
    }

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
    free(mRawBuffer);

    first_ = typedIntermediateBuffer;
    mRawBuffer = tmp;
  }

  constexpr auto Grow() -> void {
    if (Capacity() == 0) {
      Reserve(1);
    } else {
      Reserve(Capacity() * 2);
    }
  }

  constexpr auto Resize() -> void{

  }

  [[nodiscard]] constexpr auto At(const size_t off) const -> const T & {

  }

  [[nodiscard]] constexpr auto At(const size_t off) -> T & {

  }

  [[nodiscard]] constexpr auto Empty() const noexcept -> bool {
    return first_ == last_;
  }

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