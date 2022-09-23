//
// Created by Lee on 2022/9/22.
//

#ifndef MYCPPIMPLEMENT_QUICKSORTMEDIANOFTHREE_H
#define MYCPPIMPLEMENT_QUICKSORTMEDIANOFTHREE_H

#include <vector>
#include <string>
#include <iostream>

using std::string;
using std::vector;
using std::swap;
using std::to_string;

class QS {
  public:
    QS() = default;
    QS(const vector<int>&);
    virtual ~QS() = default;
    void sortAll();
    int medianOfThree(int left, int right);
    int partition(int left, int right, int pivotIndex);
    string getArray() const;
    vector<int> getVec() const;
    int getSize() const;
    bool addToArray(int value);
    bool createArray(int capacity);
    void clear();
  private:
    void quickSort(int left, int right);
    void bubbleSort(vector<int>& data, int len);
    void printArray();
  private:
    std::vector<int> m_array;
    int m_capacity;
};

QS::QS(const vector<int>& x) {
    m_array = x;
}

void QS::sortAll() {
    quickSort(0, m_array.size());
}

int QS::medianOfThree(int left, int right) {
    if (right <= left || m_array.empty() || right >= m_array.size() || left < 0) {
        return -1;
    }
    // 只有两个数的时候该怎么排序？
    int middle = (left + right) / 2;
    if (left == middle && m_array[left] > m_array[right]) {
        swap(m_array[left], m_array[right]);
        return middle;
    }
    vector<int> tmp;
    tmp.push_back(m_array[left]);
    tmp.push_back(m_array[right]);
    tmp.push_back(m_array[middle]);
    bubbleSort(tmp, 3);
    m_array[left] = tmp[0];
    m_array[middle] = tmp[1];
    m_array[right] = tmp[2];
    return middle;
}

int QS::partition(int left, int right, int pivotIndex) {
    if (m_array.empty()
        || left < 0
        || right <= left
        || right >= m_array.size()
        || pivotIndex < left
        || pivotIndex > right) {
        return -1;
    }
    swap(m_array[left], m_array[pivotIndex]);
    int pivot = m_array[left];
    int up = left + 1;
    int down = right;
    while (up < down) {
        while (up <= right && m_array[up] <= pivot) {
            up++;
        }
        while (down >= left && m_array[down] > pivot) {
            down--;
        }
        if (up < down) {
            swap(m_array[up], m_array[down]);
        }
    }
    if (down < 0) {
        return -1;
    }
    if (right - left > 1) { // 当只有 2 个元素时不需要交换
        swap(m_array[left], m_array[down]);
    }
    return down;
}

string QS::getArray() const {
    string ret = "";
    string sep = ",";
    for (int i = 0; i < m_array.size(); ++i) {
        if (i == m_array.size() - 1) {
            sep = "";
        }
        ret += (to_string(m_array[i]) + sep);
    }
    return ret;
}

int QS::getSize() const {
    return m_array.size();
}

bool QS::addToArray(int value) {
    if (m_array.size() == m_capacity) {
        return false;
    }
    m_array.push_back(value);
    return true;
}

bool QS::createArray(int capacity) {
    if (capacity <= 0) {
        return false;
    }
    m_array.resize(0);
    m_capacity = capacity;
    return true;
}

void QS::clear() {
    m_array.resize(0);
    m_capacity = 0;
}

vector<int> QS::getVec() const {
    return m_array;
}

void QS::printArray() {
    std::string array = getArray();
    std::cout << array << std::endl;
}

void QS::quickSort(int left, int right) {
    if (right - left > 1) {
        int pivot_index = medianOfThree(left, right - 1);
        int part = partition(left, right - 1, pivot_index);
        quickSort(left, part);
        quickSort(part+1, right);
    }
}

void QS::bubbleSort(vector<int>& data, int len) {
    for (int i = 0; i < len - 1; ++i) {
        int skip = false;
        for (int j = len - 1; j > i; --j) {
            if (data[j] < data[j-1]) {
                swap(data[j], data[j - 1]);
                skip = true;
            }
        }
        if (skip == false) {
            return;
        }
    }
}



#endif // MYCPPIMPLEMENT_QUICKSORTMEDIANOFTHREE_H
