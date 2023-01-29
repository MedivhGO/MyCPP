//
// Created by Lee on 2022/9/28.
//
#include <algorithm>
#include "MyBST.h"
#include "gtest/gtest.h"

TEST(MyBST, TEST1) {
  BST::Node left{10, nullptr, nullptr};
  BST::Node right{10, nullptr, nullptr};
  BST::Node node{10, &left, &right};
  EXPECT_EQ(node.left_, &left);
  EXPECT_EQ(node.right_, &right);
}

TEST(MyBST, TEST2) {
  BST::Node left_{10, nullptr, nullptr};
  BST::Node right{10, nullptr, nullptr};
  BST::Node node{10, &left_, &right};
  BST::Node copy{node};
  EXPECT_EQ(copy.value_, node.value_);
  EXPECT_EQ(copy.left_, node.left_);
  EXPECT_EQ(copy.right_, node.right_);
}

TEST(MyBST, TEST3) {
  BST::Node default_{};
  EXPECT_EQ(default_.value_, 0);
  EXPECT_EQ(default_.left_, nullptr);
  EXPECT_EQ(default_.right_, nullptr);
}

TEST(MyBST, TEST4) {
  BST::Node left_{10, nullptr, nullptr};
  BST::Node right{10, nullptr, nullptr};
  BST::Node node{10, &left_, &right};

  std::cout << "address of node: " << &node << std::endl;
  std::cout << "address of left_: " << &left_ << std::endl;
  std::cout << "address of right: " << &right << std::endl;
  std::cout << std::string(80, '*') << std::endl;
  std::cout << "PRINT A NODE" << std::endl;
  std::cout << node << std::endl;
  std::cout << std::string(80, '*') << std::endl;
}

TEST(MyBST, TEST5) {
  BST::Node node{5, nullptr, nullptr};

  EXPECT_TRUE(node > 4);
  EXPECT_FALSE(node > 5);
  EXPECT_TRUE(node >= 5);
  EXPECT_FALSE(node >= 6);
  EXPECT_TRUE(node < 6);
  EXPECT_FALSE(node < 2);
  EXPECT_TRUE(node <= 5);
  EXPECT_FALSE(node <= 2);
  EXPECT_TRUE(node == 5);
  EXPECT_FALSE(node == 3);

  EXPECT_TRUE(4 < node);
  EXPECT_FALSE(5 < node);
  EXPECT_TRUE(5 <= node);
  EXPECT_FALSE(6 <= node);
  EXPECT_TRUE(6 > node);
  EXPECT_FALSE(2 > node);
  EXPECT_TRUE(5 >= node);
  EXPECT_FALSE(2 >= node);
  EXPECT_TRUE(5 == node);
  EXPECT_FALSE(3 == node);
}

TEST(MyBST, TEST6) {
  BST bst{};
  EXPECT_EQ(bst.GetRoot(), nullptr);
}

TEST(MyBST, TEST7) {
  BST bst{};
  bst.AddNode(10);
  bst.AddNode(20);
  bst.AddNode(5);
  EXPECT_EQ(bst.GetRoot()->value_, 10);
  EXPECT_EQ(bst.GetRoot()->left_->value_, 5);
  EXPECT_EQ(bst.GetRoot()->right_->value_, 20);
}

TEST(MyBST, TEST8) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(53);
  bst.AddNode(15);
  bst.AddNode(7);

  EXPECT_FALSE(bst.AddNode(53));
  EXPECT_EQ(bst.GetRoot()->left_->left_->value_, 7);
  EXPECT_EQ(bst.GetRoot()->left_->right_->value_, 15);
  EXPECT_EQ(bst.GetRoot()->right_->right_->value_, 53);
}

