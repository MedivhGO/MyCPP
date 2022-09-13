//
// Created by Allen on 2018/10/23.
//

#ifndef APUE_BINARYSEARCH_H
#define APUE_BINARYSEARCH_H

#include <vector>

using namespace std;

int BinarySearch(vector<int> &data, int elem) {
    int len = data.size();
    int i = 0;
    int j = len - 1;
    int mid;
    while (i <= j) {
        mid = i + (j - i) / 2;
        if (data[mid] > elem) {
            j = mid - 1;
        } else if (data[mid] == elem) {
            return mid;
        } else {
            i = mid + 1;
        }
    }
    return -1;
}

int BinarySearchR(vector<int> &data, int elem, int left, int right) {
    if (left > right) {
        return -1;
    }
    int i = left;
    int j = right;
    int mid = i + (j - i) / 2;
    if (data[mid] > elem) {
        j = mid - 1;
        return BinarySearchR(data, elem, i, j);
    } else if (data[mid] == elem) {
        return mid;
    } else {
        return BinarySearchR(data, elem, mid + 1, j);
    }
}

#endif // APUE_BINARYSEARCH_H
