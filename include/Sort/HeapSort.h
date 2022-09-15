//
// Created by Allen on 2018/10/23.
//

#ifndef APUE_HEAPSORT_H
#define APUE_HEAPSORT_H

#include <vector>

using namespace std;

/**
 * 堆排序其实是选择排序的优化变种，选择排序是把最大或最小的元素放到最边上，然后不断重复以上过程。
 * 堆排序也是如此，只不过堆排序通过构建数据结构，让查找最大或最小元素并放到最边上的速度比选择排序快得多。
 *
 * 堆指的是二叉堆，是一种完全二叉树，其父节点的值不小于其孩子节点的值时叫最大堆，父节点不大于其孩子节点的值时
 * 叫最小堆。
 *
 * 使用二叉堆，是因为可以直接用数组表示，如下图所示

 *                        ┌─────────┐
 *                        │         │
 *                        │  30     │
 *                        │         │
 *               ┌────────┴─────────┴────────────────┐
 *               │                                   │
 *               │                                   │
 *          ┌────▼────┐                        ┌─────▼────┐
 *          │         │                        │          │
 *          │   29    │                        │    28    │
 *          │         │                        │          │
 *     ┌────┴─────────┴───┐                ┌───┴──────────┴────────┐
 *     │                  │                │                       │
 *     │                  │                │                       │
 *     │                  │                │                       │
 *     ▼                  ▼                │                       ▼
 * ┌────────┐        ┌────────┐        ┌───▼─────┐          ┌───────────┐
 * │        │        │        │        │         │          │           │
 * │  24    │        │   25   │        │  26     │          │    22     │
 * │        │        │        │        │         │          │           │
 * └───┬────┘        └────────┘        └─────────┘          └───────────┘
 *     │
 *     │
 *     │
 * ┌───▼────┐
 * │        │
 * │        │
 * │  21    │
 * │        │
 * └────────┘

 * ┌────┬─────┬─────┬────┬──────┬─────┬────┬────┬─────┐
 * │    │     │     │    │      │     │    │    │     │
 * │idx │ 0   │  1  │ 2  │  3   │  4  │ 5  │ 6  │  7  │
 * │    │     │     │    │      │     │    │    │     │
 * ├────┼─────┼─────┼────┼──────┼─────┼────┼────┼─────┤
 * │    │     │     │    │      │     │    │    │     │
 * │val │ 30  │  29 │ 28 │  24  │  25 │ 26 │  22│  21 │
 * │    │     │     │    │      │     │    │    │     │
 * └────┴─────┴─────┴────┴──────┴─────┴────┴────┴─────┘
 */

/**
 *  由上图可知，下标为k的节点，其孩子节点的下标为 2*k+1, 2*k+2
 *  下标为k的节点，其父节点的下标为 (k-1)/2
 * */

void max_heapify(vector<int> &data, int root, int len) {
    int lchild = root * 2 + 1;
    int rchild = root * 2 + 2;
    if (lchild < len) { // 有左孩子
        int next_modify = lchild;
        if (rchild < len) { // 有右孩子
            if (data[rchild] > data[lchild]) {
                next_modify = rchild;
            }
        }
        if (data[root] < data[next_modify]) {
            swap(data[root], data[next_modify]);   // 交换这个子树中大小关系不符合堆定义的节点
            max_heapify(data, next_modify, len); // 对调整过的孩子重新建堆
        }
    }
}

int getParent(int i) {
    return (i - 1) / 2;
}

void shiftUp(vector<int> &data, int len) {
    if (len == 0) {
        return;
    }
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

// len 是最后一个元素的index+1
void HeapSort(vector<int> &data, int len) {
    if (len == 0) {
        return;
    }
    // 建堆shiftDown方法，从最后一个节点的父节点开始调整
    for (int i = (len - 1) / 2; i >= 0; --i) {
        max_heapify(data, i, len);
    }

    // 使用shiftUp方法建堆
    for (int i = 0; i < len; ++i) {
        shiftUp(data, i);
    }

    // 使用大顶堆，所以 data[0] 在建堆后存储了最大值
    for (int j = len - 1; j >= 1; --j) {      // 类似选择排序的流程
        swap(data[0], data[j]);        // 将最大值与有序集合的第一个元素交换
        max_heapify(data, 0, j); // 重新建堆
    }
}

// 时间复杂度 O(nlogn)
// 空间复杂度 O(1)
// 不稳定

// 由代码可知，当数组中的元素大部分相等时，max_heapify中的操作基本上没有执行
// 因此其时间复杂度接近 O(n)

#endif // APUE_HEAPSORT_H
