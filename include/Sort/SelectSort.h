//
// Created by Allen on 2018/10/23.
//

#ifndef APUE_SELECTSORT_H
#define APUE_SELECTSORT_H

#include <vector>

using namespace std;

void SelectSort(vector<int> &data) {
    int len = data.size();
    if (len == 0) {
        return;
    }
    for (int i = 0; i < len - 1; ++i) {
        int min = i;
        for (int j = i + 1; j < len; ++j) {
            if (data[j] < data[min]) {
                min = j;
            }
        }
        if (min != i) {
            swap(data[min], data[i]);
        }
    }
    }
#endif // APUE_SELECTSORT_H
