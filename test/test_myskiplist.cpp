//
// Created by Administrator on 2023/9/24.
//

#include "MySkipList.h"
#include "gtest/gtest.h"

class SkipListTest : public ::testing::Test {
public:
    SkipList skipList;
};

TEST_F(SkipListTest, defaultMaxLevel
) {
ASSERT_EQ(16, skipList.

getMaxLevel()

);
}

TEST_F(SkipListTest, specificMaxLevel
) {
const int MAX_LEVEL = 18;
SkipList skipList = SkipList(0.5, MAX_LEVEL);
ASSERT_EQ(MAX_LEVEL, skipList
.

getMaxLevel()

);
}


TEST_F(SkipListTest, minKey
) {
ASSERT_THROW(skipList
.

getMinKey(), std::runtime_error

);

skipList.insertOrUpdate(1, "a");
skipList.insertOrUpdate(2, "b");
ASSERT_EQ(1, skipList.

getMinKey()

);

skipList.insertOrUpdate(3, "d");
ASSERT_EQ(1, skipList.

getMinKey()

);

skipList.insertOrUpdate(0, "c");
ASSERT_EQ(0, skipList.

getMinKey()

);

skipList.

peek();

ASSERT_EQ(1, skipList.

getMinKey()

);

while(!skipList.

isEmpty()

)
skipList.

peek();

skipList.insertOrUpdate(100, "c");
ASSERT_EQ(100, skipList.

getMinKey()

);
}


TEST_F(SkipListTest, maxKey
) {
ASSERT_THROW(skipList
.

getMaxKey(), std::runtime_error

);

skipList.insertOrUpdate(1, "a");
skipList.insertOrUpdate(2, "b");
ASSERT_EQ(2, skipList.

getMaxKey()

);

skipList.insertOrUpdate(0, "c");
ASSERT_EQ(2, skipList.

getMaxKey()

);

skipList.insertOrUpdate(3, "d");
ASSERT_EQ(3, skipList.

getMaxKey()

);

skipList.

peek();

ASSERT_EQ(3, skipList.

getMaxKey()

);

while(!skipList.

isEmpty()

)
skipList.

peek();

skipList.insertOrUpdate(1, "a");
ASSERT_EQ(1, skipList.

getMaxKey()

);
}

TEST_F(SkipListTest, insertAndPeek
) {
const unsigned long SIZE = 1000;

//insert values
for (
int i = 1;
i <=
SIZE;
i++) {
std::ostringstream os;
os <<
i;
skipList.
insertOrUpdate(i, os
.

str()

);
ASSERT_FALSE(skipList
.

isEmpty()

);
ASSERT_EQ(i, skipList
.

size()

);
}

//find and delete
for (
int i = 1;
i <=
SIZE;
i++) {
std::ostringstream os;
os <<
i;
ASSERT_FALSE(skipList
.

isEmpty()

);
ASSERT_EQ(skipList
.

peek(), os

.

str()

);
ASSERT_EQ(SIZE
- i, skipList.

size()

);
}
ASSERT_TRUE(skipList
.

isEmpty()

);
EXPECT_THROW(skipList
.

peek(), std::runtime_error

);
}


TEST_F(SkipListTest, updateValues
) {
const unsigned long SIZE = 1000;

//insert values
for (
int i = 1;
i <=
SIZE;
i++) {
std::ostringstream os;
os <<
i;
skipList.
insertOrUpdate(i, os
.

str()

);
ASSERT_FALSE(skipList
.

isEmpty()

);
ASSERT_EQ(i, skipList
.

size()

);
}

//update values
for (
int i = 1;
i <=
SIZE;
i++) {
std::ostringstream os;
os <<
i;
skipList.
insertOrUpdate(i, os
.

str()

);
ASSERT_FALSE(skipList
.

isEmpty()

);
ASSERT_EQ(SIZE, skipList
.

size()

);
}

//find and delete
for (
int i = 1;
i <=
SIZE;
i++) {
std::ostringstream os;
os << i <<
i;
ASSERT_FALSE(skipList
.

isEmpty()

);
ASSERT_EQ(skipList
.

peek(), os

.

str()

);
ASSERT_EQ(SIZE
- i, skipList.

size()

);
}
ASSERT_TRUE(skipList
.

isEmpty()

);
EXPECT_THROW(skipList
.

peek(), std::runtime_error

);
}

TEST_F(SkipListTest, EmptyList
) {
ASSERT_TRUE(skipList
.

isEmpty()

);
ASSERT_EQ(0, skipList.

size()

);
EXPECT_THROW(skipList
.

peek(), std::runtime_error

);
}

