//
// Created by Allen on 2018/10/23.
//

#ifndef APUE_BUBBLESORT_H
#define APUE_BUBBLESORT_H

#include <vector>

using namespace std;

void BubbleSort(vector<int> &data, int len) {
    for (int i = 0; i < len - 1; ++i) {
        int flag = false;
        for (int j = len - 1; j > i; --j) {
            if (data[j] < data[j - 1]) {
                swap(data[j], data[j - 1]);
                flag = true;
            }
        }
        if (flag == false) {
            return;
        }
    }
}

#endif // APUE_BUBBLESORT_H
