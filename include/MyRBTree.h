//
// Interface of Red-Black Tree
//
// author: Haris Wang, dynmiw@gmail.com
//
#ifndef RBTREE_H_
#define RBTREE_H_

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

enum color { BLACK, RED };

template <class K, class T>
struct RbNode {
  K key_;
  T val_;
  color color_{RED};
  RbNode *parent_;
  RbNode *left_;
  RbNode *right_;

  RbNode() : parent_(nullptr), left_(nullptr), right_(nullptr) {}
  ~RbNode() {
    delete left_;
    delete right_;
  }
};

template <typename K, typename T>
class RbTree {
 public:
  RbTree() : root_(nullptr){};
  ~RbTree();
  void Insert(K &key, T &val);
  auto Remove(const K &key) -> bool;
  auto Search(const K &key, T &val) const -> bool;
  void Clear();
  void PrintTree() const;
  auto GetSize() const -> int;

 private:
  int size_{0};
  RbNode<K, T> *root_;

  auto Cmp(const K &a, const K &b) const -> int;
  void LeftRotate(RbNode<K, T> *node);
  void RightRotate(RbNode<K, T> *node);
  void RemoveNode(RbNode<K, T> *node);
};

// Destructor of 'rbTree'
template <class K, class T>
RbTree<K, T>::~RbTree() {
  delete root_;
}

// insert (key, val) pair to appropriate position, then
// update the tree to keep its properties if needed.
// this->size will increase by 1.
template <class K, class T>
void RbTree<K, T>::Insert(K &key, T &val) {
  auto *node = new RbNode<K, T>;
  node->key_ = key;
  node->val_ = val;

  // whether first node
  if (root_ == nullptr) {
    root_ = node;
    node->color_ = BLACK;
    return;
  }

  // navigate to the desired positon of new node
  RbNode<K, T> *curr = root_;
  while (curr->left_ != nullptr | curr->right_ != nullptr) {
    if (Cmp(key, curr->key_) == -1) {
      curr = curr->left_;
    } else {
      curr = curr->right_;
    }
  }

  // link new node and its parent_
  node->parent_ = curr;
  if (Cmp(key, curr->key_) == -1) {
    curr->left_ = node;
  } else {
    curr->right_ = node;
  }

  // update the tree recursively to keep its properties if needed
  while (curr->color_ == RED && curr->parent_ != nullptr) {
    bool is_right = (curr == curr->parent_->right_);
    RbNode<K, T> *uncle;
    if (is_right) {
      uncle = curr->parent_->left_;
    } else {
      uncle = curr->parent_->right_;
    }

    if (uncle == nullptr) {
      curr->color_ = BLACK;
      curr->parent_->color_ = RED;
      if (uncle == curr->parent_->right_) {
        RightRotate(curr->parent_);
      } else {
        LeftRotate(curr->parent_);
      }
      break;
    }

    if (uncle->color_ == RED) {
      curr->color_ = BLACK;
      uncle->color_ = BLACK;
      curr->parent_->color_ = RED;
      curr = curr->parent_;
    } else {
      curr->color_ = BLACK;
      curr->parent_->color_ = RED;

      if (is_right) {
        if (node == curr->left_) {
          RightRotate(curr);
          curr = node;
        }
        LeftRotate(curr->parent_);
      } else {
        if (node == curr->right_) {
          LeftRotate(curr);
          curr = node;
        }
        RightRotate(curr->parent_);
      }
    }
    root_->color_ = BLACK;
  }

  this->size_++;
}

// return 1 if success
// return 0 if not found
// remove the node with 'key' from tree, then
// update the tree to keep its properties if needed.
// this->size will decrease by 1
template <class K, class T>
auto RbTree<K, T>::Remove(const K &key) -> bool {
  // find the node to be deleted
  auto curr = root_;
  while (curr->left_ != nullptr | curr->right_ != nullptr) {
    if (curr->key_ == key) {
      break;
    }

    if (Cmp(key, curr->key_) >= 0) {
      curr = curr->right_;
    } else {
      curr = curr->left_;
    }
  }
  if (curr->key_ != key) {
    return false;
  }

  this->RemoveNode(curr);
  (this->size_)--;
  return true;
}

