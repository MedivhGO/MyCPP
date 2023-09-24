//
// Created by Administrator on 2023/9/24.
//

#include "MySkipList.h"
#include "gtest/gtest.h"

class SkipListTest : public ::testing::Test {
public:
    Skiplist skipList;
};

TEST_F(SkipListTest, defaultMaxLevel) {
    ASSERT_EQ(16, skipList.GetMaxLevel());
}

TEST_F(SkipListTest, specificMaxLevel) {
    const int MAX_LEVEL = 18;
    Skiplist skipList = Skiplist(0.5, MAX_LEVEL);
    ASSERT_EQ(MAX_LEVEL, skipList.GetMaxLevel());
}

TEST_F(SkipListTest, minKey) {
    ASSERT_THROW(skipList.GetMinKey(), std::runtime_error);

    skipList.InsertOrUpdate(1, "a");
    skipList.InsertOrUpdate(2, "b");
    ASSERT_EQ(1, skipList.GetMinKey());

    skipList.InsertOrUpdate(3, "d");
    ASSERT_EQ(1, skipList.GetMinKey());

    skipList.InsertOrUpdate(0, "c");
    ASSERT_EQ(0, skipList.GetMinKey());

    skipList.Peek();

    ASSERT_EQ(1, skipList.
            GetMinKey());
    while (!skipList.IsEmpty()) {
        skipList.Peek();
    }

    skipList.InsertOrUpdate(100, "c");
    ASSERT_EQ(100, skipList.GetMinKey());
}

TEST_F(SkipListTest, maxKey) {
    ASSERT_THROW(skipList.GetMaxKey(), std::runtime_error);

    skipList.InsertOrUpdate(1, "a");
    skipList.InsertOrUpdate(2, "b");
    ASSERT_EQ(2, skipList.GetMaxKey());

    skipList.InsertOrUpdate(0, "c");
    ASSERT_EQ(2, skipList.GetMaxKey());

    skipList.InsertOrUpdate(3, "d");
    ASSERT_EQ(3, skipList.GetMaxKey());

    skipList.Peek();

    ASSERT_EQ(3, skipList.GetMaxKey());

    while (!skipList.IsEmpty()) {
        skipList.Peek();
    }

    skipList.InsertOrUpdate(1, "a");
    ASSERT_EQ(1, skipList.GetMaxKey());
}

TEST_F(SkipListTest, insertAndPeek) {
    const unsigned long SIZE = 1000;

    // insert values
    for (int i = 1; i <= SIZE; i++) {
        std::ostringstream os;
        os << i;
        skipList.InsertOrUpdate(i, os.str());
        ASSERT_FALSE(skipList.IsEmpty());
        ASSERT_EQ(i, skipList.Size());
    }

    // find and delete
    for (int i = 1; i <= SIZE; i++) {
        std::ostringstream os;
        os << i;
        ASSERT_FALSE(skipList.IsEmpty());
        ASSERT_EQ(skipList.Peek(), os.str());
        ASSERT_EQ(SIZE - i, skipList.Size());
    }
    ASSERT_TRUE(skipList.IsEmpty());
    EXPECT_THROW(skipList.Peek(), std::runtime_error);
}

TEST_F(SkipListTest, updateValues) {
    const unsigned long SIZE = 1000;

    // insert values
    for (int i = 1; i <= SIZE; i++) {
        std::ostringstream os;
        os << i;
        skipList.InsertOrUpdate(i, os.str());
        ASSERT_FALSE(skipList.IsEmpty());
        ASSERT_EQ(i, skipList.Size());
    }

    // update values
    for (int i = 1; i <= SIZE; i++) {
        std::ostringstream os;
        os << i;
        skipList.InsertOrUpdate(i, os.str());
        ASSERT_FALSE(skipList.IsEmpty());
        ASSERT_EQ(SIZE, skipList.Size());
    }

    // find and delete
    for (int i = 1; i <= SIZE; i++) {
        std::ostringstream os;
        os << i << i;
        ASSERT_FALSE(skipList.IsEmpty());
        ASSERT_EQ(skipList.Peek(), os.str());
        ASSERT_EQ(SIZE - i, skipList.Size());
    }
    ASSERT_TRUE(skipList.IsEmpty());
    EXPECT_THROW(skipList.Peek(), std::runtime_error);
}

TEST_F(SkipListTest, EmptyList) {
    ASSERT_TRUE(skipList.IsEmpty());
    ASSERT_EQ(0, skipList.Size());
    EXPECT_THROW(skipList.Peek(), std::runtime_error);
}
