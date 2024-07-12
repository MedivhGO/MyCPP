//
// Created by Lee on 2024/7/8.
//

#include <gtest/gtest.h>

#include <future>
#include <thread>
//#include <semaphore>
#include <chrono>
#include <iostream>

void hello() {
  std::cout << "Hello Concurrent World!\n" << std::this_thread::get_id() << std::endl;
}

TEST(MyCppConcurrency, test1) {
  std::thread t(hello);
  std::cout << t.get_id() << std::endl;
  t.join();
}

TEST(MyCppConcurrency, test2) {
    auto now = std::chrono::system_clock::now();
    std::cout << "Current time: " << now.time_since_epoch().count() << " milliseconds since epoch" << std::endl;
}

class thread_guard {
public:
    explicit thread_guard(std::thread& t) : t_(t) {}
    ~thread_guard() {
        if (t_.joinable()) {
            t_.join();
        }
    }
    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;
private:
    std::thread& t_;
};

TEST(MyCppConcurrency, test3) {
    std::thread t(hello);
    thread_guard g(t);
}

TEST(MyCppConcurrency, test4) {
    // std::promise 常配合 std::future 使用。
    // 其作用是在一个线程中保存一个类型的值，可供相绑定的 std::future 对象在另一个线程中获取。
    std::packaged_task<int()> task([] { return 7; });
    std::future<int> f1 = task.get_future();
    std::thread t(std::move(task));

    std::future<int> f2 = std::async(std::launch::async, [] { return 8; });

    std::promise<int> p;                  // 声明一个 promise 对象 p，其保存的值的类型为 int。
    std::future<int> f3 = p.get_future(); // 声明一个 std::future 对象 f3，并通过 std::promise 的 get_future() 函数与 p 绑定。
    std::thread([&p] { p.set_value_at_thread_exit(9); }).detach();

    std::cout << "Waiting..." << std::flush;

    f1.wait();
    f2.wait();
    f3.wait();

    std::cout << "Done!\n Result are:" << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';

    t.join();
}

// std::counting_semaphore<1> sem(1);

// void thread_function() {
//  sem.acquire();
//  std::cout << "Thread " << std::this_thread::get_id()
//      << " is accessing the shared resource." << std::endl;
//  std::this_thread::sleep_for(std::chrono::seconds(1));
//  sem.release();
//}

// TEST(MyCppConcurrency, test5) {
//  std::thread t1(thread_function);
//  std::thread t2(thread_function);

//  t1.join();
//  t2.join();
//}

void func(std::promise<int> result_promise) noexcept {
    result_promise.set_value(42);
}

TEST(MyCppConcurrency, test6) {
    std::promise<int> result_promise;
    std::future<int> result_future = result_promise.get_future();
    std::thread t{func, std::move(result_promise)};
    thread_guard g(t);
    EXPECT_EQ(result_future.get(), 42);
}

void accm(int& count) {
    count++;
}

TEST(MyCppConcurrency, test7) {
    int count = 0;
    std::once_flag of;
    for (int i = 0; i < 10; ++i) {
        std::call_once(of, accm, count);
    }
    EXPECT_EQ(count, 1);
}

TEST(MyCppConcurrency, test8) {
    thread_local int a = 10;
    auto count_func = [&](int id) {
        std::cout << "Thread " << id << " starting." << std::endl;
        a++;
        std::cout << "Thread " << id << " counter: " << a << std::endl;
    };
    for (int i = 0; i < 10; ++i) {
        std::thread t(count_func, i);
        thread_guard g(t);
    }
    EXPECT_EQ(a, 10);
}



