//
// Created by Lee on 2022/9/6.
// modify by lee 2024年6月19日14:43:19
//

#ifndef MYCPPIMPLEMENT_MYALLOCATOR_H
#define MYCPPIMPLEMENT_MYALLOCATOR_H

#include <cstdint>
// implement below paper
// http://www.thinkmind.org/index.php?view=article&articleid=computation_tools_2012_1_10_80006
template<typename T>
class MemPool {
public:
    MemPool();

    ~MemPool();

    void CreatePool(uint32_t num_cells);

    void DestroyPool();

    T *Allocate();

    void Deallocate(void *p);

private:
    uint8_t *AddrFromIndex(uint32_t i) const;

    uint32_t IndexFromAddr(const uint8_t *p) const;

    const uint32_t cell_size_;
    uint32_t num_cells_;
    uint32_t num_free_cells_;
    uint32_t num_init_;
    uint8_t *mem_beg_;
    uint8_t *next_;
};

template<typename T>
MemPool<T>::MemPool()
        : cell_size_(sizeof(T)), num_cells_(0), num_free_cells_(0), num_init_(0), mem_beg_(nullptr), next_(nullptr) {
    static_assert(sizeof(uint32_t) <= sizeof(T), "sizeof(T) must be equal or greater than sizeof(uint32_t)");
}

template<typename T>
MemPool<T>::~MemPool() {
    DestroyPool();
}

template<typename T>
void MemPool<T>::CreatePool(uint32_t num_cells) {
    num_cells_ = num_cells;
    num_free_cells_ = num_cells_;
    mem_beg_ = new uint8_t[num_cells_ * cell_size_];
    next_ = mem_beg_;
}

template<typename T>
void MemPool<T>::DestroyPool() {
    delete[] mem_beg_;
    mem_beg_ = nullptr;
}

template<typename T>
T *MemPool<T>::Allocate() {
    if (num_init_ < num_cells_) {
        uint32_t *p = reinterpret_cast<uint32_t *>(AddrFromIndex(num_init_));
        *p = ++num_init_;
    }

    T *res = nullptr;

    if (num_free_cells_ > 0) {
        res = reinterpret_cast<T *>(next_);

        if (--num_free_cells_ > 0) {
            next_ = AddrFromIndex(*reinterpret_cast<uint32_t *>(next_));
        } else {
            next_ = nullptr;
        }
    }

    return res;
}

template<typename T>
void MemPool<T>::Deallocate(void *p) {
    *static_cast<uint32_t *>(p) = next_ == nullptr ? num_cells_ : IndexFromAddr(next_);
    next_ = static_cast<uint8_t *>(p);
    ++num_free_cells_;
}

template<typename T>
uint8_t *MemPool<T>::AddrFromIndex(uint32_t i) const {
    return mem_beg_ + (i * cell_size_);
}

template<typename T>
uint32_t MemPool<T>::IndexFromAddr(const uint8_t *p) const {
    return static_cast<uint32_t>(p - mem_beg_) / cell_size_;
}

//#include <malloc.h>
//#include <stdlib.h>
//
//typedef void (*ALLOC_FUN)();
//
//template<int inst>          // 非类型模板参数
//class MallocAllocTemplate { // 一级空间配置器(malloc, free, realloc)
//public:
//    static void *Allocate(size_t n) {
//        void *ret = malloc(n);
//        if (0 == ret) {
//            ret = OomMalloc(n);
//        }
//        return ret;
//    }
//
//    static void Deallocate(void *p) {
//        free(p);
//    }
//
//    static void *Reallocate(void *p, size_t new_size) {
//        void *ret = realloc(p, new_size);
//        if (ret == 0)
//            ret = OomRealloc(p, new_size);
//        return ret;
//    }
//
//private:
//    static void *OomMalloc(size_t n) { // 调用自定义的句柄处理函数释放并分配内存
//        ALLOC_FUN handler;
//        void *ret;
//        while (1) {
//            handler = MallocAllocHandler;
//            if (0 == handler) {
//                LOG_WARN("Out of memory");
//                exit(-1);
//            }
//            handler();
//            ret = malloc(n);
//            if (ret) {
//                return ret;
//            }
//        }
//    }
//
//    static void *OomRealloc(void *p, size_t new_size) {
//        ALLOC_FUN handler;
//        void *ret;
//        while (1) {
//            handler = MallocAllocHandler;
//            if (0 == handler) {
//                exit(-1);
//            }
//            handler();
//            ret = realloc(p, new_size);
//            if (ret) {
//                return ret;
//            }
//        }
//    }
//
//    static void (*SetMallocHandler(void(*f)()))(); // 设置操作系统分配内存失败时的句柄处理函数
//    static ALLOC_FUN MallocAllocHandler;
//};
//
//template<int inst>
//ALLOC_FUN MallocAllocTemplate<inst>::MallocAllocHandler = 0; // 句柄函数初始化为 0

#endif //MYCPPIMPLEMENT_MYALLOCATOR_H
