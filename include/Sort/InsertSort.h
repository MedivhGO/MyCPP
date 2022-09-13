//
// Created by Allen on 2018/10/23.
// Modify by Allen on 2022年9月13日14:06:58
//

#ifndef APUE_INSERTSORT_H
#define APUE_INSERTSORT_H

#include <vector>

using namespace std;

void InsertSort(vector<int> &data) {
    int len = data.size();
    if (len == 0) {
        return;
    }
    int tmp;
    for (int i = 1; i < len; ++i) {
        if (data[i] < data[i - 1]) {
            tmp = data[i];
        }
        int j;
        for (j = i - 1; data[j] > tmp; --j) {
            data[j + 1] = data[j];
        }
        data[j] = tmp;
    }
}

#endif // APUE_INSERTSORT_H
