//
// Created by Allen on 2018/10/23.
//

#ifndef APUE_SELECTSORT_H
#define APUE_SELECTSORT_H

#include <vector>

void SelectSort(std::vector<int> &data, int len) {
    if (len == 0) {
        return;
    }
    for (int i = 0; i < len - 1; ++i) { // 为前 len-1 个位确定合适的key
        int min = i;
        for (int j = i + 1; j < len; ++j) { // 从无序集合中选出一个最小的index
            if (data[j] < data[min]) {
                min = j;
            }
        }
        if (min != i) { // 把这个最小的 key 放在当前需要确定的 slot 上
            swap(data[min], data[i]);
        }
    }
}

// 时间复杂度 O(n^2)
// 空间复杂度 O(1)
// 稳定的排序

#endif // APUE_SELECTSORT_H
