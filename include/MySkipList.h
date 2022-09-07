//
// Created by Lee on 2022/9/6.
//

#ifndef MYCPPIMPLEMENT_MYSKIPLIST_H
#define MYCPPIMPLEMENT_MYSKIPLIST_H

#define MAXLENGTH 32
#define N 999

#include <vector>
#include <random>

using namespace std;

struct node {
    int key;
    vector<node *> next;

    node(int val, int max_level) : key(val) {
        for (int i = 0; i < max_level; i++) {
            next.emplace_back(nullptr);
        }
    };
};

class Skiplist {
private:
    int _max_height;
    node *_head;

public:
    Skiplist() : _max_height(1) {
        _head = new node(0, MAXLENGTH);
        srand(0);
    }

    node *FindGENode(const int key, vector<node *> &prev) {  // for insert
        node *ptr = _head;
        int level = _max_height - 1;
        while (true) {
            node *next = ptr->next[level];
            if (next != nullptr && key > next->key) {
                ptr = next;
            } else {
                prev[level] = ptr;
                if (level == 0) {
                    return next;
                } else {
                    level--;
                }
            }
        }
    }

    node *FindGENode(const int key) {  // for search
        node *ptr = _head;
        int level = _max_height - 1;
        while (true) {
            node *next = ptr->next[level];
            if (next != nullptr && key > next->key) {
                ptr = next;
            } else {
                if (level == 0) {
                    return next;
                } else {
                    level--;
                }
            }
        }
    }

    int RandomLevel(float factor = 0.5) {
        int level = 1;
        while ((rand() % N) / N > factor && level < MAXLENGTH) {
            level++;
        }
        return level;
    }

    bool search(int target) {
        node *temp = FindGENode(target);
        if (temp && temp->key == target) {
            return true;
        } else {
            return false;
        }
    }

    void add(int num) {
        int ran_level = RandomLevel();
        node *new_node = new node(num, ran_level);
        vector<node *> prev(MAXLENGTH, _head);
        node *temp = FindGENode(num, prev);
        if (ran_level > _max_height) {
            for (int i = _max_height - 1; i < ran_level; i++) {
                prev[i] = _head;
            }
            _max_height = ran_level;
        }
        for (int i = 0; i < _max_height; i++) {
            new_node->next[i] = prev[i]->next[i];
            prev[i]->next[i] = new_node;
        }
    }

    bool erase(int num) {
        vector<node *> prev(MAXLENGTH, _head);
        node *temp = FindGENode(num, prev);
        if (temp == nullptr || temp->key != num) {
            return false;
        }
        int level = temp->next.size();
        for (int i = 0; i < level; i++) {
            prev[i]->next[i] = temp->next[i];
        }
        delete temp;
        return true;
    }
};

#endif //MYCPPIMPLEMENT_MYSKIPLIST_H
