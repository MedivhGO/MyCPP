#include <iostream>
#include "MySharedPtr.h"

#include <gtest/gtest.h>

class PtrFixture : public :: testing::Test{



};
using namespace std;
TEST_F(PtrFixture, init)
{
    auto ptr1 = new MySharedPtr<int>(new int(120));
    auto ptr2 = new MySharedPtr<int>(*ptr1);
    EXPECT_EQ(ptr1->use_count(), 2);
    EXPECT_EQ(ptr2->use_count(), 2);
    delete ptr1;
    EXPECT_EQ(ptr2->use_count(), 1);
    EXPECT_EQ(*(*ptr2), 120);
    MySharedPtr<int> ptr;
    EXPECT_EQ(ptr, nullptr);

}

TEST_F(PtrFixture, weak_ptr)
{
    MySharedPtr<string> ptr1(new string("hello"));
    EXPECT_EQ(ptr1.unique(), true);
    MyWeakPtr<string> ptr2(ptr1);
    MySharedPtr<string> ptr3 = ptr2.lock();
    EXPECT_EQ(ptr3.use_count(), 2);
    EXPECT_EQ(ptr1, ptr3);
    ptr3->push_back('1');
    EXPECT_EQ(*ptr1, "hello1");
    MySharedPtr<string> ptr4;
    EXPECT_EQ(!ptr4, true);
    ptr4 = ptr3;
    EXPECT_EQ(ptr3.use_count(), 3);
}