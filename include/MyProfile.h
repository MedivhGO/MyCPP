//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYPROFILE_H
#define MYCPPIMPLEMENT_MYPROFILE_H

#include <chrono>
#include <functional>

#ifdef WIN32
#include <Windows.h>
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")
#else

#include <sys/resource.h>
#include <sys/time.h>

#endif

using Seconds = std::chrono::duration<double>;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::chrono::duration_cast;

// Measure how much time the given function takes to execute using chrono
// Pass the function name, then all relevant arguments, including the object as the first if it's a member function

template<typename Function, typename... Args>
Seconds measure(Function &&toTime, Args &&... a) {
    auto start{std::chrono::steady_clock::now()};                           // Start timer
    std::invoke(std::forward<Function>(toTime), std::forward<Args>(a)...);  // Forward and call
    auto stop{std::chrono::steady_clock::now()};                            // Stop timer
    return (stop - start);
}

class MemProfiler {
public:
    enum class MemoryUnit {
        KB_, MB_, GB_
    };
public:
    static size_t memory(MemoryUnit mu = MemoryUnit::KB_) { //查询当前程序的内存使用量
        size_t memory = 0;
#ifdef WIN32
        PROCESS_MEMORY_COUNTERS pmc;
        HANDLE hProcess = GetCurrentProcess();
        if (!GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
            throw std::runtime_error("GetProcessMemoryInfo failed");
        CloseHandle(hProcess);
#else
        struct rusage usage{};
        if (getrusage(RUSAGE_SELF, &usage) == -1) {
            throw std::runtime_error("getrusage failed");
        }
        memory = usage.ru_maxrss / 1024; // 如果某些linux平台不完全支持 getrusage 则 ru_maxrss 总是返回0
#endif
        switch (mu) {
            case MemoryUnit::KB_:
                memory = memory * 1024;
                break;
            case MemoryUnit::MB_:
                memory = memory;
                break;
            case MemoryUnit::GB_:
                memory = memory / 1024;
                break;
        }
        return memory;
    }
};


#endif //MYCPPIMPLEMENT_MYPROFILE_H
