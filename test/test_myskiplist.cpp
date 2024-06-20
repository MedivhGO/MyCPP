//
// Created by Lee on 2023/9/24.
//

#include "MySkipList.h"
#include "gtest/gtest.h"

class SkipListTest : public ::testing::Test {
public:
    Skiplist skip_list_;
};

TEST_F(SkipListTest, defaultMaxLevel) {
    ASSERT_EQ(16, skip_list_.GetMaxLevel());
}

TEST_F(SkipListTest, specificMaxLevel) {
    const int max_level = 18;
    Skiplist skip_list = Skiplist(0.5, max_level);
    ASSERT_EQ(max_level, skip_list.GetMaxLevel());
}

TEST_F(SkipListTest, minKey) {
    ASSERT_THROW(skip_list_.GetMinKey(), std::runtime_error);

    skip_list_.Add(1, "a");
    skip_list_.Add(2, "b");
    ASSERT_EQ(1, skip_list_.GetMinKey());

    skip_list_.Add(3, "d");
    ASSERT_EQ(1, skip_list_.GetMinKey());

    skip_list_.Add(0, "c");
    ASSERT_EQ(0, skip_list_.GetMinKey());

    skip_list_.Peek();

    ASSERT_EQ(1, skip_list_.GetMinKey());
    while (!skip_list_.IsEmpty()) {
        skip_list_.Peek();
    }

    skip_list_.Add(100, "c");
    ASSERT_EQ(100, skip_list_.GetMinKey());
}

TEST_F(SkipListTest, maxKey) {
    ASSERT_THROW(skip_list_.GetMaxKey(), std::runtime_error);

    skip_list_.Add(1, "a");
    skip_list_.Add(2, "b");
    ASSERT_EQ(2, skip_list_.GetMaxKey());

    skip_list_.Add(0, "c");
    ASSERT_EQ(2, skip_list_.GetMaxKey());

    skip_list_.Add(3, "d");
    ASSERT_EQ(3, skip_list_.GetMaxKey());

    skip_list_.Peek();

    ASSERT_EQ(3, skip_list_.GetMaxKey());

    while (!skip_list_.IsEmpty()) {
        skip_list_.Peek();
    }

    skip_list_.Add(1, "a");
    ASSERT_EQ(1, skip_list_.GetMaxKey());
}

TEST_F(SkipListTest, insertAndPeek) {
    const uint64_t size = 1000;

    // insert values
    for (int i = 1; i <= size; i++) {
        std::ostringstream os;
        os << i;
        skip_list_.Add(i, os.str());
        ASSERT_FALSE(skip_list_.IsEmpty());
        ASSERT_EQ(i, skip_list_.Size());
    }

    ASSERT_EQ(1000, skip_list_.KeysOnSpecificLevel(0));

    // find and delete
    for (int i = 1; i <= size; i++) {
        std::ostringstream os;
        os << i;
        ASSERT_FALSE(skip_list_.IsEmpty());
        ASSERT_EQ(skip_list_.Peek(), os.str());
        ASSERT_EQ(size - i, skip_list_.Size());
    }
    ASSERT_TRUE(skip_list_.IsEmpty());
    EXPECT_THROW(skip_list_.Peek(), std::runtime_error);
}

TEST_F(SkipListTest, updateValues) {
    const uint64_t size = 1000;

    // insert values
    for (int i = 1; i <= size; i++) {
        std::ostringstream os;
        os << i;
        skip_list_.Add(i, os.str());
        ASSERT_FALSE(skip_list_.IsEmpty());
        ASSERT_EQ(i, skip_list_.Size());
    }

    // update values
    for (int i = 1; i <= size; i++) {
        std::ostringstream os;
        os << i;
        skip_list_.Add(i, os.str());
        ASSERT_FALSE(skip_list_.IsEmpty());
        ASSERT_EQ(size, skip_list_.Size());
    }

    // find and delete
    for (int i = 1; i <= size; i++) {
        std::ostringstream os;
        os << i;
        ASSERT_FALSE(skip_list_.IsEmpty());
        ASSERT_EQ(skip_list_.Peek(), os.str());
        ASSERT_EQ(size - i, skip_list_.Size());
    }
    ASSERT_TRUE(skip_list_.IsEmpty());
    EXPECT_THROW(skip_list_.Peek(), std::runtime_error);
}

TEST_F(SkipListTest, EmptyList) {
    ASSERT_TRUE(skip_list_.IsEmpty());
    ASSERT_EQ(0, skip_list_.Size());
    EXPECT_THROW(skip_list_.Peek(), std::runtime_error);
}

TEST_F(SkipListTest, SearchAndErase) {
    const uint64_t size = 1000;

    for (int i = 1; i <= size; i++) {
        std::ostringstream os;
        os << i;
        skip_list_.Add(i, os.str());
        ASSERT_FALSE(skip_list_.IsEmpty());
        ASSERT_EQ(i, skip_list_.Size());
    }

    ASSERT_TRUE(skip_list_.Search(999));
    ASSERT_TRUE(skip_list_.Erase(999));
    ASSERT_FALSE(skip_list_.Search(999));
}
