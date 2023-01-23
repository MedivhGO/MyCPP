//
// Created by Lee on 2023/1/11.
//

#include "MyBST.h"

BST::Node::Node(int _value, Node *_left, Node *_right) : value_{_value}, left_{_left}, right_{_right} {}

BST::Node::Node(int _value) : value_{_value}, left_{nullptr}, right_{nullptr} {}

BST::Node::Node() : Node(0, nullptr, nullptr) {}

BST::Node::Node(const Node &_node) : value_{_node.value_}, left_{_node.left_}, right_{_node.right_} {}

std::ostream &operator<<(std::ostream &output_, const BST::Node &_node) {
    output_ << std::setw(18) << std::left << &_node << " => "
            << "value:" << std::setw(8) << std::left << _node.value_ << "left:" << std::setw(18) << std::left
            << _node.left_ << "right:" << std::setw(18) << std::left << _node.right_;
    return output_;
}

bool BST::Node::operator==(const Node &_node) const { return value_ == _node.value_; }

std::partial_ordering BST::Node::operator<=>(const Node &_node) const { return value_ <=> _node.value_; }

BST::BST() : root_{nullptr} {}

BST::~BST() {
    std::vector<Node*> nodes;
    Bfs([&nodes](BST::Node*& node) { nodes.push_back(node); });
    for (auto& node : nodes) {
        delete node;
    }
}

BST::BST(const BST &_bst) : BST() {
    _bst.Bfs([this](BST::Node *&node) { this->AddNode(node->value_); });
}

BST::BST(BST &&source) noexcept {
    if (this == &source) {
        return;
    }
    root_ = source.root_;
    source.root_ = nullptr;
}

BST::BST(std::initializer_list<int> _list) : BST() {
    for (auto x :  _list) {
      AddNode(x);
    }
}

BST::Node *&BST::GetRoot() { return root_; }

void BST::Bfs(const std::function<void(Node *&node)>& func) const {
    std::vector<Node*> tree;
    tree.push_back(root_);
    while (!tree.empty()) {
        Node* front = tree.front();
        tree.erase(tree.begin());
        if (front != nullptr) {
            func(front);
            if (front->left_ != nullptr) {
                tree.push_back(front->left_);
            }
            if (front->right_ != nullptr) {
                tree.push_back(front->right_);
            }
        }
    }
}

