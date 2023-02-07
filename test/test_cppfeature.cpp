//
// Created by Lee on 2022/9/7.
//

#include <gtest/gtest.h>

#include <future>
#include <numbers>
#include <regex>
#include <string>
#include <type_traits>
#include <unordered_set>

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
    int a_;
    explicit IntWrapper(int data) : a_(data) {}
    auto operator<=>(const IntWrapper &b) const -> auto{  // 尾置返回类型
      [[likely]] if (a_ < b.a_) {                         // likely C++ 20
        return -1;
      }
      if (a_ == b.a_) {
        return 0;
      }
      return 1;
    }
  };
  EXPECT_TRUE(IntWrapper(1) < IntWrapper(2));
}

template <typename T>
auto Adder(T value) -> T {
  return value;
}

template <typename T, typename... Args>
auto Adder(T first, Args... args) -> T {
  return first + Adder(args...);
}

TEST(MyCppFeatureTest, test3) {
  // variadic template for C++ 11
  EXPECT_EQ(Adder(1, 2, 3), 6);
}

auto Foo(int para) -> int { return para; }

TEST(MyCppFeatureTest, test4) {
  // std::function
  std::function<int(int)> func1 = Foo;

  int important = 10;

  std::function<int(int)> func2 = [&](int value) -> int { return 1 + value + important; };

  EXPECT_EQ(func1(10), 10);
  EXPECT_EQ(func2(10), 21);
}

