//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYPROFILE_H
#define MYCPPIMPLEMENT_MYPROFILE_H

#include <chrono>
#include <functional>

using Seconds = std::chrono::duration<double>;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::chrono::duration_cast;

// Measure how much time the given function takes to execute using chrono
// Pass the function name, then all relevant arguments, including the object as the first if it's a member function

template<typename Function, typename... Args>
Seconds measure(Function&& toTime, Args&&... a) {
    auto start{std::chrono::steady_clock::now()};                      // Start timer
    std::invoke(std::forward<Function>(toTime), std::forward<Args>(a)...);  // Forward and call
    auto stop{std::chrono::steady_clock::now()};                       // Stop timer
    return (stop - start);
}


#endif //MYCPPIMPLEMENT_MYPROFILE_H
