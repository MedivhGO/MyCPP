//
// Created by Lee on 2022/9/3.
//

#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <string>

#include "MyRandom.h"
#include "MyProfile.h"
#include "MySingleton.h"
#include "MyString.h"
#include "MyThread.h"

using std::cout;
using std::endl;

TEST(MyUtil, test_random) {
    RandomNumberGenerator rg(10,2000);
    EXPECT_GT(rg.GetRandomNumberByMt19937(), 10);
    EXPECT_LT(rg.GetRandomNumberByDefault(), 2000);
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

    EXPECT_LT(measure(mt19937_generate).count(), measure(default_generate).count());
}

TEST(MyUtil, test_singleton) {
    MyString& a = Singleton<MyString>::getInstance();
    MyString& b = Singleton<MyString>::getInstance();
    EXPECT_EQ(&a, &b);
    std::shared_ptr<MyString> p = SingleDemo<MyString>::getInstance();
    std::shared_ptr<MyString> pp = SingleDemo<MyString>::getInstance();
    EXPECT_EQ(p, pp);
}

TEST(MyUtil, test_thread) {
    std::shared_ptr<MyString> str_p = SingleDemo<MyString>::getInstance();
    auto test_call = [](std::shared_ptr<MyString> str_p, uint64_t thread_itr = 0) {
        std::shared_ptr<MyString> t = SingleDemo<MyString>::getInstance();
        EXPECT_EQ(str_p, t);
    };
    LaunchParallelTest(100, test_call,str_p);
}

TEST(MyUtil, test_mutex) {
    MutexLock m;
    const int atomic_str_length = 10;
    const int test_cycle = 100;
    int failure_time_without_lock = 0;
    int failure_time_with_lock = 0;
    for (int tc = 0; tc < test_cycle; ++tc) {
        std::string expect_str;
        for (int i = 0; i < atomic_str_length; ++i) {
            expect_str += "0123456789";
        }

        std::string res_str_without_lock;
        auto multi_add_without_lock = [&](uint64_t thread_itr = 0) {
            for (int i = 0; i < 10; ++i) {
                res_str_without_lock += std::to_string(i);
            }
        };

        std::string res_str_with_lock;
        auto multi_add_with_lock = [&](uint64_t thread_itr = 0) {
            m.lock();
            for (int i = 0; i < 10; ++i) {
                res_str_with_lock += std::to_string(i);
            }
            m.unlock();
        };

        LaunchParallelTest(atomic_str_length, multi_add_without_lock);
        LaunchParallelTest(atomic_str_length, multi_add_with_lock);

        if (expect_str != res_str_without_lock) {
            failure_time_without_lock++;
        }

        if (expect_str != res_str_with_lock) {
            failure_time_with_lock++;
        }
    }

    EXPECT_GT(failure_time_without_lock, 0);
    EXPECT_EQ(failure_time_with_lock, 0);
}


