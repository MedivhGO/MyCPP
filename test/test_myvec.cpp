//
// Created by Lee on 2023/2/6.
//

#include <gtest/gtest.h>
#include <cmath>
#include "MyVec.h"
#include "mysmallvec.h"

// Custom Test Types
// --------------------------------------------------------------------
class DestructoType {
 public:
  static size_t destruction_count;

  DestructoType() = default;
  ~DestructoType() {
    destruction_count++;
    destroyed = true;
  }

  bool destroyed = false;
};
size_t DestructoType::destruction_count = 0;  // todo why does this have to be initialized?

struct ThreeIntType {
  int a, b, c;
};

struct PaddedType {
  char a;
  double b;
};

class Pigeon;  // fwd
class PigeonManager {
 public:
  PigeonManager() = default;
  static void RegisterPigeon(Pigeon *p) { mPigeons.insert(p); }
  static void DeregisterPigeon(Pigeon *p) { mPigeons.erase(p); }
  static bool IsRegistered(Pigeon *p) { return mPigeons.find(p) != mPigeons.end(); }
  static std::set<Pigeon *> mPigeons;
};
std::set<Pigeon *> PigeonManager::mPigeons;

class Pigeon {
 public:
  Pigeon() { PigeonManager::RegisterPigeon(this); }
  Pigeon(const Pigeon &other) { PigeonManager::RegisterPigeon(this); }
  ~Pigeon() { PigeonManager::DeregisterPigeon(this); }
};

// Fixtures
// --------------------------------------------------------------------
class MyVecTest : public ::testing::Test {
 protected:
  virtual void SetUp() override { DestructoType::destruction_count = 0; }
  virtual void TearDown() override { PigeonManager::mPigeons.clear(); }

  MyVec<int> v;
  MyVec<char> vc;
  MyVec<float> vf;
  MyVec<double> vd;
  MyVec<ThreeIntType> v3i;
  MyVec<PaddedType> vp;
  MyVec<Pigeon> vpigeons;
};

// Tests
// --------------------------------------------------------------------
TEST_F(MyVecTest, Init) {
  EXPECT_EQ(v.Size(), 0);
  EXPECT_EQ(v.Capacity(), 0);
}

TEST_F(MyVecTest, Append) {
  static const size_t insertions = 0xFFFF;

  for (size_t i = 0; i < insertions; ++i) {
    v.Append(i);
    // test that the size is correctly incrementing
    EXPECT_EQ(v.Size(), i + 1);
    // test that capacity is doubling when it should
    EXPECT_EQ(std::log2(v.Capacity()), std::ceil(std::log2(i + 1)));
  }

  for (size_t i = 0; i < insertions; ++i) {
    // test that the values are in the correct positions
    EXPECT_EQ(v[i], i);
  }
}

TEST_F(MyVecTest, Insert) {
  MyVec<int> expected({-1, 0, 1, 2});

  for (size_t i = 0; i < 3; ++i) {
    v.Append(i);
  }

  v.Insert(-1, 0);

  EXPECT_TRUE(v.Size() == 4);
  EXPECT_TRUE(v == expected);
}

TEST_F(MyVecTest, Delete) {
  MyVec<int> expected({0, 1, 3});

  for (size_t i = 0; i < 4; ++i) {
    v.Append(i);
  }

  v.Delete(2);

  EXPECT_TRUE(v.Size() == 3);
  EXPECT_TRUE(v == expected);
}

TEST_F(MyVecTest, Clear) {
  MyVec<DestructoType> destructo;

  const size_t expected = 100;
  destructo.Reserve(expected);
  for (size_t i = 0; i < expected; ++i) {
    // todo emplace?
    destructo.Append(DestructoType());
  }

  // test that we're actually calling the dtors
  destructo.Clear();
  EXPECT_EQ(
      DestructoType::destruction_count,
      expected * 2);  // *2 because the tmp gets destroyed as well, todo will this still happen if I make a move cons?
}

TEST_F(MyVecTest, Grow) {
  static const uint16_t growths = 16;

  for (size_t i = 0; i < growths; ++i) {
    v.Grow();
    EXPECT_EQ(v.Capacity(), std::pow(2, i));
  }

  EXPECT_EQ(v.Size(), 0);
}

