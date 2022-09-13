//
// Created by Allen on 2018/10/23.
//

#ifndef APUE_QUICKSORT_H
#define APUE_QUICKSORT_H

#include <vector>

using namespace std;

int partition(vector<int> &data, int left, int right) {
    int i = left;
    int j = right - 1;

    int pivot = data[i];
    while (i < j) {
        while (i < j && data[j] >= pivot) {
            j--;
        }
        data[i] = data[j];
        while (i < j && data[i] <= pivot) {
            i++;
        }
        data[j] = data[i];
    }
    data[i] = pivot;
    return i;
}

void QuickSort(vector<int> &data, int left, int right) {
    if (left < right) { // left < right 说明待排序的区间中还有元素
        int i = partition(data, left, right);
        QuickSort(data, left, i);
        QuickSort(data, i + 1, right);
    }
}
#endif // APUE_QUICKSORT_H
