//
// Created by Lee on 2022/9/6.
//

#ifndef MYCPPIMPLEMENT_MYALLOCATOR_H
#define MYCPPIMPLEMENT_MYALLOCATOR_H

#include <malloc.h>
#include <stdlib.h>

#include "MyLog.h"

typedef void (*ALLOC_FUN)();

template<int inst> // 非类型模板参数
class MallocAllocTemplate // 一级空间配置器(malloc, free, realloc)
{
public:
    static void *Allocate(size_t n) {
        void *ret = malloc(n);
        if (0 == ret)
            ret = OomMalloc(n);
        return ret;
    }

    static void Deallocate(void *p) {
        free(p);
    }

    static void *Reallocate(void *p, size_t new_size) {
        void *ret = realloc(p, new_size);
        if (ret == 0)
            ret = OomRealloc(p, new_size);
        return ret;
    }

private:
    static void *OomMalloc(size_t n) {  // 调用自定义的句柄处理函数释放并分配内存
        ALLOC_FUN handler;
        void *ret;
        while (1) {
            handler = MallocAllocHandler;
            if (0 == handler) {
                LOG_WARN("Out of memory");
                exit(-1);
            }
            handler();
            ret = malloc(n);
            if (ret) {
                return ret;
            }
        }
    }

    static void *OomRealloc(void *p, size_t new_size) {
        ALLOC_FUN handler;
        void *ret;
        while (1) {
            handler = MallocAllocHandler;
            if (0 == handler) {
                LOG_WARN("Out of memory");
                exit(-1);
            }
            handler();
            ret = realloc(p, new_size);
            if (ret) {
                return ret;
            }
        }
    }

    static void (*SetMallocHandler(void(*f)()))(); // 设置操作系统分配内存失败时的句柄处理函数
    static ALLOC_FUN MallocAllocHandler;
};

template<int inst>
ALLOC_FUN MallocAllocTemplate<inst>::MallocAllocHandler = 0; // 句柄函数初始化为0

#endif //MYCPPIMPLEMENT_MYALLOCATOR_H
