//
// Created by Lee on 2022/9/7.
//

#include <gtest/gtest.h>

TEST(MyCppFeatureTest, test1) {
    // Structured Bindings for C++ 17
    auto [x, y] = std::make_pair(1, 2);
    EXPECT_EQ(x, 1);
    EXPECT_EQ(y, 2);

    auto [x1, y1] = std::pair{1, 20};
    EXPECT_EQ(x1, 1);
    EXPECT_EQ(y1, 20);
}

TEST(MyCppFeatureTest, test2) {
    // <=> for C++ 20
    struct IntWrapper {
        int a;
        IntWrapper(int data) : a(data) {}
        auto operator<=>(const IntWrapper& b) const->auto { // 尾置返回类型
            [[likely]] if (a < b.a) { // likely
                return -1;
            } else if (a == b.a) {
                return 0;
            } else {
                return -1;
            }
        }
    };
    EXPECT_TRUE(IntWrapper(1) < IntWrapper(2));
}

template<typename T>
T  adder(T value) {
    return value;
}

template<typename T, typename... Args>
T adder(T first, Args... args) {
    return first + adder(args...);
}

TEST(MyCppFeatureTest, test3) {
    // variadic template for C++ 11
    EXPECT_EQ(adder(1,2,3), 6);
}
