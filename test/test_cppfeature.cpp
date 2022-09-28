//
// Created by Lee on 2022/9/7.
//

#include <gtest/gtest.h>

#include <regex>
#include <string>
#include <future>

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

int foo(int para) {
    return para;
}

TEST(MyCppFeatureTest, test4) {
    // std::function
    std::function<int(int)> func1 = foo;

    int important = 10;

    std::function<int(int)> func2 = [&](int value) -> int {
        return 1 + value + important;
    };

    EXPECT_EQ(func1(10), 10);
    EXPECT_EQ(func2(10), 21);
}

TEST(MyCppFeatureTest, test5) {
    std::string true_fnames[] = {"foo.txt"};
    std::string false_fnames[] = {"test","a0.txt","AAA.txt"};
    std::regex txt_regex("[a-z]+\\.txt");

    for (const auto& x : true_fnames) {
        EXPECT_TRUE(std::regex_match(x, txt_regex));
    }

    for (const auto& x : false_fnames) {
        EXPECT_FALSE(std::regex_match(x, txt_regex));
    }

    // 使用 std::smatch 可以方便的对匹配的结果进行获取
    std::smatch base_match;
    std::regex base_regex("([a-z]+)\\.txt");
    for (const auto& x : true_fnames) {
        if (std::regex_match(x, base_match, base_regex)) {
            // std::smatch 的第一个元素匹配整个字符串
            // std::smatch 的第二个元素匹配了第一个括号表达式
            if (base_match.size() == 2) {
                EXPECT_EQ(x, base_match[0]);
                EXPECT_EQ("foo", base_match[1]);
            }
        }
    }
}

TEST(MyCppFeatureTest, test6) {
    long long int i;
    EXPECT_EQ(sizeof(i), 8);
}

TEST(MyCppFeatureTest, test7) {
    std::packaged_task<int()> task([](){return 7;});
    std::future<int> result = task.get_future(); // 在一个线程中执行 task
    std::thread(std::move(task)).detach();
    std::cout << "waiting...";
    result.wait(); // 在此设置屏障，阻塞到期物的完成
    std::cout << "done!" << std:: endl << "future result is "
              << result.get() << std::endl;
}

TEST(MyCppFeatureTest, test8) {
    // runtime type identification, RTTI
    // typeid 返回表达式的类型
    int a = 10;
    double b = 10;
    EXPECT_NE(typeid(a), typeid(b));
    EXPECT_NE(typeid(a), typeid(double));
    EXPECT_STREQ("d", typeid(double).name());
}

class A{};
class A_int{int a;};
class A_char{char a;};

TEST(MyCppFeatureTest, test9) {
    EXPECT_EQ(sizeof(A),1);
    EXPECT_EQ(sizeof(A_int), 4);
    EXPECT_EQ(sizeof(A_char), 1);
}

TEST(MyCppFeatureTest, test10) {
    int a = 10;
    decltype(a) b = 20;
    EXPECT_STREQ("i", typeid(b).name());
}