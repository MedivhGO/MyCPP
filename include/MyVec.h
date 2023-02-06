//
// Created by Lee on 2023/2/6.
//

#ifndef MYCPPIMPLEMENT_MYVEC_H
#define MYCPPIMPLEMENT_MYVEC_H

#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

template
    <typename T>
    class MyVec
{
 public:
  MyVec()
  {
  }

  // ilist constructor helps a lot with testing
  MyVec(std::initializer_list<T> ilist)
  {
    Reserve(ilist.size());
    for (auto it = ilist.begin(); it != ilist.end(); ++it)
    {
      Append(*it);
    }
  }

  ~MyVec()
  {
    Clear();
    free(mRawBuffer);
  }

  size_t Capacity() const { return mCapacity; }
  size_t Size() const { return mSize; }

  void Append(const T& element)
  {
    if (FreeSpace() == 0)
    {
      Grow();
    }

    new (&(mBuffer[mSize++])) T(element);
  }

  void Insert(const T& element, size_t position)
  {
    if (position > Size() || position < 0)
    {
      throw std::range_error("insert position must be in range");
    }

    // allow for append-asserts
    if (position == Size())
    {
      Append(element);
      return;
    }

    // TODO allow for multi-inserts later, just do single value for now
    ShiftElements(position, Size(), 1);
    new (&(mBuffer[position])) T(element);
  }

  void Delete(size_t position)
  {
    if (position > Size() || position < 0)
    {
      throw std::range_error("delete position must be in range");
    }

    // TODO allow for multi-deletes later, just do single value for now
    ShiftElements(position + 1, Size(), -1);
  }

  void Clear()
  {
    DestroyElements(0, Size());
    mSize = 0;
  }

  void DestroyElements(size_t begin, size_t end)
  {
    for (size_t i = begin; i < end; ++i)
    {
      mBuffer[i].~T();
    }
  }

  void ShiftElements(size_t begin, size_t end, size_t offset)
  {
    if (offset == 0)
    {
      return;
    }

    if (end <= begin)
    {
      // wtf
      return;
    }

    if (Size() + offset > Capacity())
    {
      // Make space if needed. TODO there's some optimization
      // that could be done here to only do the shift-copy once
      // (right now we do the grow-copy into the new buffer and
      // then still go ahead and do the shift-copy, which is dumb)
      Grow();
    }

    // Do this in reverse so you don't clobber stuff you want to copy later
    // Originally was doing inequalities here but got bitten by how unsigned it was :)
    const size_t shiftCount      = end - begin;
    const size_t startingElement = (offset > 0) ? end - 1 : begin;
    const int    advancing       = (offset > 0) ? -1      : 1;
    for (size_t i = 0; i < shiftCount; ++i)
    {
      // if it's a positive offset, shift from the tail rightward
      // if it's a negative offset, shift from the head leftward
      const size_t targetIdx = startingElement + (i * advancing);
      mBuffer[targetIdx + offset] = std::move(mBuffer[targetIdx]);
    }

    mSize += offset;
  }

  void Reserve(size_t requested)
  {
    if (requested < mCapacity)
    {
      return;
    }

    const size_t requiredAlignment = alignof(T);
    void* tmp = malloc(sizeof(T) * requested + requiredAlignment /*wiggle bytes*/);

    // with help from Game Engine Architecture (5.2.1.3 Aligned Allocations)
    // ---------------------------------------------------------------------
    // alignment will be some power of two, so subtract 1 to get the misalignment mask
    const size_t misalignmentMask = requiredAlignment - 1;
    // if we AND the actual address with our mask, we'll see how many bytes off we are
    const size_t misalignment     = reinterpret_cast<uintptr_t>(tmp) & misalignmentMask;
    const size_t requiredOffset   = requiredAlignment - misalignment;

    T* typedIntermediateBuffer = reinterpret_cast<T*>(reinterpret_cast<char*>(tmp) + requiredOffset);

    // Copy-Construct into the new buffer
    // TODO do we have to worry about memory overlap?
    for (size_t i = 0; i < Size(); ++i)
    {
      new (&(typedIntermediateBuffer[i])) T(mBuffer[i]);
    }

    // kill off the old
    DestroyElements(0, Size());
    free(mRawBuffer);

    mBuffer    = typedIntermediateBuffer;
    mRawBuffer = tmp;
    mCapacity  = requested;
  }

  void Grow()
  {
    if (mCapacity == 0)
    {
      Reserve(1);
    }
    else
    {
      Reserve(mCapacity * 2);
    }
  }

  size_t FreeSpace() const { return mCapacity - mSize; }

  const T& operator[](int idx) const { return mBuffer[idx]; }
  T& operator[](int idx)       { return mBuffer[idx]; }

  bool operator==(const MyVec<T>& other) const
  {
    if (Size() != other.Size())
    {
      return false;
    }

    for (size_t i = 0; i < Size(); ++i)
    {
      if (mBuffer[i] != other[i])
      {
        return false;
      }
    }
    return true;
  }

 private:
  T*     mBuffer    = nullptr;
  void*  mRawBuffer = nullptr;

  size_t mSize      = 0;
  size_t mCapacity  = 0;
};

#endif  // MYCPPIMPLEMENT_MYVEC_H
