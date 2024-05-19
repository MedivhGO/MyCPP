//
// Created by lee on 23-11-21.
//

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <random>
#include <limits>
#include <stdexcept>
#include <string>
#include <sys/resource.h>
#include <thread>
#include <vector>
#include <chrono>
#include <condition_variable>

namespace fs = std::filesystem;

constexpr int CHUNK_SIZE = 20 * 1024 * 1024;
constexpr int MEMORY_LIMIT = 30 * 1024 * 1024;
constexpr int FILE_SIZE = 500 * 1024 * 1024;

static int partition(std::vector<int> &data, int start, int end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(start, end);

  int pivotIndex = dis(gen);
  std::swap(data[pivotIndex], data[end]);
  int pivot = data[end];

  int i = start - 1, j = start;
  while (j < end) {
    if (data[j] > pivot) {
      i++;
      std::swap(data[i], data[j]);
    }
    j++;
  }

  std::swap(data[i + 1], data[end]);
  return i + 1;
}

static int helper(std::vector<int> &data, int k, int start, int end) {
  if (start == end) {
    return start;
  }

  int mid = partition(data, start, end);
  int index = mid - start + 1;

  if (index == k) {
    return mid;
  } else if (index > k) {
    return helper(data, k, start, mid - 1);
  } else {
    return helper(data, k - index, mid + 1, end);
  }
}

std::vector<int> topKUsingPartition(std::vector<int> &data, int k, int end) {
  if (k >= end) {
    return std::vector<int>{data.cbegin(), data.cbegin() + end + 1};
  }

  int index = helper(data, k, 0, end - 1);
  return std::vector<int>{data.cbegin(), data.cbegin() + index + 1};
}

std::vector<int> topKTwoArray(std::vector<int> array1, std::vector<int> array2, int k) {
  std::vector<int> array{};
  array.reserve(array1.size() + array2.size());

  for (auto num : array1) {
    array.push_back(num);
  }

  for (auto num : array2) {
    array.push_back(num);
  }

  return topKUsingPartition(array, k, array.size());
}

fs::path CreateTempfile() {
  fs::path temp_dir = fs::temp_directory_path();
  fs::path temp_file = temp_dir / "temp_file.bin";

  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<int> dis(1, 1000);

  std::ofstream file{temp_file, std::ios::binary | std::ios::out | std::ios::trunc};

  if (!file.is_open()) {
    throw std::runtime_error("cannot read temp file");
  }

  std::vector<int> data(FILE_SIZE / sizeof(int));
  for (int & i : data) {
    i = dis(gen);
  }

  file.write(reinterpret_cast<char *>(data.data()), data.size() * sizeof(int));
  file.close();

  return temp_file;
}

std::vector<int> topKUsingPartitionParallelWithRestrictedMemory(fs::path &p, int k) {
  if (!fs::exists(p)) {
    std::string info = "Error: " + p.string() + " does not exist";
    throw std::runtime_error(info);
  }

  std::ifstream file{p, std::ios_base::in};
  if (!file.is_open()) {
    std::string info = "Error: cannot open " + p.string();
    throw std::runtime_error(info);
  }

  std::vector<int> buffer(CHUNK_SIZE / sizeof(int));
  std::vector<std::vector<int>> nums{};

  while (file.read(reinterpret_cast<char *>(buffer.data()), CHUNK_SIZE)) {
    std::streamsize bytesRead = file.gcount();
    if (bytesRead > 0) {
      int numElements = bytesRead / sizeof(int);
      auto partialK = topKUsingPartition(buffer, k, numElements);
      nums.push_back(std::move(partialK));
    }
  }

  file.close();

  int numTask = 0, length = nums.size();
  while (length != 1) {
    numTask += length / 2;
    length = length / 2 + length % 2;
  }

  int allocatedTask = 0, finishedTask = 0;

  std::mutex lock{};
  std::condition_variable cond{};

  auto threadFunc = [numTask = numTask, k = k](int &allocatedTask,
                                               int &finishedTask,
                                               std::mutex &lock,
                                               std::condition_variable &cond,
                                               std::vector<std::vector<int>> &nums) {
    while (true) {
      int index1 = -1, index2 = -1;

      {
        std::unique_lock<std::mutex> guard{lock};
        while (allocatedTask + 2 > nums.size()) {
          cond.wait(guard);
          if (finishedTask == numTask) {
            return;
          }
        }
        index1 = allocatedTask;
        index2 = allocatedTask + 1;
        allocatedTask += 2;
      }

      auto result = topKTwoArray(nums[index1], nums[index2], k);

      {
        std::unique_lock<std::mutex> guard{lock};
        nums.push_back(std::move(result));
        finishedTask++;

        if (finishedTask == numTask) {
          cond.notify_all();
          return;
        } else if (allocatedTask + 2 <= nums.size()) {
          cond.notify_all();
        }
      }
    }
  };

  const int numThreads = 8;
  std::vector<std::thread> threads{};

  for (int i = 0; i < numThreads; i++) {
    threads.emplace_back(std::thread{
        threadFunc, std::ref(allocatedTask), std::ref(finishedTask), std::ref(lock), std::ref(cond), std::ref(nums)});
  }

  for (int i = 0; i < numThreads; i++) {
    threads[i].join();
  }

  return nums.back();
}

TEST(MyFile, DISABLED_Multi_Thread_File) {
  auto tempFile = CreateTempfile();
  struct rlimit limit;
  limit.rlim_cur = MEMORY_LIMIT;
  limit.rlim_max = MEMORY_LIMIT;

  if (setrlimit(RLIMIT_AS, &limit) != 0) {
    throw std::runtime_error("cannot set memory limit");
  }
  std::cout << "\n=== Running using select parallelism\n";
  auto start = std::chrono::high_resolution_clock::now();
  auto ansSelect = topKUsingPartitionParallelWithRestrictedMemory(tempFile, 10);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  double seconds = elapsed.count();
  std::cout << "Elapsed: " << seconds << " s" << std::endl;
  fs::remove(tempFile);
}