TEST(MyBST, TEST9) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(53);

  EXPECT_FALSE(bst.AddNode(50));

  std::vector<int> values;
  bst.Bfs([&values](BST::Node *&node) { values.push_back(node->value_); });
  EXPECT_EQ(values.size(), 4);
  EXPECT_TRUE(std::find(values.begin(), values.end(), 25) != values.end());
  EXPECT_TRUE(std::find(values.begin(), values.end(), 10) != values.end());
  EXPECT_TRUE(std::find(values.begin(), values.end(), 50) != values.end());
  EXPECT_TRUE(std::find(values.begin(), values.end(), 53) != values.end());
}

TEST(MyBST, TEST10) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(53);
  bst.AddNode(15);
  bst.AddNode(7);

  EXPECT_EQ(bst.Length(), 6);
}

TEST(MyBST, TEST11) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(53);
  bst.AddNode(15);
  bst.AddNode(7);

  std::cout << "PRINT A BINARY SEARCH TREE" << std::endl;
  std::cout << bst << std::endl;
}

TEST(MyBST, TEST12) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(53);
  bst.AddNode(15);
  bst.AddNode(7);

  BST::Node **node{bst.FindNode(10)};
  EXPECT_EQ((*node)->value_, 10);
  EXPECT_EQ(((*node)->left_)->value_, 7);
  EXPECT_EQ((*node)->right_->value_, 15);
}

TEST(MyBST, TEST13) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(53);
  bst.AddNode(15);
  bst.AddNode(7);

  BST::Node **node{bst.FindNode(11)};
  EXPECT_EQ(node, nullptr);
}

TEST(MyBST, TEST14) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(53);
  bst.AddNode(15);
  bst.AddNode(7);

  std::cout << "here" << std::endl;
  BST::Node **node{bst.FindParrent(15)};
  std::cout << "here2" << std::endl;
  EXPECT_EQ((*node)->value_, 10);
  EXPECT_EQ((*node)->left_->value_, 7);
  EXPECT_EQ((*node)->right_->value_, 15);
}

TEST(MyBST, TEST15) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(53);
  bst.AddNode(15);
  bst.AddNode(7);
  bst.AddNode(8);
  bst.AddNode(9);

  BST::Node **node{bst.FindPredecessor(10)};
  EXPECT_EQ((*node)->value_, 9);
  EXPECT_EQ((*node)->left_, nullptr);
  EXPECT_EQ((*node)->right_, nullptr);

  BST::Node **node_suc{bst.FindSuccessor(10)};
  EXPECT_EQ((*node_suc)->value_, 15);
  EXPECT_EQ((*node_suc)->left_, nullptr);
  EXPECT_EQ((*node_suc)->right_, nullptr);
}

TEST(MyBST, TEST16) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(53);
  bst.AddNode(15);
  bst.AddNode(7);
  bst.AddNode(8);
  bst.AddNode(9);

  EXPECT_FALSE(bst.DeleteNode(11));
}

TEST(MyBST, TEST17) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(53);
  bst.AddNode(15);
  bst.AddNode(7);
  bst.AddNode(8);
  bst.AddNode(9);

  EXPECT_EQ(bst.Length(), 8);
  EXPECT_TRUE(bst.DeleteNode(53));  // leaf node
  EXPECT_EQ(bst.Length(), 7);
  EXPECT_EQ(bst.GetRoot()->right_->right_, nullptr);
}

TEST(MyBST, TEST18) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(65);
  bst.AddNode(60);
  bst.AddNode(70);
  bst.AddNode(5);
  bst.AddNode(2);
  bst.AddNode(7);
  bst.AddNode(75);

  EXPECT_EQ(bst.Length(), 10);
  EXPECT_TRUE(bst.DeleteNode(10));  // only left_ child
  EXPECT_EQ(bst.Length(), 9);
  EXPECT_EQ(bst.GetRoot()->left_->value_, 5);
  EXPECT_EQ(bst.GetRoot()->left_->left_->value_, 2);
  EXPECT_EQ(bst.GetRoot()->left_->right_->value_, 7);
  EXPECT_EQ(bst.GetRoot()->left_->right_->left_, nullptr);
  EXPECT_EQ(bst.GetRoot()->left_->right_->right_, nullptr);
}

