//
// Created by Lee on 2023/1/11.
//

#include "MyBST.h"

//----------------------------------------------------------------
// Node Constructor
BST::Node::Node(int _value, Node *_left, Node *_right) : value{_value}, left{_left}, right{_right} {}
//----------------------------------------------------------------
// Default Constructor
BST::Node::Node()
    : Node(0, nullptr, nullptr) // Set The Default Value = 0 and it's Childeren Pointer = Nullptr
{}
//----------------------------------------------------------------
// Copy Cunstructor
BST::Node::Node(const Node &_node) : value{_node.value}, left{_node.left}, right{_node.right} {}
//----------------------------------------------------------------
std::ostream &operator<<(std::ostream &output_, const BST::Node &_node) {
    output_ << std::setw(18) << std::left << &_node << " => "
            << "value:" << std::setw(8) << std::left << _node.value << "left:" << std::setw(18) << std::left
            << _node.left << "right:" << std::setw(18) << std::left << _node.right;
    return output_;
}
//----------------------------------------------------------------
bool BST::Node::operator==(const Node &_node) const { return value == _node.value; }
//----------------------------------------------------------------
std::partial_ordering BST::Node::operator<=>(const Node &_node) const { return value <=> _node.value; }
//----------------------------------------------------------------
BST::BST() // Default Constructor
    : root{nullptr} {}
