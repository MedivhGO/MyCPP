//
// Created by Lee on 2022/9/28.
//

#ifndef MYCPPIMPLEMENT_MYBST_H
#define MYCPPIMPLEMENT_MYBST_H

#include <compare>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

class BST {
 public:
  class Node {
   public:
    explicit Node(int value, Node *left = nullptr, Node *right = nullptr);
    Node();                  // Default Constructor
    Node(const Node &node);  // Copy Constructor
    Node(int _value); // NOLINT
    auto operator==(const Node &) const -> bool;
    auto operator<=>(const Node &) const -> std::partial_ordering;
    friend auto operator<<(std::ostream &, const BST::Node &) -> std::ostream &;

    int value_;
    Node *left_;
    Node *right_;
  };

  BST();                                     // Default Constructor
  BST(const BST &bst);                       // Copy Constructor
  ~BST();                                    // Destructor
  BST(BST &&source) noexcept;                // Move Constructor
  BST(std::initializer_list<int>);           // Initializer List Constructor
  auto operator=(const BST &) -> BST &;      // Copy Version
  auto operator=(BST &&) noexcept -> BST &;  // Move Version

  auto GetRoot() -> Node *&;
  void Bfs(const std::function<void(Node *&node)> &func) const;
  auto Length() const -> size_t;  // bst 中的节点数
  auto AddNode(const int &value) -> bool;
  auto FindNode(int value) -> Node **;
  auto FindParrent(int value) -> Node **;
  auto FindPredecessor(int value) -> Node **;
  auto FindSuccessor(int value) -> Node **;
  auto DeleteNode(int value) -> bool;

  auto operator++() const -> const BST &;   // Left ++
  auto operator++(int) const -> BST;  // Right ++
  friend auto operator<<(std::ostream &, const BST &) -> std::ostream &;

 private:
  Node *root_;
};

#endif  // MYCPPIMPLEMENT_MYBST_H
