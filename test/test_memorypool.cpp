//
// Created by Lee on 2024/6/19.
//
#include <gtest/gtest.h>
#include "MyAllocator.h"
#include "MemoryPoolProfiler.h"

TEST(MyMemoryPool, test_random) {
    MemPool<uint32_t> mp;
    mp.CreatePool(4);

    uint32_t *p0 = mp.Allocate();
    uint32_t *p1 = mp.Allocate();
    uint32_t *p2 = mp.Allocate();
    mp.Deallocate(p0);
    mp.Deallocate(p2);
    uint32_t *p3 = mp.Allocate();
    uint32_t *p4 = mp.Allocate();
    uint32_t *p5 = mp.Allocate();
    mp.Deallocate(p1);

    p0 = p1 = p2 = p3 = p4 = p5 = nullptr;
    mp.DestroyPool();

    const int num = 10000;
    const int size = 1000;
    const int tot = 4 * size * num;
    struct S {
        uint32_t i_;
        uint8_t c_;
        char str_[300];
    };
    using Type = S;
    //using Type = uint16_t; // <- will not compile due to static_assert in MemPool ctor

    {
        MemoryPoolProfiler prof("new", true);
        Type *arr[size];

        for (int n = 0; n < num; ++n) {
            for (int i = size - 1; 0 <= i; --i) {
                arr[i] = new Type;
            }

            for (int i = size - 1; 0 <= i; --i) {
                delete arr[i];
            }

            for (auto & i : arr) {
                i = new Type;
            }

            for (auto & i : arr) {
                delete i;
            }
        }
    }

    {
        MemoryPoolProfiler prof("pool", true);
        Type *arr[size];
        MemPool<Type> mp;
        mp.CreatePool(size);

        for (int n = 0; n < num; ++n) {
            for (int i = size - 1; 0 <= i; --i) {
                arr[i] = mp.Allocate();
            }

            for (int i = size - 1; 0 <= i; --i) {
                mp.Deallocate(arr[i]);
            }

            for (auto & i : arr) {
                i = mp.Allocate();
            }

            for (auto & i : arr) {
                mp.Deallocate(i);
            }
        }

        mp.DestroyPool();
    }
}
