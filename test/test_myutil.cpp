//
// Created by Lee on 2022/9/3.
//

#include <gtest/gtest.h>
#include <iostream>

#include "MyRandom.h"
#include "MyProfile.h"
#include "MySingleton.h"
#include "MyString.h"
#include "MyThread.h"

using std::cout;
using std::endl;

TEST(MyUtil, test_random) {
    RandomNumberGenerator rg(10,2000);
    cout << rg.GetRandomNumberByMt19937() << endl;
    cout << rg.GetRandomNumberByDefault() << endl;
}

TEST(MyUtil, test_random_profile) {
    RandomNumberGenerator rg(-1000,1000);
    auto default_generate = [&]() {
        for (int i = 0; i < 100000; ++i) {
            rg.GetRandomNumberByDefault();
        }
    };

    auto mt19937_generate = [&]() {
        for (int i = 0; i < 100000; ++i) {
            rg.GetRandomNumberByMt19937();
        }
    };
    cout << measure(default_generate).count() << endl;
    cout << measure(mt19937_generate).count() << endl;
}

TEST(MyUtil, test_singleton) {
    MyString& a = Singleton<MyString>::getInstance();
    MyString& b = Singleton<MyString>::getInstance();
    EXPECT_EQ(&a, &b);
    MyString* p = SingleDemo<MyString>::getInstance();
    MyString* pp = SingleDemo<MyString>::getInstance();
    EXPECT_EQ(p, pp);
}

TEST(MyUtil, test_thread) {
    MyString* str_p = SingleDemo<MyString>::getInstance();
    auto test_call = [](MyString* str_p, uint64_t thread_itr = 0) {
        MyString* t = SingleDemo<MyString>::getInstance();
        EXPECT_EQ(str_p, t);
    };
    LaunchParallelTest(1000, test_call,str_p);
}


