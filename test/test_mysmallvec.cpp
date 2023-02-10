//
// Created by Lee on 2023/2/11
//

#include <gtest/gtest.h>

#include "MySmallVector.h

TEST(MySmallVecTest, constructor) {
  util::smallvector<int> vec1;
  expect_true(vec1.size() == 0);
  expect_true(vec1.empty() == true);

  util::smallvector<int, 4> vec2;
  expect_true(vec2.data() != nullptr);
  expect_true(vec2.size() == 0);
  expect_true(vec2.empty() == true);
  expect_true(vec2.capacity() == 4);
}

TEST(MySmallVecTest, constructor_n) {
  for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
    util::SmallVector<int> vec(N);
    EXPECT_TRUE(vec.size() == N);
    EXPECT_TRUE(vec.empty() == (N == 0));
    EXPECT_TRUE(vec.max_size() >= vec.size());
    EXPECT_TRUE(vec.capacity() >= vec.size());
  }
}

TEST(MySmallVecTest, copy_constructor) {
  for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
    util::SmallVector<int> vec1(N);
    for (auto &item : vec1) {
      item = N;
    }

    util::SmallVector<int> vec2(vec1);
    EXPECT_TRUE(vec1.size() == N);
    EXPECT_TRUE(vec2.size() == N);
    for (size_t i = 0; i < vec1.size(); ++i) {
      EXPECT_TRUE(vec1[i] == vec2[i]);
      EXPECT_TRUE(vec1[i] == N);
    }
  }
}

TEST(MySmallVecTest, move_constructor) {
  for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
    util::SmallVector<int> vec1(N);
    for (auto &item : vec1) {
      item = N;
    }

    util::SmallVector<int> vec2(std::move(vec1));
    EXPECT_TRUE(vec1.size() == 0);
    EXPECT_TRUE(vec1.empty() == true);
    EXPECT_TRUE(vec2.size() == N);

    for (size_t i = 0; i < vec2.size(); ++i) {
      EXPECT_TRUE(vec2[i] == N);
    }
  }
}

TEST(MySmallVecTest, push_back) {
  for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
    util::SmallVector<int> vec;
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

TEST(MySmallVecTest, pop_back) {
  size_t size{0};
  size_t pcap{0};
  size_t ncap{0};
  util::SmallVector<int> vec;
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

TEST(MySmallVecTest, iterator) {
  for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
    util::SmallVector<int> vec;
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

TEST(MySmallVecTest, clear) {
  for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
    util::SmallVector<int> vec(N);
    auto cap = vec.capacity();
    EXPECT_TRUE(vec.size() == N);
    vec.clear();
    EXPECT_TRUE(vec.size() == 0);
    EXPECT_TRUE(vec.capacity() == cap);
  }
}

TEST(MySmallVecTest, comparison) {
  for (int N = 0; N <= 65536; N = (N ? N << 1 : 1)) {
    util::SmallVector<int> vec1;
    for (int i = 0; i < N; ++i) {
      vec1.push_back(i);
    }
    util::SmallVector<int> vec2(vec1);
    EXPECT_TRUE(vec1 == vec2);
  }
}