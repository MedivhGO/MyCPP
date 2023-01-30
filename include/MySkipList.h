//
// Created by Lee on 2022/9/6.
//

#ifndef MYCPPIMPLEMENT_MYSKIPLIST_H
#define MYCPPIMPLEMENT_MYSKIPLIST_H

#define MAXLENGTH 32
#define N 999

#include <random>
#include <vector>

struct Node {
  int key_;
  std::vector<std::shared_ptr<Node>> next_;

  Node(int val, int max_level) : key_(val) {
    for (int i = 0; i < max_level; i++) {
      next_.emplace_back(nullptr);
    }
  };
};

class Skiplist {
 private:
  int max_height_{1};
  std::shared_ptr<Node> head_;

 public:
  Skiplist() {
    head_ = std::make_shared<Node>(0, MAXLENGTH);
    srand(0);
  }

  auto FindGENode(const int key, std::vector<std::shared_ptr<Node>> &prev) -> std::shared_ptr<Node> {  // for insert
    std::shared_ptr<Node> ptr = head_;
    int level = max_height_ - 1;
    while (true) {
      std::shared_ptr<Node> next = ptr->next_[level];
      if (next != nullptr && key > next->key_) {
        ptr = next;
      } else {
        prev[level] = ptr;
        if (level == 0) {
          return next;
        }
        level--;
      }
    }
  }

  auto FindGENode(const int key) -> std::shared_ptr<Node> {  // for search
    std::shared_ptr<Node> ptr = head_;
    int level = max_height_ - 1;
    while (true) {
      std::shared_ptr<Node> next = ptr->next_[level];
      if (next != nullptr && key > next->key_) {
        ptr = next;
      } else {
        if (level == 0) {
          return next;
        }           level--;

      }
    }
  }

  auto RandomLevel(float factor = 0.5) -> int {
    int level = 1;
    while ((rand() % N) / N > factor && level < MAXLENGTH) {
      level++;
    }
    return level;
  }

  auto Search(int target) -> bool {
    std::shared_ptr<Node> temp = FindGENode(target);
    return temp && temp->key_ == target;
  }

  void Add(int num) {
    int ran_level = RandomLevel();
    std::shared_ptr<Node> new_node = std::make_shared<Node>(num, ran_level);
    std::vector<std::shared_ptr<Node>> prev(MAXLENGTH, head_);
    std::shared_ptr<Node> temp = FindGENode(num, prev);
    if (ran_level > max_height_) {
      for (int i = max_height_ - 1; i < ran_level; i++) {
        prev[i] = head_;
      }
      max_height_ = ran_level;
    }
    for (int i = 0; i < max_height_; i++) {
      new_node->next_[i] = prev[i]->next_[i];
      prev[i]->next_[i] = new_node;
    }
  }

  auto Erase(int num) -> bool {
    std::vector<std::shared_ptr<Node>> prev(MAXLENGTH, head_);
    std::shared_ptr<Node> temp = FindGENode(num, prev);
    if (temp == nullptr || temp->key_ != num) {
      return false;
    }
    int level = temp->next_.size();
    for (int i = 0; i < level; i++) {
      prev[i]->next_[i] = temp->next_[i];
    }
    temp.reset();
    return true;
  }

  ~Skiplist() = default;
};

#endif  // MYCPPIMPLEMENT_MYSKIPLIST_H
