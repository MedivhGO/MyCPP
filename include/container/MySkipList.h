//
// Created by Lee on 2022/9/6.
// Modified by Lee on 2023/9/24.
// Modified by Lee on 2024年6月20日11:58:03
//

#ifndef MYCPPIMPLEMENT_MYSKIPLIST_H
#define MYCPPIMPLEMENT_MYSKIPLIST_H

#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

struct Node {
  int key_;
  std::string value_;
  std::vector<std::shared_ptr<Node>> next_;

  Node(int key, std::string value, int max_level)
      : key_(key), value_(std::move(value)), next_(max_level, nullptr) {}
};

class Skiplist {
 private:
  std::shared_ptr<Node> tail_;
  std::shared_ptr<Node> head_;
  int max_key_{};
  const int max_level_;
  const float probability_;
  static constexpr auto DEFAULT_MAX_LEVEL = 16;
  static constexpr auto DEFAULT_PROBABILITY = 0.5F;
  static constexpr auto HEAD_VALUE = "head";
  static constexpr auto TAIL_VALUE = "NIL";
  std::mt19937 rng_{std::random_device{}()};
  std::uniform_real_distribution<float> random_dist_{0.0F, 1.0F};

 public:
  Skiplist();
  Skiplist(float probability, int max_level);

  auto FindNode(int key) const -> std::vector<std::shared_ptr<Node>>;
  auto RandomLevel() -> int;

  [[nodiscard]] auto Search(int target) const -> bool;
  auto Add(int key, const std::string& value) -> void;
  auto Erase(int num) -> bool;

  [[nodiscard]] auto GetMaxLevel() const -> int;
  auto GetMinKey() const -> int;
  auto GetMaxKey() const -> int;
  [[nodiscard]] auto IsEmpty() const -> bool;

  // 取跳表的头元素，并将头元素删除
  auto Peek() -> std::string;
  [[nodiscard]] auto Size() const -> int;

  auto ResetMaxKey() -> void;
  auto CheckIsNotEmpty() const -> void;
  [[nodiscard]] auto KeysOnSpecificLevel(int level) const -> int;

  ~Skiplist() = default;
};

#endif  // MYCPPIMPLEMENT_MYSKIPLIST_H