//----------------------------------------------------------------
BST::~BST() // Destructor
{
    std::vector<Node *> nodes;
    bfs([&nodes](BST::Node *&node) { nodes.push_back(node); });
    for (auto &node : nodes)
        delete node;
}
//----------------------------------------------------------------
BST::BST(const BST &_bst) // Copy Constructor
    : BST() {             // Construct a New BST with Same Node's Value as Received BST
    _bst.bfs([this](BST::Node *&node) { this->add_node(node->value); });
}
//----------------------------------------------------------------
BST::BST(BST &&source) // Move Constructor
{
    root = source.root;
    source.root = nullptr;
}
//----------------------------------------------------------------
BST::BST(std::initializer_list<int> _list) // Initializer List Constructor
    : BST() {
    for (auto &i : _list)
        add_node(i);
}
//----------------------------------------------------------------
BST::Node *&BST::get_root() { return root; } // get_root() function
//----------------------------------------------------------------
void BST::bfs(std::function<void(Node *&node)> func) const // bfs function
{
    std::vector<Node *> Tree;
    Tree.push_back(root);
    while (!Tree.empty()) {
        Node *node{Tree.front()}; // Return The First Element Of The Tree Vector
        Tree.erase(Tree.begin()); // Delete The First Element Of The Tree Vector
        if (node != nullptr) {
            func(node);
            if (node->left)
                Tree.push_back(node->left);
            if (node->right)
                Tree.push_back(node->right);
        }
    }
}
//----------------------------------------------------------------
bool BST::add_node(const int &_value) {
    if (!root) {
        root = new Node(_value);
        return true;
    }
    Node *Temp{root};
    while (true) {
        if (Temp->value == _value)
            return false;

        if (Temp->value < _value) {
            if (!Temp->right) {
                Temp->right = new Node(_value);
                return true;
            }
            Temp = Temp->right;

        } else {
            if (!Temp->left) {
                Temp->left = new Node(_value);
                return true;
            }
            Temp = Temp->left;
        }
    }
}
//----------------------------------------------------------------
size_t BST::length() const {
    size_t length{};
    this->bfs([&length](BST::Node *&node) { ++length; });
    return length;
}
//----------------------------------------------------------------
BST::Node **BST::find_node(int _value) {
    BST::Node **Temp{&root};
    while (*Temp) {
        if ((*Temp)->value == _value)
            return Temp;

        if ((*Temp)->value < _value)
            Temp = &(*Temp)->right;
        else
            Temp = &(*Temp)->left;
    }
    return nullptr;
}
//----------------------------------------------------------------
BST::Node **BST::find_parrent(int _value) {
    BST::Node **Temp{&root};
    if ((*Temp)->value == _value)
        return Temp;

    while ((*Temp)->right || (*Temp)->left) {
        if ((*Temp)->right && (*Temp)->value < _value && (*Temp)->right->value == _value)
            return Temp;
        else if ((*Temp)->left && (*Temp)->value > _value && (*Temp)->left->value == _value)
            return Temp;

        if ((*Temp)->right && (*Temp)->value < _value)
            Temp = &(*Temp)->right;
        else if ((*Temp)->left && (*Temp)->value > _value)
            Temp = &(*Temp)->left;
    }
    return nullptr;
}
//----------------------------------------------------------------
BST::Node **BST::find_successor(int _value) {
    BST::Node **Temp{find_node(_value)};

    if (!(*Temp)->left)
        return Temp;
    else
        Temp = &(*Temp)->left;

    while ((*Temp)->right) {
        Temp = &(*Temp)->right;
    }
    return Temp;
}
//----------------------------------------------------------------
bool BST::delete_node(int _value) {
    BST::Node **Temp{find_node(_value)};
    BST::Node **Parrent{find_parrent(_value)};

    if (Temp == nullptr)
        return false;
    // For The Leaf Node
    if ((*Temp)->left == nullptr && (*Temp)->right == nullptr) {
        if ((*Parrent)->value > _value)
            (*Parrent)->left = nullptr;
        else
            (*Parrent)->right = nullptr;
        return true;
    }
    // If The Node just has Left Child
    else if ((*Temp)->left == nullptr) {
        if ((*Parrent)->value > _value)
            (*Parrent)->left = (*Temp)->right;
        else
            (*Parrent)->right = (*Temp)->right;
        return true;
    }
    // If The Node just has Right Child
    else if ((*Temp)->right == nullptr) {
        if ((*Parrent)->value > _value)
            (*Parrent)->left = (*Temp)->left;
        else
            (*Parrent)->right = (*Temp)->left;
        return true;
        // If The Node has 2 Childeren
    } else if ((*Temp)->left != nullptr && (*Temp)->right != nullptr) {
        BST::Node **successor{find_successor(_value)};
        BST::Node **Successor_Parrent{find_parrent((*successor)->value)};
        (*Temp)->value = (*successor)->value;

        if ((*Successor_Parrent)->value > (*successor)->value)
            (*Successor_Parrent)->left = nullptr;
        else
            (*Successor_Parrent)->right = nullptr;
        return true;
    }
    return true;
}
//----------------------------------------------------------------
std::ostream &operator<<(std::ostream &_output, const BST &_bst) {
    _output << std::string(80, '*') << std::endl;
    _bst.bfs([&_output](BST::Node *&node) { _output << *node << std::endl; });
    _output << "binary search tree size: " << _bst.length() << std::endl;
    _output << std::string(80, '*');
    return _output;
}
//----------------------------------------------------------------
BST &BST::operator=(const BST &_bst) // Copy Version
{
    if (this == &_bst)
        return *this;
    // Copy Version of Operator =
    delete root;
    _bst.bfs([this](BST::Node *&node) { this->add_node(node->value); });
    return *this;
}
//----------------------------------------------------------------
BST &BST::operator=(BST &&_bst) // Move Version
{
    // Move Version of Operator =
    delete root;
    root = _bst.root;
    _bst.root = nullptr;
    return *this;
}
//----------------------------------------------------------------
const BST &BST::operator++() const {
    // Left ++
    this->bfs([this](BST::Node *&node) { node->value++; });
    return *this;
}
//----------------------------------------------------------------
const BST BST::operator++(int) const {
    // Right ++
    BST Temp{*this};
    this->bfs([this](BST::Node *&node) { node->value++; });
    return Temp;
}
//--------------------------The End-------------------------------