TEST(MyBST, TEST19) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(65);
  bst.AddNode(60);
  bst.AddNode(70);
  bst.AddNode(5);
  bst.AddNode(2);
  bst.AddNode(7);
  bst.AddNode(75);

  BST::Node *address_of_5{*bst.FindNode(5)};
  BST::Node *address_of_2{*bst.FindNode(2)};

  EXPECT_TRUE(bst.DeleteNode(10));  // only left_ child

  // the remaining nodes should not be regenerated
  // => address of remaining nodes before and after deletion should be the same
  // when deleting a node: just reconnect the needed parents and children
  EXPECT_EQ(*bst.FindNode(5), address_of_5);
  EXPECT_EQ(*bst.FindNode(2), address_of_2);
}

TEST(MyBST, TEST20) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(65);
  bst.AddNode(60);
  bst.AddNode(70);
  bst.AddNode(5);
  bst.AddNode(2);
  bst.AddNode(7);
  bst.AddNode(75);

  EXPECT_EQ(bst.Length(), 10);
  EXPECT_TRUE(bst.DeleteNode(50));  // only right child
  EXPECT_EQ(bst.Length(), 9);
  EXPECT_EQ(bst.GetRoot()->right_->value_, 65);
  EXPECT_EQ(bst.GetRoot()->right_->right_->value_, 70);
  EXPECT_EQ(bst.GetRoot()->right_->right_->right_->value_, 75);
  EXPECT_EQ(bst.GetRoot()->right_->left_->value_, 60);
  EXPECT_EQ(bst.GetRoot()->right_->left_->left_, nullptr);
  EXPECT_EQ(bst.GetRoot()->right_->left_->right_, nullptr);
}

TEST(MyBST, TEST21) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(65);
  bst.AddNode(60);
  bst.AddNode(70);
  bst.AddNode(5);
  bst.AddNode(2);
  bst.AddNode(7);
  bst.AddNode(75);

  BST::Node *address_of_60{*bst.FindNode(60)};
  BST::Node *address_of_75{*bst.FindNode(75)};

  EXPECT_TRUE(bst.DeleteNode(50));  // only left_ child

  // the remaining nodes should not be regenerated
  // => address of remaining nodes before and after deletion should be the same
  // when deleting a node: just reconnect the needed parents and children
  EXPECT_EQ(*bst.FindNode(60), address_of_60);
  EXPECT_EQ(*bst.FindNode(75), address_of_75);
}

TEST(MyBST, TEST22) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(65);
  bst.AddNode(60);
  bst.AddNode(70);
  bst.AddNode(5);
  bst.AddNode(2);
  bst.AddNode(7);
  bst.AddNode(75);
  bst.AddNode(20);
  bst.AddNode(15);

  EXPECT_EQ(bst.Length(), 12);
  EXPECT_TRUE(bst.DeleteNode(10));  // both children exist
  EXPECT_EQ(bst.Length(), 11);
  EXPECT_EQ(bst.GetRoot()->left_->value_, 7);
  EXPECT_EQ(bst.GetRoot()->left_->left_->right_, nullptr);
  EXPECT_EQ(bst.GetRoot()->left_->left_->value_, 5);
}

TEST(MyBST, TEST23) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(65);
  bst.AddNode(60);
  bst.AddNode(70);
  bst.AddNode(5);
  bst.AddNode(2);
  bst.AddNode(7);
  bst.AddNode(75);
  bst.AddNode(20);
  bst.AddNode(15);
  bst.AddNode(22);

  EXPECT_EQ(bst.Length(), 13);
  EXPECT_TRUE(bst.DeleteNode(25));  // both children exist
  EXPECT_EQ(bst.Length(), 12);
  EXPECT_EQ(bst.GetRoot()->value_, 22);
  EXPECT_EQ(bst.GetRoot()->left_->value_, 10);
  EXPECT_EQ(bst.GetRoot()->left_->right_->right_, nullptr);
}

