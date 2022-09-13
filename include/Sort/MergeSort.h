//
// Created by Allen on 2018/10/24.
// Modify by Allen on 2022年9月13日14:06:58
//

#ifndef APUE_MERGESORT_H
#define APUE_MERGESORT_H

#include <vector>

using namespace std;

void merge(vector<int> &a, int start, int mid, int end) {
    int *tmp = (int *)malloc((end - start + 1) * sizeof(int));
    int i = start;
    int j = mid + 1;
    int k = 0;
    while (i <= mid && j <= end) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
        }
    }
    while (i <= mid) {
        tmp[k++] = a[i++];
    }
    while (j <= end) {
        tmp[k++] = a[j++];
    }
    for (int i = 0; i < k; ++i) {
        a[start + i] = tmp[i];
    }
    free(tmp);
}

void MergeSort(vector<int> &a, int start, int end) {
    if (a.empty() || start >= end) {
        return;
    }
    int mid = (end + start) / 2;
    MergeSort(a, start, mid);
    MergeSort(a, mid + 1, end);
    merge(a, start, mid, end);
}

#endif // APUE_MERGESORT_H
