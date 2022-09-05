#include <gtest/gtest.h>

#include "MySharedPtr.h"
#include "MyUniquePtr.h"

TEST(MySmartPtrTest, TEST1) {
    MyUniquePtr<int> ptr1{new int{10}};
    EXPECT_EQ(*ptr1.get(), 10);

    MyUniquePtr<std::string> ptr2{new std::string{"hello world!"}};
    EXPECT_EQ(*ptr2.get(), "hello world!");
}

TEST(MySmartPtrTest, TEST2) {
    MyUniquePtr<int> ptr1{my_make_unique<int>(10)};
    EXPECT_EQ(*ptr1.get(), 10);

    MyUniquePtr<std::string> ptr2{my_make_unique<std::string>("hello world!")};
    EXPECT_EQ(*ptr2.get(), "hello world!");
}

TEST(MySmartPtrTest, TEST3) {
    MyUniquePtr<int> ptr1;
    EXPECT_EQ(ptr1.get(), nullptr);

    MyUniquePtr<std::string> ptr2{};
    EXPECT_EQ(ptr2.get(), nullptr);
}

TEST(MySmartPtrTest, TEST4) {
    MyUniquePtr<int> ptr1{new int{10}};
    EXPECT_EQ(*ptr1, 10);

    MyUniquePtr<std::string> ptr2{new std::string{"hello world!"}};
    EXPECT_EQ(*ptr2, "hello world!");

    MyUniquePtr<std::vector<int>> ptr3{new std::vector<int>{1, 2, 3, 4, 5}};
    EXPECT_EQ((*ptr3)[1], 2);
}

TEST(MySmartPtrTest, TEST5) {
    MyUniquePtr<std::string> ptr1{new std::string{"hello world!"}};
    EXPECT_EQ(ptr1->length(), 12);

    MyUniquePtr<std::vector<int>> ptr3{new std::vector<int>{1, 2, 3, 4, 5}};
    EXPECT_FALSE(ptr3->empty());
}

TEST(MySmartPtrTest, TEST6) {
    MyUniquePtr<std::string> ptr{new std::string{"hello world!"}};
    EXPECT_EQ(ptr->length(), 12);
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST(MySmartPtrTest, TEST7) {
    MyUniquePtr<std::string> ptr{new std::string{"hello world!"}};
    EXPECT_EQ(ptr->length(), 12);
    ptr.reset(new std::string{"nice job!"});
    EXPECT_EQ(*ptr, "nice job!");
}

TEST(MySmartPtrTest, TEST8) {
    MyUniquePtr<double> ptr{new double{1.567}};
    EXPECT_DOUBLE_EQ(*ptr, 1.567);
    if (ptr) {
        ptr.reset();
    }
    EXPECT_FALSE(ptr);
}

TEST(MySmartPtrTest, TEST9) {
    MyUniquePtr<double> ptr{new double{1.567}};
    EXPECT_DOUBLE_EQ(*ptr, 1.567);
    double *tmp;
    if (ptr) {
        tmp = ptr.release();
    }
    EXPECT_DOUBLE_EQ(*tmp, 1.567);
    EXPECT_FALSE(ptr);
    delete tmp;
}

TEST(MySmartPtrTest, TEST10) {
    MySharedPtr<int> ptr1{new int{10}};
    EXPECT_EQ(*ptr1.get(), 10);

    MySharedPtr<std::string> ptr2{new std::string{"hello world!"}};
    EXPECT_EQ(*ptr2.get(), "hello world!");
}

TEST(MySmartPtrTest, TEST11) {
    MySharedPtr<int> ptr1{my_make_shared<int>(10)};
    EXPECT_EQ(*ptr1.get(), 10);

    MySharedPtr<std::string> ptr2{my_make_shared<std::string>("hello world!")};
    EXPECT_EQ(*ptr2.get(), "hello world!");
}

TEST(MySmartPtrTest, TEST12) {
    MySharedPtr<int> ptr1;
    EXPECT_EQ(ptr1.get(), nullptr);

    MySharedPtr<std::string> ptr2{};
    EXPECT_EQ(ptr2.get(), nullptr);
}

TEST(MySmartPtrTest, TEST13) {
    MySharedPtr<int> ptr1{my_make_shared<int>(10)};
    MySharedPtr<int> ptr2{ptr1};
    EXPECT_EQ(*ptr1, 10);
    EXPECT_EQ(*ptr2, 10);
    EXPECT_EQ(ptr1.get(), ptr2.get());
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}

TEST(MySmartPtrTest, TEST14) {
    MySharedPtr<std::string> ptr1{my_make_shared<std::string>("hello world!")};
    MySharedPtr<std::string> ptr2{ptr1};
    MySharedPtr<std::string> ptr3{ptr1};
    EXPECT_EQ(*ptr1, "hello world!");
    EXPECT_EQ(*ptr1, "hello world!");
    EXPECT_EQ(*ptr2, "hello world!");
    EXPECT_EQ(ptr1.get(), ptr2.get());
    EXPECT_EQ(ptr2.get(), ptr3.get());
    EXPECT_EQ(ptr1.use_count(), 3);
    EXPECT_EQ(ptr2.use_count(), 3);
    EXPECT_EQ(ptr3.use_count(), 3);
}

TEST(MySmartPtrTest, TEST15) {
    MySharedPtr<std::string> ptr1{my_make_shared<std::string>("hello world!")};
    {
        {
            MySharedPtr<std::string> ptr2{ptr1};
            EXPECT_EQ(ptr1.use_count(), 2);
            EXPECT_EQ(ptr2.use_count(), 2);
        }
        EXPECT_EQ(ptr1.use_count(), 1);
        MySharedPtr<std::string> ptr3{ptr1};
        EXPECT_EQ(ptr1.use_count(), 2);
        EXPECT_EQ(ptr3.use_count(), 2);
    }
    EXPECT_EQ(ptr1.use_count(), 1);
    ptr1.~MySharedPtr();
    EXPECT_EQ(ptr1.use_count(), 0);
    EXPECT_EQ(ptr1.get(), nullptr);
}

TEST(MySmartPtrTest, TEST16) {
    MySharedPtr<int> ptr1{new int{10}};
    EXPECT_EQ(*ptr1, 10);

    MySharedPtr<std::string> ptr2{new std::string{"hello world!"}};
    EXPECT_EQ(*ptr2, "hello world!");

    MySharedPtr<std::vector<int>> ptr3{new std::vector<int>{1, 2, 3, 4, 5}};
    EXPECT_EQ((*ptr3)[1], 2);
}

TEST(MySmartPtrTest, TEST17) {
    MySharedPtr<std::string> ptr1{new std::string{"hello world!"}};
    EXPECT_EQ(ptr1->length(), 12);

    MySharedPtr<std::vector<int>> ptr3{new std::vector<int>{1, 2, 3, 4, 5}};
    EXPECT_FALSE(ptr3->empty());
}

TEST(MySmartPtrTest, TEST18) {
    MySharedPtr<std::string> ptr{new std::string{"hello world!"}};
    EXPECT_EQ(ptr->length(), 12);
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.use_count(), 0);
}

