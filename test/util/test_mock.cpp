#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockClass {
 public:
  MOCK_METHOD(int, GetNumber, ());
};

TEST(MockTest, TestMethodCall) {
  MockClass mock;

  // 设置期望调用
  EXPECT_CALL(mock, GetNumber())
      .WillOnce(::testing::Return(10))  // 第一次调用时返回10
      .WillOnce(::testing::Return(20))  // 第二次调用时返回20
      .WillRepeatedly(::testing::Return(30));  // 之后的调用都返回30

  // 测试代码
  EXPECT_EQ(mock.GetNumber(), 10);  // 验证第一次调用
  EXPECT_EQ(mock.GetNumber(), 20);  // 验证第二次调用
  EXPECT_EQ(mock.GetNumber(), 30);  // 验证之后的调用
}