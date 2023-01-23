//
// Created by Lee on 2022/9/6.
//

#ifndef MYCPPIMPLEMENT_MYCACHE_H
#define MYCPPIMPLEMENT_MYCACHE_H

#include <unordered_map>
#include <thread>
#include <memory>

#include "MutexLock.h"

template<typename T>
class Cache {
public:
    std::shared_ptr<const T> FastLoadT(std::thread::id id);
    int GetCacheSize() { return cache_.size(); }
private:
    auto LoadT(std::thread::id id) -> std::unique_ptr<const T>;

    MutexLock m_;
    std::unordered_map<std::thread::id, std::weak_ptr<const T>> cache_;
};

template<typename  T>
auto Cache<T>::LoadT(std::thread::id id) -> std::unique_ptr<const T> {
    if (std::is_same<T, int>::value) {
        return std::make_unique<const int>(9999);
    }
    return std::make_unique<const T>();
}

template<typename  T>
auto Cache<T>::FastLoadT(std::thread::id id) -> std::shared_ptr<const T> {
    MutexLockGuard lm(m_);
    auto obj_ptr = cache_[id].lock();
    if (!obj_ptr) {
        obj_ptr = LoadT(id);
        cache_[id] = obj_ptr;
    }
    return obj_ptr;
}

#endif //MYCPPIMPLEMENT_MYCACHE_H
