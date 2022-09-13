//
// Created by Allen on 2018/10/23.
//

#ifndef APUE_HEAPSORT_H
#define APUE_HEAPSORT_H

#include <vector>

using namespace std;

void Adjust(vector<int> &data, int root, int len) {
    int lchild = root * 2 + 1;
    if (lchild < len) {
        int flag = lchild;
        int rchild = lchild + 1;
        if (rchild < len) {
            if (data[rchild] > data[flag]) {
                flag = rchild;
            }
        }
        if (data[root] < data[flag]) {
            swap(data[root], data[flag]);
            Adjust(data, flag, len);
        }
    }
}

int getParent(int i) {
    if (i % 2 == 0) {
        return i / 2 - 1;
    } else {
        return i / 2;
    }
}

void AdjustDonwToUp(vector<int> &data) { // 插入一个元素，对堆进行调整
    int len = data.size();
    int child = len - 1;
    int par = getParent(child);
    while (par >= 0) {
        if (data[par] >= data[child]) {
            break;
        } else {
            swap(data[par], data[child]);
            child = par;
            par = getParent(child);
        }
    }
}

void HeapSort(vector<int> &data, int len) {
    // 从第一个非叶子节点开始到Root，向上调整
    for (int i = len / 2 - 1; i >= 0; --i) { // 找到第一个非叶子节点
        // 调整方法:就是构成相应的大顶堆
        Adjust(data, i, len);
    }

    for (int j = len - 1; j >= 1; --j) { // 调整好了，进行交换
        swap(data[0], data[j]);
        Adjust(data, 0, j);
        // 交换后，重新进行调整
    }
}

#endif // APUE_HEAPSORT_H
