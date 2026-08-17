//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYTHREAD_H
#define MYCPPIMPLEMENT_MYTHREAD_H

#include <functional>
#include <thread>

template <typename... Args>
void LaunchParallelTest(uint64_t num_threads, Args &&... args) {
    std::vector<std::thread> thread_group;

    // Launch a group of threads
    for (uint64_t thread_itr = 0; thread_itr < num_threads; ++thread_itr) {
        thread_group.push_back(std::thread(args..., thread_itr));
    }

    // Join the threads with the main thread
    for (uint64_t thread_itr = 0; thread_itr < num_threads; ++thread_itr) {
        thread_group[thread_itr].join();
    }
}

#endif //MYCPPIMPLEMENT_MYTHREAD_H
