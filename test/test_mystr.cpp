#include <gtest/gtest.h>
#include <string>

#include "MyString.h"
#include "MySingleton.h"
#include "MyProfile.h"

TEST(MyStringTest, test1) {
  MyString a;
  EXPECT_EQ(0, a.length());
}

TEST(MyStringTest, test2) {
  MyString a("hello");
  EXPECT_EQ(5, a.length());
}

TEST(MyStringTest, test3) {
  MyString a("123");
  MyString b(a);
  EXPECT_STREQ("123", b.get());
}

TEST(MyStringTest, test4) {
  MyString a("123");
  MyString b(a);
  MyString c;
  c = a + b;
  EXPECT_STREQ("123123", c.get());
}

TEST(MyStringTest, test5) {
  MyString a("123");
  MyString b;
  MyString c;
  c = a + b;
  EXPECT_STREQ("123", c.get());
}

TEST(MyStringTest, test6) {
  MyString a(std::string("Hello"));
  EXPECT_STREQ("Hello", a.get());
}

TEST(MyStringTest, test7) {
  MyString a("123456789");
  std::stringstream ss;
  ss << a;
  EXPECT_EQ(ss.str(), a.get());
}

TEST(MyStringTest, test8) {
  MyString a("123");
  std::stringstream s;
  s << "123";
  MyString c;
  s >> c;
  EXPECT_STREQ("123", c.get());
}

TEST(MyStringTest, test9) {
  MyString a("123");
  MyString b("123");
  EXPECT_EQ(true, a == b);
}

TEST(MyStringTest, test10) {
  MyString a("123");
  MyString b("1234");
  EXPECT_EQ(false, a == b);
}

TEST(MyStringTest, test11) {
  MyString a("123");
  MyString b("1234");
  EXPECT_EQ(true, a != b);
}

TEST(MyStringTest, test12) {
  MyString a("123");
  MyString b("1234");
  EXPECT_EQ(true, b >= a);
}

TEST(MyStringTest, test13) {
  MyString a("45");
  MyString b("1234");
  EXPECT_EQ(true, b <= a);
}

TEST(MyStringTest, test14) {
  MyString a("abc");
  MyString b;
  b = !a;
  EXPECT_STREQ("ABC", b.get());
}

TEST(MyStringTest, test15) {
  MyString a("abc123");
  MyString b;
  b = !a;
  EXPECT_STREQ("ABC123", b.get());
}

TEST(MyStringTest, test16) {
  MyString a("45");
  char b = '5';
  EXPECT_EQ(b, a[1]);
}

TEST(MyStringTest, test17) {
  MyString a("454545");
  EXPECT_EQ(0, a("45"));
}

TEST(MyStringTest, test18) {
  MyString a("454545");
  EXPECT_EQ(-1, a("33"));
}

TEST(MyStringTest, test19) {
    MyString a("raw");
    MyString b(std::move(a));
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_STREQ("raw", b.get());
}

TEST(MyStringTest, test20) {
    MyString a("raw");
    MyString b = std::move(a);
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_STREQ("raw", b.get());
}

TEST(MyStringTest, test21) {
    MyString a("123");
    MyString b("4567");
    swap(a,b);
    EXPECT_STREQ("4567", a.get());
    EXPECT_STREQ("123", b.get());
}

TEST(MyStringTest, test22) {
    MyString a;
    std::string b;
    for (int i = 0; i < 1000; ++i) {
        a += "1";
        b += "1";
    }
    EXPECT_STREQ(a.get(), b.c_str());
}

TEST(MyStringTest, test23) {
    auto std_add = []() {
        std::string b;
        for (int i = 0; i < 1000; ++i) {
            b += "1";
        }
    };
    auto my_add = []() {
        MyString a;
        for (int i = 0; i < 1000; ++i) {
            a += "1";
        }
    };
    EXPECT_GT(measure(my_add).count(), measure(std_add).count());
}