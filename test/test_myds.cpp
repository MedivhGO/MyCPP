//
// Created by Lee on 2022/9/6.
//
#include <gtest/gtest.h>

#include "MySkipList.h"
#include "MyRandom.h"


TEST(MyDataStructTest, test1) {
    Skiplist skiplist;
    for (int i = 0; i < 10000; ++i) {
        skiplist.add(i);
    }

    RandomNumberGenerator rng(0, 9999);
    for (int i = 0; i < 10000; ++i) {
        EXPECT_TRUE(skiplist.search(rng.GetRandomNumberByMt19937()));
    }
}
