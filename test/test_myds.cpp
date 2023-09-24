//
// Created by Lee on 2022/9/6.
//
#include <gtest/gtest.h>

#include "MyRBTree.h"
#include "MyRandom.h"
#include "MySkipList.h"
#include "hashmap.h"

template <typename KeyType, typename MappedTyped>
auto FindKeys(const HashMap<KeyType, MappedTyped> &map) -> std::set<KeyType> {
  std::set<KeyType> res;
  for (auto it = map.begin(); it != map.end(); ++it) {
    auto [key, val] = *it;
    res.insert(key);
  }
  return res;
}

TEST(MyDataStructTest, test2) {
  HashMap<std::string, int> map;
  map.insert({"Anna", 2019});
  map.insert({"Avery", 2019});
  map.insert({"Nikhil", 2020});
  map.insert({"Ethan", 2020});
  map.insert({"Frankie", 2021});
  map.insert({"Sathya", 2021});

  std::set<std::string> keys = FindKeys(map);
  EXPECT_EQ(keys.size(), 6);

  map.rehash(5);
  EXPECT_EQ(map.bucket_count(), 5);

  HashMap<std::string, int> map2(map);
  EXPECT_EQ(map2, map);

  HashMap<std::string, int> map3;
  map3 = map;
  EXPECT_EQ(map3, map);

  HashMap<std::string, int> map5(map3);
  HashMap<std::string, int> map7(std::move(map5));
  EXPECT_EQ(map7, map3);

  HashMap<std::string, int> map6(map3);
  HashMap<std::string, int> map8;
  map8 = std::move(map6);
  EXPECT_EQ(map8, map3);
}

TEST(MyDataStructTest, test_rbtree) {
  RbTree<int, char> t;
  int key;
  char val;
  for (int i = 0; i < 15; i++) {
    key = i;
    val = ('a' + i);  // NOLINT
    t.Insert(key, val);
  }
  key = 7;
  t.Remove(key);
  key = 10;
  t.Remove(key);
  EXPECT_EQ(t.GetSize(), 12);
}
