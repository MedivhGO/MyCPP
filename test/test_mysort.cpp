//
// Created by Lee on 2022/9/13.
//
#include <gtest/gtest.h>

#include "Sort/BubbleSort.h"
#include "Sort/HeapSort.h"
#include "Sort/InsertSort.h"
#include "Sort/MergeSort.h"
#include "Sort/QuickSort.h"
#include "Sort/SelectSort.h"
#include "Sort/BinarySearch.h"
#include "MyRandom.h"
#include "MyProfile.h"

TEST(MySort, test_bubblesort) {
    std::vector<int> ivec = {4, 3, 2, 1, 5};
    BubbleSort(ivec, ivec.size());
    std::vector<int> res_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(ivec, res_vec);
}

TEST(MySort, test_heapsort) {
    RandomNumberGenerator rng(-100000, 100000);
    vector<int> test_dataset;
    for (int i = 0; i < 10000; ++i) {
        test_dataset.push_back(rng.GetRandomNumberByMt19937());
    }
    std::vector<int> sorted_dataset = test_dataset;
    auto run_stdsort = [&]() {
        std::sort(sorted_dataset.begin(), sorted_dataset.end());
    };
    auto run_mysort = [&]() {
        HeapSort(test_dataset, test_dataset.size());
    };
    run_stdsort();
    run_mysort();
    EXPECT_EQ(sorted_dataset, test_dataset);
}

TEST(MySort, test_insertsort) {
    std::vector<int> ivec = {4, 3, 2, 1, 5};
    InsertSort(ivec, ivec.size());
    std::vector<int> res_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(ivec, res_vec);
}

TEST(MySort, test_mergesort) {
    std::vector<int> ivec = {4, 3, 2, 1, 5};
    MergeSort(ivec, 0, ivec.size());
    std::vector<int> res_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(ivec, res_vec);
}

TEST(MySort, test_quicksort) {
    RandomNumberGenerator rng(-100000, 100000);
    vector<int> test_dataset;
    for (int i = 0; i < 10000; ++i) {
        test_dataset.push_back(rng.GetRandomNumberByMt19937());
    }
    std::vector<int> sorted_dataset = test_dataset;
    auto run_stdsort = [&]() {
        std::sort(sorted_dataset.begin(), sorted_dataset.end());
    };
    auto run_mysort = [&]() {
        QuickSort(test_dataset, 0, test_dataset.size());
    };
    run_stdsort();
    run_mysort();
    EXPECT_EQ(sorted_dataset, test_dataset);
}

TEST(MySort, test_quicksortmedianofthree) {
    RandomNumberGenerator rng(-100000, 100000);
    vector<int> test_dataset;
    for (int i = 0; i < 1000000; ++i) {
        test_dataset.push_back(rng.GetRandomNumberByMt19937());
    }
    std::vector<int> sorted_dataset = test_dataset;
    auto res1 = measure(QuickSort,test_dataset, 0, test_dataset.size()).count();
    auto res2 = measure(QuickSortMOT,sorted_dataset, 0, sorted_dataset.size()).count();
    EXPECT_LT(res1, res2);
    EXPECT_EQ(sorted_dataset, test_dataset);
}

TEST(MySort, test_selectsort) {
    std::vector<int> ivec = {4, 3, 2, 1, 5};
    SelectSort(ivec, ivec.size());
    std::vector<int> res_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(ivec, res_vec);
}

TEST(MySort, test_binarysearch) {
    std::vector<int> ivec = {4, 3, 2, 1, 5};
    int res = BinarySearch(ivec, 10);
    EXPECT_EQ(res, -1);
}