TEST_F(MyVecTest, Alignment) {
  size_t requiredAlignment;
  size_t misalignmentMask;
  size_t misalignment;

  vc.Append('a');
  requiredAlignment = alignof(char);
  EXPECT_EQ(requiredAlignment, 1);
  misalignmentMask = requiredAlignment - 1;
  misalignment = reinterpret_cast<uintptr_t>(&vc[0]) & misalignmentMask;
  EXPECT_EQ(misalignment, 0);

  vf.Append(1.0f);
  requiredAlignment = alignof(float);
  EXPECT_EQ(requiredAlignment, 4);
  misalignmentMask = requiredAlignment - 1;
  misalignment = reinterpret_cast<uintptr_t>(&vf[0]) & misalignmentMask;
  EXPECT_EQ(misalignment, 0);

  vd.Append(1.0);
  requiredAlignment = alignof(double);
  EXPECT_EQ(requiredAlignment, 8);
  misalignmentMask = requiredAlignment - 1;
  misalignment = reinterpret_cast<uintptr_t>(&vd[0]) & misalignmentMask;
  EXPECT_EQ(misalignment, 0);

  v3i.Append({0, 1, 2});
  requiredAlignment = alignof(ThreeIntType);
  EXPECT_EQ(requiredAlignment, 4);
  misalignmentMask = requiredAlignment - 1;
  misalignment = reinterpret_cast<uintptr_t>(&v3i[0]) & misalignmentMask;
  EXPECT_EQ(misalignment, 0);

  vp.Append({'a', 1.0});
  requiredAlignment = alignof(PaddedType);
  EXPECT_EQ(requiredAlignment, 8);
  misalignmentMask = requiredAlignment - 1;
  misalignment = reinterpret_cast<uintptr_t>(&vp[0]) & misalignmentMask;
  EXPECT_EQ(misalignment, 0);
}

TEST_F(MyVecTest, CopyConstructionAndDestruction) {
  static const size_t insertions = 16;
  for (size_t i = 0; i < insertions; ++i) {
    vpigeons.Append(Pigeon());
  }

  // If the size doesn't equal the number of insertions that means
  // we're not cleaning up destroyed pigeons correctly
  EXPECT_EQ(PigeonManager::mPigeons.size(), insertions);

  Pigeon *firstPigeyAddr = &vpigeons[0];

  EXPECT_TRUE(PigeonManager::IsRegistered(firstPigeyAddr));
  vpigeons.Grow();  // should invalidate all pigeon pointers
  EXPECT_FALSE(PigeonManager::IsRegistered(firstPigeyAddr));
}

TEST_F(MyVecTest, DestroyElements) {
  MyVec<DestructoType> destructo;
  static const size_t insertions = 0x10;
  for (size_t i = 0; i < insertions; ++i) {
    destructo.Append(DestructoType());
  }

  // destroy the first half of the vector
  destructo.DestroyElements(0, insertions >> 1);

  for (size_t i = 0; i < insertions >> 1; ++i) {
    EXPECT_TRUE(destructo[i].destroyed);
  }
  for (size_t i = (insertions >> 1) + 1; i < destructo.Size(); ++i) {
    EXPECT_FALSE(destructo[i].destroyed);
  }
}

TEST_F(MyVecTest, Equality) {
  MyVec<int> otherGuy;
  for (size_t i = 0; i < 0xFF; ++i) {
    v.Append(i);
    otherGuy.Append(i);
  }

  EXPECT_EQ(v, otherGuy);
}

