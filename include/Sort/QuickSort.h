//
// Created by Allen on 2018/10/23.
//

#ifndef APUE_QUICKSORT_H
#define APUE_QUICKSORT_H

#include <vector>

#include "BubbleSort.h"

// Hoare partition scheme
// 为第一个元素找合适的位置
int Partition(std::vector<int> &data, int left, int right) {
  int pivot = data[left];
  while (left < right) {
    while (left < right && data[right] >= pivot) {
      --right;
    }
    data[left] = data[right];
    while (left < right && data[left] < pivot) {
      ++left;
    }
    data[right] = data[left];
  }
  data[left] = pivot;
  return left;
}

void QuickSort(std::vector<int> &data, int left, int right) {
  if (left >= right) {
    return;
  }
  int i = Partition(data, left, right - 1);
  QuickSort(data, left, i);
  QuickSort(data, i + 1, right);
}

// 在无序的情况下，每次的划分点都能把数组平均分成两份
// 由于每次调用 QuickSort 函数的时间复杂度为 O(n)
// 共有 lgn 次的调用
// 所以时间复杂度为 O(nlgn)

// partition 函数的时间复杂度为 O(n)
// QuickSort 函数由于 i 在基本有序的情况下，由于 i 每次都会以 step=1 的步长递增，会执行 n 次
// 所以时间复杂度为 O(n^2)

// 快速排序的稳定性 不稳定
/*
[1，4，2(1)，3，6，2(2)，5，2(3)]
我们选择第二个 2 也就是 2(2) 为基准
一次partition后
[1，2(3), 2(1), 2(2), 6, 3, 5, 4]
可见是不稳定的
结论：
如果一个数字在待排序的列表中出现三次或以上，而这个数字在列表中出现的（非首次和末次）一次被选为基准（pivot），则结果肯定是不稳定的。
因此，通过更改比较时 >= 符号为 > 符号是没有意义的，因为不稳定的根源在于基准的选取。
如果要得到稳定的快速排序，必须在选取基准时避免这种情况，而且在交换元素时要小心不要打乱相同元素的相对顺序。
一种可选的做法是先记录相同元素的相对位置，再进行排序。这需要额外的空间开销。
 */

// median of three element 划分
// 我们可以在数组中选取三个候选元素（通常为数组的首、尾、中点元素），并将这三个候选元素的中位数作为基准数

int medianOfThree(std::vector<int> &data, int left, int mid, int right) {
  int l = nums[left], m = nums[mid], r = nums[right];
  if ((l <= m && m <= r) || (r <= m && m <= l)) {
    return mid;
  }
  if ((m <= l && l <= r) || (r <= l && l <= m)) {
    return left;
  }
  return right;
}

int partition(std::vector<int> &data, int left, int right) {
  int mid = medianOfThree(nums, left, (left + right) / 2, right);
  int pivot = nums[mid];
  swap(nums[left], nums[mid]);
  while (left < right) {
    while (left < right && nums[right] >= pivot) {
      right--;
    }
    nums[left] = nums[right];
    while (left < right && nums[left] < pivot) {
      left++;
    }
    nums[right] = nums[left];
  }
  nums[left] = pivot;
  return left;
}

void QuickSortMOT(std::vector<int> &data, int left, int right) {
  if (right - left > 1) {
    int part = partition(data, left, right - 1);
    QuickSortMOT(data, left, part);
    QuickSortMOT(data, part + 1, right);
  }
}

#endif  // APUE_QUICKSORT_H