TEST(MyCppFeatureTest, test5) {
  std::string true_fnames[] = {"foo.txt"};
  std::string false_fnames[] = {"test", "a0.txt", "AAA.txt"};
  std::regex txt_regex("[a-z]+\\.txt");

  for (const auto &x : true_fnames) {
    EXPECT_TRUE(std::regex_match(x, txt_regex));
  }

  for (const auto &x : false_fnames) {
    EXPECT_FALSE(std::regex_match(x, txt_regex));
  }

  // 使用 std::smatch 可以方便的对匹配的结果进行获取
  std::smatch base_match;
  std::regex base_regex("([a-z]+)\\.txt");
  for (const auto &x : true_fnames) {
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
  int64_t i;
  EXPECT_EQ(sizeof(i), 8);
}

TEST(MyCppFeatureTest, test7) {
  std::packaged_task<int()> task([]() { return 7; });
  std::future<int> result = task.get_future();  // 在一个线程中执行 task
  std::thread(std::move(task)).detach();
  std::cout << "waiting...";
  result.wait();  // 在此设置屏障，阻塞到期物的完成
  std::cout << "done!" << std::endl << "future result is " << result.get() << std::endl;
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

class A {};
class AInt {
  int a_;
};
class AChar {
  char a_;
};

TEST(MyCppFeatureTest, test9) {
  EXPECT_EQ(sizeof(A), 1);
  EXPECT_EQ(sizeof(AInt), 4);
  EXPECT_EQ(sizeof(AChar), 1);
}

TEST(MyCppFeatureTest, test10) {
  int a = 10;
  decltype(a) b = 20;
  EXPECT_STREQ("i", typeid(b).name());
}

// https://unique-ptr.com/archives/68
template <typename Iterator>
auto MyDistance(Iterator begin, Iterator end) {
  using Traits = std::iterator_traits<Iterator>;
  if constexpr (std::is_base_of_v<std::random_access_iterator_tag, typename Traits::iterator_category>) {
    return end - begin;
  } else {
    auto result = typename Traits::difference_type();
    for (auto it = begin; it != end; ++it) {
      ++result;
    }
    return result;
  }
}

TEST(MyCppFeatureTest, test11) {
  // C++ 17 if constexpr
  std::vector<int> i_vec{1, 2, 3, 4, 5};
  int64_t res = MyDistance(i_vec.begin(), i_vec.end());
  EXPECT_EQ(res, 5);
}

const bool SUPPRESS_EXPECT = true;

struct TESTNOEXPECT {
  int n_;
  TESTNOEXPECT() { n_ = 0; }

  TESTNOEXPECT(const TESTNOEXPECT &a) noexcept(SUPPRESS_EXPECT) { n_ = a.n_; }

  TESTNOEXPECT(TESTNOEXPECT &&) noexcept = delete;
};

TEST(MyCppFeatureTest, test12) {
  // keyword noexpect
  // 移动构造函数标记为 noexpect
  // 因为在资源的移动过程中如果抛出了异常，那么那些正在被处理的原始对象数据可能因为异常而丢失
  EXPECT_EQ(std::is_move_constructible<TESTNOEXPECT>::value, false);
  EXPECT_EQ(std::is_trivially_move_constructible<TESTNOEXPECT>::value, false);
  EXPECT_EQ(std::is_nothrow_move_constructible<TESTNOEXPECT>::value, false);
  EXPECT_EQ(std::is_copy_constructible<TESTNOEXPECT>::value, true);
}

TEST(MyCppFeatureTest, test13) {
  std::vector<int> i_vec{1, 1, 1};
  for (const auto &x : i_vec) {
    EXPECT_EQ(x, 1);
  }
}

TEST(MyCppFeatureTest, test14) {
  // ranges
  // C++ 20
  std::vector a = {3, 2, 1};
  std::vector b = {3, 2, 1};
  std::ranges::sort(a.begin(), a.end());
  std::ranges::sort(b);

  EXPECT_EQ(a, b);
  EXPECT_EQ(std::ranges::max(a), 3);
  EXPECT_EQ(std::ranges::min(a), 1);
}

TEST(MyCppFeatureTest, test15) {
  // C++ 20
  std::string s = "bocchi the rock";
  EXPECT_TRUE(s.starts_with("bocchi"));
}

TEST(MyCppFeatureTest, test16) {
  // C++ 17
  constexpr std::string_view haystack =
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed "
      "do eiusmod tempor incididunt ut labore et dolore magna aliqua";
  const std::string_view needle{"pisci"};
  auto it = std::search(haystack.begin(), haystack.end(), std::boyer_moore_searcher(needle.begin(), needle.end()));
  EXPECT_EQ(it - haystack.begin(), 43);
}

TEST(MyCppFeatureTest, test17) {
  // C++ 14
  const std::string_view needle{"pisci"};
  std::quoted(needle);  // 给字符串加上双引号
}

TEST(MyCppFeatureTest, test18) {
  // C++ 14 支持变量模板
  EXPECT_EQ(std::numbers::pi, std::numbers::pi_v<double>);
}

TEST(MyCppFeatureTest, test19) {
  std::map<int, int> map{{1, 10}, {2, 20}, {3, 30}};
  std::unordered_set<int> u_set{1, 2, 3};
  EXPECT_TRUE(map.contains(1));
  EXPECT_FALSE(u_set.contains(4));
}

TEST(MyCppFeatureTest, test20) {
  // C++ 17
  std::vector a{1, 2, 3, 4, 5};
  EXPECT_EQ(a.rbegin()[0], 5);
  EXPECT_EQ(rbegin(a)[1], 4);

  EXPECT_EQ(a.end()[-1], 5);
  EXPECT_EQ(end(a)[-2], 4);
}

TEST(MyCppFeatureTest, test21) {
  uint32_t a = 1;
  EXPECT_EQ(std::popcount(a), 1);
  EXPECT_TRUE(std::has_single_bit(a));
  EXPECT_EQ(std::countl_zero(a), 31);
}

TEST(MyCppFeatureTest, test23) {
  // C++ 17
  std::vector<int> a{1, 2, 3};
  for (int idx = 1; auto x : a) {  // 将初始化语句放入其中
    x += idx++;
  }

  for (int idx = 0; auto dx : {1, 1, 1, 1}) {
    EXPECT_EQ(1, idx + dx);
  }
}

constexpr auto Sort(auto arr) -> std::array<int, 3> {
  std::sort(arr.begin(), arr.end());
  return arr;
}

TEST(MyCppFeatureTest, test24) {
  // C++ 20
  constexpr auto ret = Sort(std::array{3, 2, 1});
  static_assert(std::is_sorted(ret.begin(), ret.end()));  // 编译期间检查是否已经排好序
}

struct [[deprecated]] DepClass {};

TEST(MyCppFeatureTest, test25) {
  // C++ 14
  // DepClass dc; compile warning
}

TEST(MyCppFeatureTest, test26) {
  // C++ 14
  int a = 0b0001'0011'1010;
  double b = 3.14'1234'1234'1234;
}

TEST(MyCppFeatureTest, test27) {
  // C++ 14
  struct A {};
  std::unique_ptr<A> ptr = std::make_unique<A>();
}

TEST(MyCppFeatureTest, test28) {
  // C++ 14
  // 将第二个参数交给第一个参数，返回值为第二个参数
  std::vector<int> v;
  auto p = std::exchange(v, {1, 2, 3, 4});
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(p.size(), 0);
}

TEST(MyCppFeatureTest, test30) {
  // C++ 17
  // string_view 是一个 char 数据的视图或者说引用，它并不拥有该数据是为了避免拷贝，
  // 因此使用 string_view 可以用来做性能优化
  // 由于 string_vew 并不拥有所引用的字符串，所以它也不会去关注被引用字符串的生命周期
  // 用户在使用的时候需要注意，不要将一个临时变量给一个 string_view
  // 那样会导致 string_view 引用的内容也失效

  // 使用场景
  // 字符串查找
  // 遍历字符串
  // 显示字符串
  constexpr auto s = "it is a test"sv; // 字面量后缀 sv

  std::string_view sv1("hello world");
  std::string s1(sv1);
  EXPECT_EQ(8, sizeof(sv1));
  EXPECT_EQ(28, sizeof(s1));
}
