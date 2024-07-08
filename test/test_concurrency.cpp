//
// Created by Lee on 2024/7/8.
//

#include <gtest/gtest.h>

#include <future>
#include <algorithm>
#include <thread>
#include <semaphore>
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

