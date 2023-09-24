//
// Created by Lee on 2022/9/6.
// Modified by Lee on 2023/9/24.
//

#ifndef MYCPPIMPLEMENT_MYSKIPLIST_H
#define MYCPPIMPLEMENT_MYSKIPLIST_H

#include <random>
#include <vector>
#include <memory>

struct Node {
  int key_;
  std::string value_;
  std::vector<std::shared_ptr<Node>> next_;

  Node(int key, const std::string& value, int max_level)
      : key_(key), value_(value), next_(max_level, nullptr) {};
};

class Skiplist {
 private:
  std::shared_ptr<Node> tail_;
  std::shared_ptr<Node> head_;
  int max_key_;
  const uint64_t max_level_;
  const float probability_;

 private:
  static constexpr auto DEFAULT_MAX_LEVEL = 16;
  static constexpr auto DEFAULT_PROBABILITY = 0.5F;
  static constexpr auto HEAD_VALUE = "head";
  static constexpr auto TAIL_VALUE = "NIL";

 public:
  Skiplist() : Skiplist(DEFAULT_PROBABILITY, DEFAULT_MAX_LEVEL) {}

  Skiplist(const float probability, const unsigned int max_level) : probability_(probability), max_level_(max_level) {
    head_ = std::make_shared<Node>(std::numeric_limits<int>::min(), HEAD_VALUE, max_level);
    tail_ = std::make_shared<Node>(std::numeric_limits<int>::max(), TAIL_VALUE, max_level);
    std::fill(head_->next_.begin(), head_->next_.end(), tail_);
    ResetMaxKey();
  }

  auto FindNode(const int key) -> std::vector<std::shared_ptr<Node>> {
    std::shared_ptr<Node> ptr = head_;
    std::vector<std::shared_ptr<Node>> res(max_level_, nullptr);
    for(int i = max_level_ - 1; i >= 0; i--) {
      while (ptr->next_[i] && ptr->next_[i]->key_ < key) {
        ptr = ptr->next_[i];
      }
      res[i] = ptr;
    }
    return res;
  }

  auto RandomLevel() -> int {
    int level = 1;
    while ((static_cast<float>(rand())  / RAND_MAX) > probability_ && level < max_level_) {
      level++;
    }
    return level;
  }

  auto Search(int target) -> bool {
    std::vector<std::shared_ptr<Node>> temp = FindNode(target);
    return temp[0]->next_[0] && temp[0]->next_[0]->key_ == target;
  }

  auto Add(int key, const std::string& value) -> void {
    if (key > max_key_) {
      max_key_ = key;
    }
    std::vector<std::shared_ptr<Node>> temp = FindNode(key);
    auto p = temp[0]->next_[0];
    if (p && p->key_ == key) { // exist
      p->value_ = value;
      return;
    }
    // not exist
    auto new_node = std::make_shared<Node>(key, value, max_level_);
    auto lvl = RandomLevel();
    for (int i = 0; i < lvl; ++i) {
      auto nt = temp[i]->next_[i];
      temp[i]->next_[i] = new_node;
      new_node->next_[i] = nt;
    }
  }

  auto Erase(int num) -> bool {
    std::vector<std::shared_ptr<Node>> temp = FindNode(num);
    if (!temp[0]->next_[0] || temp[0]->next_[0]->key_ != num) {
      return false;
    }
    auto p = temp[0]->next_[0];
    // change every level ptr
    for (int i = 0; i < max_level_ && temp[i]->next_[i] == p; ++i) {
      temp[i]->next_[i] = p->next_[i];
    }
    return true;
  }

  auto GetMaxLevel() -> int {
    return max_level_;
  }

  auto GetMinKey() const -> int {
    CheckIsNotEmpty();
    return head_->next_[0]->key_;
  }

  auto GetMaxKey() const -> int {
    CheckIsNotEmpty();
    return max_key_;
  }

  auto IsEmpty() const -> bool {
    return head_->next_[0] == tail_;
  }

  auto Peek() -> std::string {
    CheckIsNotEmpty();
    auto cur = head_->next_[0];
    std::string value = cur->value_;
    int key = cur->key_;
    for (int i = 0; i < cur->next_.size(); ++i) {
      head_->next_[i] = cur->next_[i];
    }
    if (IsEmpty()) {
      ResetMaxKey();
    }
    return value;
  }

  auto Size() -> int {
    auto cur = head_;
    int counter = 0;
    while(cur->next_[0] != tail_) {
      counter++;
      cur = cur->next_[0];
    }
    return counter;
  }

  auto ResetMaxKey() -> void {
    max_key_ = std::numeric_limits<int>::min();
  }

  auto CheckIsNotEmpty() const -> void {
    if(IsEmpty()) {
      throw std::runtime_error("No elements in skip list");
    }
  }

  auto KeysOnSpecificLevel(unsigned int level) const {
    if (level < max_level_) {
      auto counter = 0;
      auto new_cur = head_->next_[level];
      while (new_cur != tail_) {
        counter++;
        new_cur = new_cur->next_[level];
      }
      return counter;
    }
    return 0;
  }

  ~Skiplist() = default;
};

#endif  // MYCPPIMPLEMENT_MYSKIPLIST_H