TEST(MyVecTest1, test10) {
  {
    tf::SmallVector<int> vec1;
    EXPECT_TRUE(vec1.size() == 0);
    EXPECT_TRUE(vec1.empty() == true);

    tf::SmallVector<int, 4> vec2;
    EXPECT_TRUE(vec2.data() != nullptr);
    EXPECT_TRUE(vec2.size() == 0);
    EXPECT_TRUE(vec2.empty() == true);
    EXPECT_TRUE(vec2.capacity() == 4);
  }

  // SUBCASE("constructor_n")
  {
    for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
      tf::SmallVector<int> vec(N);
      EXPECT_TRUE(vec.size() == N);
      EXPECT_TRUE(vec.empty() == (N == 0));
      EXPECT_TRUE(vec.max_size() >= vec.size());
      EXPECT_TRUE(vec.capacity() >= vec.size());
    }
  }

  // SUBCASE("copy_constructor")
  {
    for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
      tf::SmallVector<int> vec1(N);
      for (auto &item : vec1) {
        item = N;
      }

      tf::SmallVector<int> vec2(vec1);
      EXPECT_TRUE(vec1.size() == N);
      EXPECT_TRUE(vec2.size() == N);
      for (size_t i = 0; i < vec1.size(); ++i) {
        EXPECT_TRUE(vec1[i] == vec2[i]);
        EXPECT_TRUE(vec1[i] == N);
      }
    }
  }

  // SUBCASE("move_constructor")
  {
    for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
      tf::SmallVector<int> vec1(N);
      for (auto &item : vec1) {
        item = N;
      }

      tf::SmallVector<int> vec2(std::move(vec1));
      EXPECT_TRUE(vec1.size() == 0);
      EXPECT_TRUE(vec1.empty() == true);
      EXPECT_TRUE(vec2.size() == N);

      for (size_t i = 0; i < vec2.size(); ++i) {
        EXPECT_TRUE(vec2[i] == N);
      }
    }
  }

  // SUBCASE("push_back")
  {
    for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
      tf::SmallVector<int> vec;
      size_t pcap{0};
      size_t ncap{0};
      for (int n = 0; n < N; ++n) {
        vec.push_back(n);
        EXPECT_TRUE(vec.size() == n + 1);
        ncap = vec.capacity();
        EXPECT_TRUE(ncap >= pcap);
        pcap = ncap;
      }
      for (int n = 0; n < N; ++n) {
        EXPECT_TRUE(vec[n] == n);
      }
      EXPECT_TRUE(vec.empty() == (N == 0));
    }
  }

  // SUBCASE("pop_back")
  {
    size_t size{0};
    size_t pcap{0};
    size_t ncap{0};
    tf::SmallVector<int> vec;
    for (int N = 0; N <= 65536; N = (N ? N << 1 : N + 1)) {
      vec.push_back(N);
      ++size;
      EXPECT_TRUE(vec.size() == size);
      if (N % 4 == 0) {
        vec.pop_back();
        --size;
        EXPECT_TRUE(vec.size() == size);
      }
      ncap = vec.capacity();
      EXPECT_TRUE(ncap >= pcap);
      pcap = ncap;
    }
    EXPECT_TRUE(vec.size() == size);
    for (size_t i = 0; i < vec.size(); ++i) {
      EXPECT_TRUE(vec[i] % 4 != 0);
    }
  }

  // SUBCASE("iterator")
  {
    for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
      tf::SmallVector<int> vec;
      for (int n = 0; n < N; ++n) {
        vec.push_back(n);
        EXPECT_TRUE(vec.size() == n + 1);
      }

      // non-constant iterator
      {
        int val{0};
        for (auto item : vec) {
          EXPECT_TRUE(item == val);
          ++val;
        }
      }

      // constant iterator
      {
        int val{0};
        for (const auto &item : vec) {
          EXPECT_TRUE(item == val);
          ++val;
        }
      }

      // change the value
      {
        for (auto &item : vec) {
          item = 1234;
        }
        for (auto &item : vec) {
          EXPECT_TRUE(item == 1234);
        }
      }
    }
  }

  // SUBCASE("clear")
  {
    for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
      tf::SmallVector<int> vec(N);
      auto cap = vec.capacity();
      EXPECT_TRUE(vec.size() == N);
      vec.clear();
      EXPECT_TRUE(vec.size() == 0);
      EXPECT_TRUE(vec.capacity() == cap);
    }
  }

  // SUBCASE("comparison")
  {
    for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
      tf::SmallVector<int> vec1;
      for (int i = 0; i < N; ++i) {
        vec1.push_back(i);
      }
      tf::SmallVector<int> vec2(vec1);
      EXPECT_TRUE(vec1 == vec2);
    }
  }
}