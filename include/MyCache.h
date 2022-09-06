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
    std::shared_ptr<const T> fastLoadT(std::thread::id id);
    int getCacheSize() { return cache.size(); }
private:
    std::unique_ptr<const T> loadT(std::thread::id id);

    MutexLock m;
    std::unordered_map<std::thread::id, std::weak_ptr<const T>> cache;
};

template<typename  T>
std::unique_ptr<const T> Cache<T>::loadT(std::thread::id id) {
    if (std::is_same<T, int>::value) {
        return std::make_unique<const int>(9999);
    }
    return std::make_unique<const T>();
}

template<typename  T>
std::shared_ptr<const T> Cache<T>::fastLoadT(std::thread::id id) {
    MutexLockGuard lm(m);
    auto objPtr = cache[id].lock();
    if (!objPtr) {
        objPtr = loadT(id);
        cache[id] = objPtr;
    }
    return objPtr;
}

#endif //MYCPPIMPLEMENT_MYCACHE_H
