//
// Created by Lee on 2024/6/19.
//

#ifndef MYCPP_MEMORYPOOLPROFILER_H
#define MYCPP_MEMORYPOOLPROFILER_H

#include <iostream>
#include <map>
#include <string>
#include <chrono>

class MemoryPoolProfiler {
public:
    MemoryPoolProfiler(const std::string &name, bool print = false, std::ostream & = std::cout);

    ~MemoryPoolProfiler();

    static void printStatistics(std::ostream & = std::cout);

    static void getStatistics(std::ostream &);

private:
    static void addStatistic(const std::string &, int);

    std::size_t currentMilli() const;

    const std::string name_;
    bool print_;
    std::ostream &out_;
    std::chrono::time_point<std::chrono::steady_clock> tick_;

    // name -> (total time, count)
    static std::map<std::string, std::pair<int, int>> map_;
};

std::map<std::string, std::pair<int, int>> MemoryPoolProfiler::map_;

MemoryPoolProfiler::MemoryPoolProfiler(const std::string &name, bool print, std::ostream &out)
        : name_(name), print_(print), out_(out), tick_(std::chrono::steady_clock::now()) {
    if (print_) {
        out_ << "\nBegin profile. " << name_ << std::endl;
    }
}


MemoryPoolProfiler::~MemoryPoolProfiler() {
    const auto milli = currentMilli();
    if (print_) {
        out_ << "End profile [" << milli << " ms]. " << name_ << "\n" << std::endl;
    }
    addStatistic(name_, milli);
}


void MemoryPoolProfiler::printStatistics(std::ostream &os) {
    getStatistics(os);
}


void MemoryPoolProfiler::getStatistics(std::ostream &os) {
    if (map_.empty()) {
        os
                << "============================\n"
                << "== Profiler map is EMPTY! ==\n"
                << std::endl;
        return;
    }

    os
            << "==================\n"
            << "== Profiler map ==\n"
            << "\n";

    for (const auto data: map_) {
        int ms = std::get<0>(data.second);
        int count = std::get<1>(data.second);
        os
                << data.first << "\n"
                << "total ms: " << ms << "\n"
                << "count: " << count << "\n"
                << "avg: " << static_cast< double >( ms ) / count << "\n"
                << std::endl;
    }
}

void MemoryPoolProfiler::addStatistic(const std::string &name, int milli) {
    auto it = map_.find(name);
    if (it == map_.end()) {
        bool ok;
        std::tie(it, ok) = map_.emplace(name, std::make_pair(0, 0));
        if (!ok)
            return;
    }

    auto &par = it->second;
    std::get<0>(par) += milli;
    ++std::get<1>(par);
}

std::size_t MemoryPoolProfiler::currentMilli() const {
    const auto tick = std::chrono::steady_clock::now();
    return static_cast<std::size_t>( std::chrono::duration_cast<std::chrono::milliseconds>(tick - tick_).count());
}

#endif //MYCPP_MEMORYPOOLPROFILER_H
