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

TEST(MySort, test_bubblesort) {
    std::vector<int> ivec = {4, 3, 2, 1, 5};
    BubbleSort(ivec, ivec.size());
    std::vector<int> res_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(ivec, res_vec);
}

TEST(MySort, test_heapsort) {
    std::vector<int> ivec = {4, 3, 2, 1, 5};
    HeapSort(ivec, ivec.size());
    std::vector<int> res_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(ivec, res_vec);
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
    std::vector<int> ivec = {4, 3, 2, 1, 5};
    QuickSort(ivec, 0, ivec.size());
    std::vector<int> res_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(ivec, res_vec);
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