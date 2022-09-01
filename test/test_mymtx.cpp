//
// Created by Lee on 2022/9/2.
//
#include <gtest/gtest.h>

#include "MutexLock.h"

TEST(MyMutexTest, init) {
    MutexLock m;
    m.lock();
    m.unlock();
}