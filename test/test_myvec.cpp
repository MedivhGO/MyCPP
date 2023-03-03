////
//// Created by Lee on 2023/2/6.
////
//
//#include <gtest/gtest.h>
//
//#include "MyVector.h"
//
//class DestructoType {
// public:
//  static size_t destruction_count;
//
//  DestructoType() = default;
//  ~DestructoType() {
//    destruction_count++;
//    destroyed = true;
//  }
//
//  bool destroyed = false;
//};
//size_t DestructoType::destruction_count = 0;
//
//struct ThreeIntType {
//  int a, b, c;
//};
//
//struct PaddedType {
//  char a;
//  double b;
//};
//
//class Pigeon;
//class PigeonManager {
// public:
//  PigeonManager() = default;
//  static void RegisterPigeon(Pigeon *p) { mPigeons.insert(p); }
//  static void DeregisterPigeon(Pigeon *p) { mPigeons.erase(p); }
//  static bool IsRegistered(Pigeon *p) { return mPigeons.find(p) != mPigeons.end(); }
//  static std::set<Pigeon *> mPigeons;
//};
//std::set<Pigeon *> PigeonManager::mPigeons;
//
//// 定义一个 Pigeon 对象就向 PigeonManager 中记录一下
//class Pigeon {
// public:
//  Pigeon() { PigeonManager::RegisterPigeon(this); }
//  Pigeon(const Pigeon &other) { PigeonManager::RegisterPigeon(this); }
//  ~Pigeon() { PigeonManager::DeregisterPigeon(this); }
//};
//
//// Fixtures
//class MyVecTest : public ::testing::Test {
// protected:
//  virtual void SetUp() override { DestructoType::destruction_count = 0; }
//  virtual void TearDown() override { PigeonManager::mPigeons.clear(); }
//
//  MyVector<int> v;
//  MyVector<char> vc;
//  MyVector<float> vf;
//  MyVector<double> vd;
//  MyVector<ThreeIntType> v3i;
//  MyVector<PaddedType> vp;
//  MyVector<Pigeon> vpigeons;
//};
//
//TEST_F(MyVecTest, Init) {
//  EXPECT_EQ(v.Size(), 0);
//  EXPECT_EQ(v.Capacity(), 0);
//}
//
//TEST_F(MyVecTest, PushBack) {
//  static const size_t insertions = 0xFFFF;
//
//  for (size_t i = 0; i < insertions; ++i) {
//    v.PushBack(i);
//    // test that the size is correctly incrementing
//    EXPECT_EQ(v.Size(), i + 1);
//    // test that capacity is doubling when it should
//    EXPECT_EQ(std::log2(v.Capacity()), std::ceil(std::log2(i + 1)));
//  }
//
//  for (size_t i = 0; i < insertions; ++i) {
//    // test that the values are in the correct positions
//    EXPECT_EQ(v[i], i);
//  }
//}
//
//TEST_F(MyVecTest, Insert) {
//  MyVector<int> expected({-1, 0, 1, 2});
//
//  for (size_t i = 0; i < 3; ++i) {
//    v.PushBack(i);
//  }
//
//  v.Insert(-1, 0);
//
//  EXPECT_TRUE(v.Size() == 4);
//  EXPECT_TRUE(v == expected);
//}
//
//TEST_F(MyVecTest, Remove) {
//  MyVector<int> expected({0, 1, 3});
//
//  for (size_t i = 0; i < 4; ++i) {
//    v.PushBack(i);
//  }
//
//  v.Remove(2);
//
//  EXPECT_TRUE(v.Size() == 3);
//  EXPECT_TRUE(v == expected);
//}
//
//TEST_F(MyVecTest, Clear) {
//  MyVector<DestructoType> destructo;
//
//  const size_t expected = 100;
//  destructo.Reserve(expected);
//  for (size_t i = 0; i < expected; ++i) {
//    destructo.PushBack(DestructoType());
//  }
//
//  for (size_t i = 0; i < expected; ++i) {
//    destructo.EmplaceBack(DestructoType());
//  }
//
//  // test that we're actually calling the dtors
//  destructo.Clear();
//
//  // *2 because the tmp gets destroyed as well, todo will this still happen if I make a move cons?
//  EXPECT_EQ(DestructoType::destruction_count, expected * 2);
//}
//
//TEST_F(MyVecTest, Grow) {
//  static const uint16_t growths = 16;
//
//  for (size_t i = 0; i < growths; ++i) {
//    v.Grow();
//    EXPECT_EQ(v.Capacity(), std::pow(2, i));
//  }
//
//  EXPECT_EQ(v.Size(), 0);
//}
//
//TEST_F(MyVecTest, Alignment) {
//  size_t required_alignment;
//  size_t misalignment_mask;
//  size_t misalignment;
//
//  // alignof 值是结构中的最大元素的对齐需求。
//  vc.PushBack('a');
//  required_alignment = alignof(char);
//  EXPECT_EQ(required_alignment, 1);
//  misalignment_mask = required_alignment - 1;
//  misalignment = reinterpret_cast<uintptr_t>(&vc[0]) & misalignment_mask;
//  EXPECT_EQ(misalignment, 0);
//
//  vf.PushBack(1.0f);
//  required_alignment = alignof(float);
//  EXPECT_EQ(required_alignment, 4);
//  misalignment_mask = required_alignment - 1;
//  misalignment = reinterpret_cast<uintptr_t>(&vf[0]) & misalignment_mask;
//  EXPECT_EQ(misalignment, 0);
//
//  vd.PushBack(1.0);
//  required_alignment = alignof(double);
//  EXPECT_EQ(required_alignment, 8);
//  misalignment_mask = required_alignment - 1;
//  misalignment = reinterpret_cast<uintptr_t>(&vd[0]) & misalignment_mask;
//  EXPECT_EQ(misalignment, 0);
//
//  v3i.PushBack({0, 1, 2});
//  required_alignment = alignof(ThreeIntType);
//  EXPECT_EQ(required_alignment, 4);
//  misalignment_mask = required_alignment - 1;
//  misalignment = reinterpret_cast<uintptr_t>(&v3i[0]) & misalignment_mask;
//  EXPECT_EQ(misalignment, 0);
//
//  vp.PushBack({'a', 1.0});
//  required_alignment = alignof(PaddedType);
//  EXPECT_EQ(required_alignment, 8);
//  misalignment_mask = required_alignment - 1;
//  misalignment = reinterpret_cast<uintptr_t>(&vp[0]) & misalignment_mask;
//  EXPECT_EQ(misalignment, 0);
//}
//
//TEST_F(MyVecTest, CopyConstructionAndDestruction) {
//  static const size_t insertions = 16;
//  for (size_t i = 0; i < insertions; ++i) {
//    vpigeons.PushBack(Pigeon());
//  }
//
//  // If the size doesn't equal the number of insertions that means
//  // we're not cleaning up destroyed pigeons correctly
//  EXPECT_EQ(PigeonManager::mPigeons.size(), insertions);
//
//  Pigeon *firstPigeyAddr = &vpigeons[0];
//
//  EXPECT_TRUE(PigeonManager::IsRegistered(firstPigeyAddr));
//  vpigeons.Grow();  // should invalidate all pigeon pointers
//  EXPECT_FALSE(PigeonManager::IsRegistered(firstPigeyAddr));
//}
//
//TEST_F(MyVecTest, DestroyElements) {
//  MyVector<DestructoType> destructo;
//  static const size_t insertions = 0x10;
//  for (size_t i = 0; i < insertions; ++i) {
//    destructo.PushBack(DestructoType());
//  }
//
//  // destroy the first half of the vector
//  destructo.DestroyElements(0, insertions >> 1);
//
//  for (size_t i = 0; i < insertions >> 1; ++i) {
//    EXPECT_TRUE(destructo[i].destroyed);
//  }
//  for (size_t i = (insertions >> 1) + 1; i < destructo.Size(); ++i) {
//    EXPECT_FALSE(destructo[i].destroyed);
//  }
//}
//
//TEST_F(MyVecTest, Equality) {
//  MyVector<int> otherGuy;
//  for (size_t i = 0; i < 0xFF; ++i) {
//    v.PushBack(i);
//    otherGuy.PushBack(i);
//  }
//
//  EXPECT_EQ(v, otherGuy);
//}