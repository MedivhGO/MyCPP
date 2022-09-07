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
    vector<std::shared_ptr<node>> next;

    node(int val, int max_level) : key(val) {
        for (int i = 0; i < max_level; i++) {
            next.emplace_back(nullptr);
        }
    };
};

class Skiplist {
private:
    int _max_height;
    std::shared_ptr<node> _head;

public:
    Skiplist() : _max_height(1) {
        _head = std::make_shared<node>(0, MAXLENGTH);
        srand(0);
    }

    std::shared_ptr<node> FindGENode(const int key, vector<std::shared_ptr<node>> &prev) {  // for insert
        std::shared_ptr<node> ptr = _head;
        int level = _max_height - 1;
        while (true) {
            std::shared_ptr<node> next = ptr->next[level];
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

    std::shared_ptr<node> FindGENode(const int key) {  // for search
        std::shared_ptr<node> ptr = _head;
        int level = _max_height - 1;
        while (true) {
            std::shared_ptr<node> next = ptr->next[level];
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
        std::shared_ptr<node> temp = FindGENode(target);
        if (temp && temp->key == target) {
            return true;
        } else {
            return false;
        }
    }

    void add(int num) {
        int ran_level = RandomLevel();
        std::shared_ptr<node> new_node = std::make_shared<node>(num, ran_level);
        vector<std::shared_ptr<node>> prev(MAXLENGTH, _head);
        std::shared_ptr<node> temp = FindGENode(num, prev);
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
        vector<std::shared_ptr<node>> prev(MAXLENGTH, _head);
        std::shared_ptr<node> temp = FindGENode(num, prev);
        if (temp == nullptr || temp->key != num) {
            return false;
        }
        int level = temp->next.size();
        for (int i = 0; i < level; i++) {
            prev[i]->next[i] = temp->next[i];
        }
        temp.reset();
        return true;
    }

    ~Skiplist() = default;
};

#endif //MYCPPIMPLEMENT_MYSKIPLIST_H
