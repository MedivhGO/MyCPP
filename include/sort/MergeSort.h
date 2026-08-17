//
// Created by Allen on 2018/10/24.
// Modify by Allen on 2022年9月13日14:06:58
//

#ifndef APUE_MERGESORT_H
#define APUE_MERGESORT_H

#include <vector>

void Merge(std::vector<int> &a, int start, int mid, int end) {
    int *tmp = static_cast<int *>(malloc((end - start) * sizeof(int))); // 分配一个临时空间用于排序
    int i = start;
    int j = mid;
    int k = 0;
    while (i < mid && j < end) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
        }
    }
    while (i < mid) {
        tmp[k++] = a[i++];
    }
    while (j < end) {
        tmp[k++] = a[j++];
    }
    for (int i = 0; i < k; ++i) {
        a[start + i] = tmp[i];
    }
    free(tmp);
}

void MergeSort(std::vector<int> &a, int start, int end) {
    if (end - start <= 1) { // 当还剩一个元素的时候就推出
        return;
    }
    int mid = (end  + start) / 2;
    MergeSort(a, start, mid);
    MergeSort(a, mid, end);
    Merge(a, start, mid, end);
}

#endif // APUE_MERGESORT_H
