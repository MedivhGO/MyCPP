//
// Created by Lee on 2023/1/11.
//

#include "MyBST.h"

BST::Node::Node(int _value, Node *_left, Node *_right) : value{_value}, left{_left}, right{_right} {}

BST::Node::Node() : Node(0, nullptr, nullptr) {}

BST::Node::Node(const Node &_node) : value{_node.value}, left{_node.left}, right{_node.right} {}

std::ostream &operator<<(std::ostream &output_, const BST::Node &_node) {
    output_ << std::setw(18) << std::left << &_node << " => "
            << "value:" << std::setw(8) << std::left << _node.value << "left:" << std::setw(18) << std::left
            << _node.left << "right:" << std::setw(18) << std::left << _node.right;
    return output_;
}

bool BST::Node::operator==(const Node &_node) const { return value == _node.value; }

std::partial_ordering BST::Node::operator<=>(const Node &_node) const { return value <=> _node.value; }

BST::BST() : root{nullptr} {}

BST::~BST() {
}

BST::BST(const BST &_bst) : BST() {
    _bst.bfs([this](BST::Node *&node) { this->add_node(node->value); });
}

BST::BST(BST &&source) {

}

BST::BST(std::initializer_list<int> _list) : BST() {

}

BST::Node *&BST::get_root() { return root; }

void BST::bfs(std::function<void(Node *&node)> func) const {

}

bool BST::add_node(const int &_value) {

}

size_t BST::length() const {

}

BST::Node **BST::find_node(int _value) {

}

BST::Node **BST::find_parrent(int _value) {

}

BST::Node **BST::find_successor(int _value) {

}

bool BST::delete_node(int _value) {

}

std::ostream &operator<<(std::ostream &_output, const BST &_bst) {
    _output << std::string(80, '*') << std::endl;
    _bst.bfs([&_output](BST::Node *&node) { _output << *node << std::endl; });
    _output << "binary search tree size: " << _bst.length() << std::endl;
    _output << std::string(80, '*');
    return _output;
}

BST &BST::operator=(const BST &_bst) {

}

BST &BST::operator=(BST &&_bst) {

}

const BST &BST::operator++() const {

}

const BST BST::operator++(int) const {

}