TEST(MyBST, TEST24) {
  BST bst{};
  bst.AddNode(5);
  bst.AddNode(10);
  bst.AddNode(50);
  BST copy{bst};
  EXPECT_EQ(bst.Length(), copy.Length());
  EXPECT_EQ(bst.GetRoot()->right_->value_, copy.GetRoot()->right_->value_);
}

TEST(MyBST, TEST25) {
  BST bst{};
  bst.AddNode(5);
  bst.AddNode(10);
  bst.AddNode(50);

  BST equal{};
  bst.AddNode(51);
  bst.AddNode(66);
  bst.AddNode(10);
  equal = bst;
  EXPECT_EQ(bst.Length(), equal.Length());
  EXPECT_EQ(bst.GetRoot()->right_->right_->value_, equal.GetRoot()->right_->right_->value_);
}

TEST(MyBST, TEST26) {
  BST bst{};
  bst.AddNode(5);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(1);
  bst.AddNode(2);
  bst.AddNode(4);
  bst.AddNode(60);
  bst.AddNode(8);

  bst = bst;
  EXPECT_EQ(bst.GetRoot()->right_->left_->value_, 8);
}

TEST(MyBST, TEST27) {
  BST bst{};
  bst.AddNode(5);
  bst.AddNode(10);
  bst.AddNode(50);

  int test_value{bst.GetRoot()->right_->value_};
  BST::Node *address_of_root{*bst.FindNode(5)};

  BST move{std::move(bst)};
  EXPECT_EQ(address_of_root, move.GetRoot());
  EXPECT_EQ(test_value, move.GetRoot()->right_->value_);
}

TEST(MyBST, TEST28) {
  BST bst{};
  bst.AddNode(25);
  bst.AddNode(10);
  bst.AddNode(50);
  bst.AddNode(65);
  bst.AddNode(5);
  bst.AddNode(20);
  bst.AddNode(22);

  int test_value{bst.GetRoot()->left_->right_->value_};
  BST::Node *address_of_root{*bst.FindNode(25)};

  BST move{};
  move = std::move(bst);
  EXPECT_EQ(address_of_root, move.GetRoot());
  EXPECT_EQ(test_value, move.GetRoot()->left_->right_->value_);
}

TEST(MyBST, TEST29) {
  BST bst1{5, 1, 10, 2, 8, 50, 4, 60};
  BST bst2{3, 2, 100, 20, 8, 50, 4, 60, 44, 23};
  EXPECT_EQ((*bst1.FindPredecessor(5))->value_, 4);
  EXPECT_EQ((*bst1.FindSuccessor(5))->value_, 8);
  EXPECT_EQ(bst2.Length(), 10);
}

TEST(MyBST, TEST30) {
  BST bst{5, 1, 10, 2, 8, 50, 4, 60};
  BST bst2{++bst};

  std::vector<int> values;
  EXPECT_EQ(bst.GetRoot()->value_, 6);
  EXPECT_EQ(bst2.GetRoot()->value_, 6);
  EXPECT_EQ(bst.GetRoot()->right_->right_->right_->value_, 61);
  EXPECT_EQ(bst2.GetRoot()->right_->right_->right_->value_, 61);
}

TEST(MyBST, TEST31) {
  BST bst1{5, 1, 10, 2, 8, 50, 4, 60};
  BST bst2{bst1++};

  std::vector<int> values1;
  bst1.Bfs([&values1](BST::Node *&node) { values1.push_back(node->value_); });

  std::vector<int> values2;
  bst2.Bfs([&values2](BST::Node *&node) { values2.push_back(node->value_); });

  for (size_t i{}; i < values1.size(); i++) {
    EXPECT_EQ(values2[i], values1[i] - 1);
  }
}