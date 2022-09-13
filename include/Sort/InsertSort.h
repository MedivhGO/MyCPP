//
// Created by Allen on 2018/10/23.
// Modify by Allen on 2022年9月13日14:06:58
//

#ifndef APUE_INSERTSORT_H
#define APUE_INSERTSORT_H

#include <vector>

using namespace std;

void InsertSort(vector<int> &data, int len) {
    if (len == 0) {
        return;
    }
    int tmp;
    for (int i = 1; i < len; ++i) {
        if (data[i] < data[i - 1]) {
            tmp = data[i]; // 在有序集合中寻找合适的位置
            int prop_pos = i - 1;
            while(prop_pos >= 0 && data[prop_pos] > tmp) { // 找第一个比tmp小的数，
                data[prop_pos+1] = data[prop_pos];
                prop_pos--;
            }
            data[prop_pos+1] = tmp;
        }
    }
}

#endif // APUE_INSERTSORT_H