TEST(MySmartPtrTest, TEST19) {
    MySharedPtr<std::string> ptr{new std::string{"hello world!"}};
    EXPECT_EQ(ptr->length(), 12);
    ptr.reset(new std::string{"nice job!"});
    EXPECT_EQ(*ptr, "nice job!");
    EXPECT_EQ(ptr.use_count(), 1);
}

TEST(MySmartPtrTest, TEST20) {
    MySharedPtr<double> ptr{new double{1.567}};
    EXPECT_DOUBLE_EQ(*ptr, 1.567);
    if (ptr)
        ptr.reset();
    EXPECT_FALSE(ptr);
}

TEST(MySmartPtrTest, TEST21) {
    MySharedPtr<double> ptr1{new double{1.567}};
    MySharedPtr<double> ptr2{new double{5.1234}};
    ptr1 = ptr1;
    ptr2 = ptr1;
    EXPECT_DOUBLE_EQ(*ptr2, 1.567);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}

TEST(MySmartPtrTest, TEST22) {
    MyUniquePtr<double> ptr{new double{1.567}};
    EXPECT_DOUBLE_EQ(*ptr, 1.567);
    MyUniquePtr<double> mup(std::move(ptr));
    EXPECT_DOUBLE_EQ(*mup, 1.567);
    EXPECT_FALSE(ptr);
}

TEST(MySmartPtrTest, TEST23) {
    MyUniquePtr<double> ptr{new double{1.567}};
    EXPECT_DOUBLE_EQ(*ptr, 1.567);
    MyUniquePtr<double> mup;
    mup = std::move(ptr);
    EXPECT_DOUBLE_EQ(*mup, 1.567);
    EXPECT_FALSE(ptr);
}


TEST(MySmartPtrTest, TEST24) {
    auto ptr1 = MySharedPtr<int>(new int(120));
    auto ptr2 = MySharedPtr<int>(ptr1);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    ptr1.reset();
    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(*(ptr2), 120);
    MySharedPtr<int> ptr;
    EXPECT_EQ(ptr, nullptr);
}

TEST(MySmartPtrTest, TEST25) {
    MySharedPtr<std::string> ptr1(new std::string("hello"));
    MyWeakPtr<std::string> ptr2(ptr1);
    MySharedPtr<std::string> ptr3 = ptr2.lock();
    EXPECT_EQ(ptr3.use_count(), 2);
    EXPECT_EQ(ptr1, ptr3);
    EXPECT_EQ(*ptr1, "hello");
    MySharedPtr<std::string> ptr4;
    EXPECT_EQ(!ptr4, true);
    ptr4 = ptr3;
    EXPECT_EQ(ptr3.use_count(), 3);
}