bool BST::AddNode(const int &_value) {
    if (!root_) {
        root_ = new Node(_value, nullptr, nullptr);
        return true;
    }
    Node* pre = nullptr;
    Node* cur = root_;
    while (cur) {
        if (cur->value_ == _value) {
            return false;
        }
        pre = cur;
        if (cur->value_ > _value) {
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }
    if (pre->value_ > _value) {
        pre->left_ = new Node(_value, nullptr, nullptr);
    } else {
        pre->right_ = new Node(_value, nullptr, nullptr);
    }
    return true;
}

size_t BST::Length() const {
    size_t count = 0;
    Bfs([&count](BST::Node* &node) { ++count; });
    return count;
}

BST::Node **BST::FindNode(int _value) {
    Node** ret{&root_};
    while(*ret) {
        if ((*ret)->value_ == _value) {
            return ret;
        } else if ((*ret)->value_ > _value) {
            ret = &((*ret)->left_);
        } else {
            ret = &((*ret)->right_);
        }
    }
    return nullptr;
}

BST::Node **BST::FindParrent(int _value) {
    BST::Node **ret{&root_};
    if ((*ret)->value_ == _value) {
        return ret;
    }

    while((*ret)->right_ || (*ret)->left_) {
        if ((*ret)->right_ && (*ret)->value_ < _value && (*ret)->right_->value_ == _value) {
            return ret;
        } else if ((*ret)->left_ && (*ret)->value_ > _value && (*ret)->left_->value_ == _value) {
            return ret;
        }

        if ((*ret) && (*ret)->value_ < _value) {
            ret = &(*ret)->right_;
        } else if ((*ret) && (*ret)->value_ > _value) {
            ret = &(*ret)->left_;
        }
    }
    return nullptr;
}

// 前驱结点：节点val值小于该节点val值并且值最大的节点
// find_predecessor
BST::Node **BST::FindPredecessor(int _value) {
    Node** target_node{FindNode(_value)};
    if (target_node == nullptr) {
        return nullptr;
    }

    if (!(*target_node)->left_) {
        return target_node;
    } else {
        target_node = &((*target_node)->left_);
    }

    while ((*target_node)->right_) {
        target_node = &((*target_node)->right_);
    }

    return target_node;
}

// 后继节点：节点val值大于该节点val值并且值最小的节点
// find_successor
BST::Node **BST::FindSuccessor(int _value) {
    Node** target_node{FindNode(_value)};
    if (target_node == nullptr) {
        return nullptr;
    }

    if (!((*target_node)->right_)) {
        return target_node;
    } else {
        target_node = &((*target_node)->right_);
    }

    while((*target_node)->left_) {
        target_node = &((*target_node)->left_);
    }

    return target_node;
}

bool BST::DeleteNode(int _value) {
    Node** one_node = FindNode(_value);
    if (one_node == nullptr) {
        return false;
    }
    Node** parent_node = FindParrent(_value);
    // 是叶子节点
    if (!(*one_node)->left_ && !(*one_node)->right_) {
        if ((*one_node)->value_ > (*parent_node)->value_) {
            delete (*one_node);
            (*parent_node)->right_ = nullptr;
        } else {
            delete (*one_node);
            (*parent_node)->left_ = nullptr;
        }
        return true;
    }
    // one_node 有两个孩子
    if ((*one_node)->left_ && (*one_node)->right_) {
        Node* t = (*one_node)->right_;
        Node* del_left = (*one_node)->left_;
        Node* find_sub_left = (*one_node)->right_;
        while (find_sub_left->left_) {
            find_sub_left = find_sub_left->left_;
        }
        find_sub_left->left_ = del_left;

        if ((*one_node)->value_ > (*parent_node)->value_) {
            delete (*one_node);
            (*parent_node)->right_ = t;
        } else {
            delete (*one_node);
            (*parent_node)->left_ = t;
        }
        return true;
    }
    // one_node 有一个孩子
    if ((*one_node)->left_) {
        Node* t = (*one_node)->left_;
        if ((*one_node)->value_ > (*parent_node)->value_) {
            delete (*one_node);
            (*parent_node)->right_ = t;
        } else {
            delete (*one_node);
            (*parent_node)->left_ = t;
        }
        return true;
    }

    if ((*one_node)->right_) {
        Node* t = (*one_node)->right_;
        if ((*one_node)->value_ > (*parent_node)->value_) {
            delete (*one_node);
            (*parent_node)->right_ = t;
        } else {
            delete (*one_node);
            (*parent_node)->left_ = t;
        }
        return true;
    }
    return false;
}

auto operator<<(std::ostream &_output, const BST &_bst) -> std::ostream & {
    _output << std::string(80, '*') << std::endl;
    _bst.Bfs([&_output](BST::Node *&node) { _output << *node << std::endl; });
    _output << "binary search tree size: " << _bst.Length() << std::endl;
    _output << std::string(80, '*');
    return _output;
}

auto BST::operator=(const BST &_bst) -> BST & {
    if (&_bst == this) {
        return *this;
    }
    this->~BST();
    root_ = nullptr;
    _bst.Bfs([this](BST::Node *&node) { this->AddNode(node->value_); });
    return *this;
}

auto BST::operator=(BST &&_bst) noexcept -> BST & {
    this->~BST();
    root_ = _bst.root_;
    _bst.root_ = nullptr;
    return *this;
}

// ++object
auto BST::operator++() const -> const BST & {
    Bfs([](BST::Node *&node) { (node->value_)++; });
    return *this;
}

// object++
auto BST::operator++(int) const -> const BST {
    BST ret(*this);
    Bfs([](BST::Node *&node) { (node->value_)++; });
    return ret;
}