// remove given node from this tree. 'node' must
// be in this tree
template <class K, class T>
void RbTree<K, T>::RemoveNode(RbNode<K, T> *node) {
  if (node->color_ == RED) {
    if (node->left_ != nullptr && node->right_ != nullptr) {
      // child x 2
      // find successor, then fill 'node' with successor
      auto successor = node->right_;
      while (successor->left_ != nullptr) {
        successor = successor->left_;
      }
      node->key_ = successor->key_;
      node->val_ = successor->val_;
      this->RemoveNode(successor);
    } else if (node->left_ != nullptr) {
      // only left_ child
      // fill 'node' with left_ child
      node->key_ = node->left_->key_;
      node->val_ = node->left_->val_;
      node->color_ = node->left_->color_;
      this->RemoveNode(node->left_);
    } else if (node->right_ != nullptr) {
      // only right_ child
      // fill 'node' with right_ child
      node->key_ = node->right_->key_;
      node->val_ = node->right_->val_;
      node->color_ = node->right_->color_;
      this->RemoveNode(node->right_);
    } else {
      // no child
      if (node->parent_ == nullptr) {
        delete(node);
        root_ = nullptr;
        return;
      }

      if (node->parent_->left_ == node) {
        node->parent_->left_ = nullptr;
      } else {
        node->parent_->right_ = nullptr;
      }

      delete(node);
    }
  } else {
    if (node->left_ != nullptr && node->right_ != nullptr) {
      // child x 2
      // find successor, then fill 'node' with successor
      auto successor = node->right_;
      while (successor->left_ != nullptr) {
        successor = successor->left_;
      }
      node->key_ = successor->key_;
      node->val_ = successor->val_;
      this->RemoveNode(successor);
    } else if (node->left_ != nullptr) {
      // only left_ child
      // fill 'node' with left_ child
      node->key_ = node->left_->key_;
      node->val_ = node->left_->val_;
      this->RemoveNode(node->left_);
    } else if (node->right_ != nullptr) {
      // only right_ child
      // fill 'node' with right_ child
      node->key_ = node->right_->key_;
      node->val_ = node->right_->val_;
      this->RemoveNode(node->right_);
    } else {
      // no child. As the black node deleted is a leaf, fixup
      // is neccesary after delete.

      if (node->parent_ == nullptr) {
        // 'node' is root
        delete(node);
        root_ = nullptr;
        return;
      }

      if (node->parent_->left_ == node) {
        node->parent_->left_ = nullptr;
        if (node->parent_->right_ != nullptr && node->parent_->right_->color_ == RED) {
          node->parent_->right_->color_ = BLACK;
          LeftRotate(node->parent_);
        }
      } else {
        node->parent_->right_ = nullptr;
        if (node->parent_->left_ != nullptr && node->parent_->left_->color_ == RED) {
          node->parent_->left_->color_ = BLACK;
          RightRotate(node->parent_);
        }
      }

      if (node->parent_->left_ == nullptr && node->parent_->right_ == nullptr && node->parent_->parent_ != nullptr) {
        // you can guess, 'node->parent_->parent_' must be RED
        RightRotate(node->parent_->parent_);
      }

      delete(node);
    }
  }
}

// return 1 if success
// return 0 if not found
// find the node with 'key' in this tree, then set
// 'val' to the val of this node
template <class K, class T>
auto RbTree<K, T>::Search(const K &key, T &val) const -> bool {
  auto curr = root_;
  while (curr->left_ != nullptr | curr->right_ != nullptr) {
    if (curr->key_ == key) {
      val = curr->val;
      break;
    }

    if (Cmp(key, curr->key_) < 0) {
      curr = curr->left_;
    } else {
      curr = curr->right_;
    }
  }

  return static_cast<bool>(curr->key_ == key);
}

// compare 'a' and 'b'
// return -1 if a < b
// return 0 if a = b
// return 1 if a > b
template <class K, class T>
auto RbTree<K, T>::Cmp(const K &a, const K &b) const -> int {
  if (typeid(a) == typeid(char *)) {
    // string type
    char *x = (char*)(a); // NOLINT
    char *y = (char*)(b); // NOLINT
    return strcmp(x, y);
  }
  if (a < b) {
    return -1;
  }
  if (a == b) {
    return 0;
  }
  if (a > b) {
    return 1;
  }
  return 0;
}

// take 'node' as the center, make a left_ rotation
template <class K, class T>
void RbTree<K, T>::LeftRotate(RbNode<K, T> *node) {
  assert(node->right_ != nullptr);
  auto temp = node->right_;

  // update the two nodes
  node->right_ = temp->left_;
  if (temp->left_ != nullptr) {
    temp->left_->parent_ = node;
  }
  temp->left_ = node;
  temp->parent_ = node->parent_;
  node->parent_ = temp;

  // update the parent_
  if (root_ == node) {
    root_ = temp;
    return;
  }
  if (temp->parent_->left_ == node) {
    temp->parent_->left_ = temp;
  } else {
    temp->parent_->right_ = temp;
  }
}

// take 'node' as the center, make a right_ rotation
template <class K, class T>
void RbTree<K, T>::RightRotate(RbNode<K, T> *node) {
  assert(node->left_ != nullptr);
  auto temp = node->left_;

  // update the two nodes
  node->left_ = temp->right_;
  if (temp->right_ != nullptr) {
    temp->right_->parent_ = node;
  }
  temp->right_ = node;
  temp->parent_ = node->parent_;
  node->parent_ = temp;

  // update the parent_
  if (root_ == node) {
    root_ = temp;
    return;
  }
  if (temp->parent_->left_ == node) {
    temp->parent_->left_ = temp;
  } else {
    temp->parent_->right_ = temp;
  }
}

// From the root to the leaf, send 'node',
// 'node's left_ sub-tree,'node's right_ sub-tree to stdout
template <class K, class T>
void RbTree<K, T>::PrintTree() const {
  std::cout << "----------------" << std::endl;
  std::queue<RbNode<K, T> *> q;
  q.push(root_);
  while (!q.empty()) {
    auto top = q.front();
    q.pop();
    if (top->color_ == RED) {
      std::cout << "R";
    } else {
      std::cout << "B";
    }
    std::cout << top->key_;
    std::cout << " ";
    if (top->left_ != nullptr) {
      q.push(top->left_);
      if (top->left_->color_ == RED) {
        std::cout << "R";
      } else {
        std::cout << "B";
      }
      std::cout << top->left_->key_;
      std::cout << " ";
    } else {
      std::cout << "NULL"
                << " ";
    }
    if (top->right_ != nullptr) {
      q.push(top->right_);
      if (top->right_->color_ == RED) {
        std::cout << "R";
      } else {
        std::cout << "B";
      }
      std::cout << top->right_->key_;
      std::cout << " ";
    } else {
      std::cout << "NULL"
                << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << "----------------" << std::endl;
}

// return the number of nodes in this tree
template <class K, class T>
auto RbTree<K, T>::GetSize() const -> int {
  return this->size_;
}

// delete all nodes in this tree, recycle the memory
// space token by them.
template <class K, class T>
void RbTree<K, T>::Clear() {
  delete this->root_;
  this->root_ = nullptr;
}

#endif  // RBTREE_H_