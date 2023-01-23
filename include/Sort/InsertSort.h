//
// Created by Allen on 2018/10/23.
// Modify by Allen on 2022年9月13日14:06:58
//

#ifndef APUE_INSERTSORT_H
#define APUE_INSERTSORT_H

#include <vector>

void InsertSort(std::vector<int> &data, int len) {
    if (len == 0) {
        return;
    }
    int key;
    for (int i = 1; i < len; ++i) {
        if (data[i] < data[i - 1]) {
            key = data[i]; // 在有序集合中寻找合适的位置
            int j = i - 1;
            while(j >= 0 && data[j] > key) { // 找第一个比 key 小的数，
                data[j+1] = data[j];
                j--;
            }
            data[j+1] = key;
        }
    }
}

// 时间复杂度 O(n^2)
// 空间复杂度 O(1)
// 稳定排序
// 由于在像有序集合中插入新元素时，当遇到相同的元素，就会在这个元素的后面放置待插入元素，所以排序前后
// 相同元素的相对位置是不变的.

#endif // APUE_INSERTSORT_H
