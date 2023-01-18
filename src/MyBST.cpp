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
    std::vector<Node*> nodes;
    bfs([&nodes](BST::Node*& node) { nodes.push_back(node); });
    for (auto& node : nodes) {
        delete node;
    }
}

BST::BST(const BST &_bst) : BST() {
    _bst.bfs([this](BST::Node *&node) { this->add_node(node->value); });
}

BST::BST(BST &&source) {
    if (this == &source) {
        return;
    }
    root = source.root;
    source.root = nullptr;
}

BST::BST(std::initializer_list<int> _list) : BST() {
    for (auto x :  _list) {
        add_node(x);
    }
}

BST::Node *&BST::get_root() { return root; }

void BST::bfs(std::function<void(Node *&node)> func) const {
    std::vector<Node*> tree;
    tree.push_back(root);
    while (!tree.empty()) {
        Node* front = tree.front();
        tree.erase(tree.begin());
        if (front != nullptr) {
            func(front);
            if (front->left != nullptr) {
                tree.push_back(front->left);
            }
            if (front->right != nullptr) {
                tree.push_back(front->right);
            }
        }
    }
}

bool BST::add_node(const int &_value) {
    if (!root) {
        root = new Node(_value, nullptr, nullptr);
        return true;
    }
    Node* pre = nullptr;
    Node* cur = root;
    while (cur) {
        if (cur->value == _value) {
            return false;
        }
        pre = cur;
        if (cur->value > _value) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    if (pre->value > _value) {
        pre->left = new Node(_value, nullptr, nullptr);
    } else {
        pre->right = new Node(_value, nullptr, nullptr);
    }
    return true;
}

size_t BST::length() const {
    size_t count = 0;
    bfs([&count](BST::Node* &node) { ++count; });
    return count;
}

BST::Node **BST::find_node(int _value) {
    Node** ret{&root};
    while(*ret) {
        if ((*ret)->value == _value) {
            return ret;
        } else if ((*ret)->value > _value) {
            ret = &((*ret)->left);
        } else {
            ret = &((*ret)->right);
        }
    }
    return nullptr;
}

BST::Node **BST::find_parrent(int _value) {
    BST::Node **ret{&root};
    if ((*ret)->value == _value) {
        return ret;
    }

    while((*ret)->right || (*ret)->left) {
        if ((*ret)->right && (*ret)->value < _value && (*ret)->right->value == _value) {
            return ret;
        } else if ((*ret)->left && (*ret)->value > _value && (*ret)->left->value == _value) {
            return ret;
        }

        if ((*ret) && (*ret)->value < _value) {
            ret = &(*ret)->right;
        } else if ((*ret) && (*ret)->value > _value) {
            ret = &(*ret)->left;
        }
    }
    return nullptr;
}

// 前驱结点：节点val值小于该节点val值并且值最大的节点
// find_predecessor
BST::Node **BST::find_predecessor(int _value) {
    Node** target_node{find_node(_value)};
    if (target_node == nullptr) {
        return nullptr;
    }

    if (!(*target_node)->left) {
        return target_node;
    } else {
        target_node = &((*target_node)->left);
    }

    while ((*target_node)->right) {
        target_node = &((*target_node)->right);
    }

    return target_node;
}

// 后继节点：节点val值大于该节点val值并且值最小的节点
// find_successor
BST::Node **BST::find_successor(int _value) {
    Node** target_node{find_node(_value)};
    if (target_node == nullptr) {
        return nullptr;
    }

    if (!((*target_node)->right)) {
        return target_node;
    } else {
        target_node = &((*target_node)->right);
    }

    while((*target_node)->left) {
        target_node = &((*target_node)->left);
    }

    return target_node;
}

bool BST::delete_node(int _value) {
    Node** one_node = find_node(_value);
    if (one_node == nullptr) {
        return false;
    }
    Node* target_node = *one_node;
    return true;
}

std::ostream &operator<<(std::ostream &_output, const BST &_bst) {
    _output << std::string(80, '*') << std::endl;
    _bst.bfs([&_output](BST::Node *&node) { _output << *node << std::endl; });
    _output << "binary search tree size: " << _bst.length() << std::endl;
    _output << std::string(80, '*');
    return _output;
}

BST &BST::operator=(const BST &_bst) {
    if (&_bst == this) {
        return *this;
    }
    this->~BST();
    root = nullptr;
    _bst.bfs([this](BST::Node *&node) { this->add_node(node->value); });
    return *this;
}

BST &BST::operator=(BST &&_bst) {
    this->~BST();
    root = _bst.root;
    _bst.root = nullptr;
    return *this;
}

// ++object
const BST &BST::operator++() const {
    bfs([](BST::Node *&node) { (node->value)++; });
    return *this;
}

// object++
const BST BST::operator++(int) const {
    BST ret(*this);
    bfs([](BST::Node *&node) { (node->value)++; });
    return ret;
}
