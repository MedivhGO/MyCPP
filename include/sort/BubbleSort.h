//
// Created by Allen on 2018/10/23.
//

#ifndef APUE_BUBBLESORT_H
#define APUE_BUBBLESORT_H

#include <vector>

using namespace std;

void BubbleSort(vector<int> &data, int len) {
    for (int i = 0; i < len - 1; ++i) { // 需要给 len-1 个位置确认 key 值
        int flag = false; // 减少比较次数
        for (int j = len - 1; j > i; --j) { // 从后往前依次比较，直到第 i+1 位置
            if (data[j] < data[j - 1]) { // 如果当前元素比它之前的那个小，那么就交换这两个元素
                swap(data[j], data[j - 1]);
                flag = true; // 记录进行过比较
            }
        }
        // 说明此前一趟没进行过交换，也就是说之前一趟的非有序集合中的元素，已经是有序了。可以退出排序过程了。
        if (flag == false) {
            return;
        }
    }
}

// 时间复杂度 O(n^2)
// 空间复杂度 O(1)
// 稳定

#endif // APUE_BUBBLESORT_H
