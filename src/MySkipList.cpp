//
// Created by Lee on 2022/9/6.
//

#include "container/MySkipList.h"

#include <algorithm>
#include <limits>
#include <stdexcept>

Skiplist::Skiplist() : Skiplist(DEFAULT_PROBABILITY, DEFAULT_MAX_LEVEL) {}

Skiplist::Skiplist(float probability, int max_level)
    : probability_(probability), max_level_(max_level) {
  head_ = std::make_shared<Node>(std::numeric_limits<int>::min(), HEAD_VALUE, max_level);
  tail_ = std::make_shared<Node>(std::numeric_limits<int>::max(), TAIL_VALUE, max_level);
  std::fill(head_->next_.begin(), head_->next_.end(), tail_);
  ResetMaxKey();
}

auto Skiplist::FindNode(int key) const -> std::vector<std::shared_ptr<Node>> {
  std::shared_ptr<Node> ptr = head_;
  std::vector<std::shared_ptr<Node>> res(max_level_, nullptr);
  for (int i = max_level_ - 1; i >= 0; i--) {
    while (ptr->next_[i] && ptr->next_[i]->key_ < key) {
      ptr = ptr->next_[i];
    }
    res[i] = ptr;
  }
  return res;
}

auto Skiplist::RandomLevel() -> int {
  int level = 1;
  while (random_dist_(rng_) > probability_ && level < max_level_) {
    level++;
  }
  return level;
}

auto Skiplist::Search(int target) const -> bool {
  std::vector<std::shared_ptr<Node>> temp = FindNode(target);
  return temp[0]->next_[0] && temp[0]->next_[0]->key_ == target;
}

auto Skiplist::Add(int key, const std::string& value) -> void {
  if (key > max_key_) {
    max_key_ = key;
  }
  std::vector<std::shared_ptr<Node>> temp = FindNode(key);
  auto existing = temp[0]->next_[0];
  if (existing && existing->key_ == key) {  // exist
    existing->value_ = value;
    return;
  }
  // not exist
  auto new_node = std::make_shared<Node>(key, value, max_level_);
  auto level = RandomLevel();
  for (int i = 0; i < level; ++i) {
    auto old_next = temp[i]->next_[i];
    temp[i]->next_[i] = new_node;
    new_node->next_[i] = old_next;
  }
}

auto Skiplist::Erase(int num) -> bool {
  std::vector<std::shared_ptr<Node>> temp = FindNode(num);
  if (!temp[0]->next_[0] || temp[0]->next_[0]->key_ != num) {
    return false;
  }
  auto node = temp[0]->next_[0];
  // change every level ptr
  for (int i = 0; i < max_level_ && temp[i]->next_[i] == node; ++i) {
    temp[i]->next_[i] = node->next_[i];
  }
  return true;
}

auto Skiplist::GetMaxLevel() const -> int {
  return max_level_;
}

auto Skiplist::GetMinKey() const -> int {
  CheckIsNotEmpty();
  return head_->next_[0]->key_;
}

auto Skiplist::GetMaxKey() const -> int {
  CheckIsNotEmpty();
  return max_key_;
}

auto Skiplist::IsEmpty() const -> bool {
  return head_->next_[0] == tail_;
}

auto Skiplist::Peek() -> std::string {
  CheckIsNotEmpty();
  auto cur = head_->next_[0];
  std::string value = cur->value_;
  for (size_t i = 0; i < cur->next_.size(); ++i) {
    head_->next_[i] = cur->next_[i];
  }
  if (IsEmpty()) {
    ResetMaxKey();
  }
  return value;
}

auto Skiplist::Size() const -> int {
  auto cur = head_;
  int counter = 0;
  while (cur->next_[0] != tail_) {
    counter++;
    cur = cur->next_[0];
  }
  return counter;
}

auto Skiplist::ResetMaxKey() -> void {
  max_key_ = std::numeric_limits<int>::min();
}

auto Skiplist::CheckIsNotEmpty() const -> void {
  if (IsEmpty()) {
    throw std::runtime_error("No elements in skip list");
  }
}

auto Skiplist::KeysOnSpecificLevel(int level) const -> int {
  if (level >= 0 && level < max_level_) {
    int counter = 0;
    auto new_cur = head_->next_[level];
    while (new_cur != tail_) {
      counter++;
      new_cur = new_cur->next_[level];
    }
    return counter;
  }
  return 0;
}
