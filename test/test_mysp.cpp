#include <gtest/gtest.h>

#include "MySharedPtr_1.h"


TEST(MySmartPtrTest, DISABLED_shared_ptr) {
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

TEST(MySmartPtrTest, DISABLED_weak_ptr) {
    MySharedPtr<std::string> ptr1(new std::string("hello"));
    EXPECT_EQ(ptr1.unique(), true);
    MyWeakPtr<std::string> ptr2(ptr1);
    MySharedPtr<std::string> ptr3 = ptr2.lock();
    EXPECT_EQ(ptr3.use_count(), 2);
    EXPECT_EQ(ptr1, ptr3);
    ptr3->push_back('1');
    EXPECT_EQ(*ptr1, "hello1");
    MySharedPtr<std::string> ptr4;
    EXPECT_EQ(!ptr4, true);
    ptr4 = ptr3;
    EXPECT_EQ(ptr3.use_count(), 3);
}

TEST(MySmartPtrTest, DISABLED_unique_ptr) {
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

TEST(MySmartPtrTest, DISABLED_circle_ref) {
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

TEST(MySmartPtrTest, DISABLED_resolve_circle) {
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