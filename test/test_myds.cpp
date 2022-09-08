//
// Created by Lee on 2022/9/6.
//
#include <gtest/gtest.h>

#include "MyRandom.h"
#include "MySkipList.h"
#include "hashmap.h"

TEST(MyDataStructTest, test1) {
    Skiplist skiplist;
    for (int i = 0; i < 100; ++i) {
        skiplist.add(i);
    }

    RandomNumberGenerator rng(0, 99);
    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(skiplist.search(rng.GetRandomNumberByMt19937()));
    }
}

template <typename KeyType, typename MappedTyped>
std::set<KeyType> find_keys(const HashMap<KeyType, MappedTyped> &map) {
    std::set<KeyType> res;
    for (auto it = map.begin(); it != map.end(); ++it) {
        auto [key, val] = *it;
        res.insert(key);
    }
    return res;
}

TEST(MyDataStructTest, test2) {
    HashMap<string, int> map;
    map.insert({"Anna", 2019});
    map.insert({"Avery", 2019});
    map.insert({"Nikhil", 2020});
    map.insert({"Ethan", 2020});
    map.insert({"Frankie", 2021});
    map.insert({"Sathya", 2021});
    // demonstrate the map in action!
    map.debug();
    map.rehash(5);
    map.debug();
    std::set<string> keys = find_keys(map);
    for (auto it = keys.begin(); it != keys.end(); ++it) {
        cout << *it << endl;
    }

    HashMap<string, int> map2(map);
    map2.debug();
    HashMap<string, int> map3;
    map3 = map;
    map3.debug();

    HashMap<string, int> map5(map3);
    HashMap<string, int> map6(map3);

    HashMap<string, int> map7(std::move(map5));
    map7.debug();

    HashMap<string, int> map8;
    map8 = std::move(map6);
    map8.debug();
}
