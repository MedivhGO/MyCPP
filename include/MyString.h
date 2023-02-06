//
// Created by Lee on 2022/9/1.
//

#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>

class MyString {
 public:
  explicit MyString(const char *str = nullptr);  // 一个参数的构造函数，当没有提供初始值时就采用默认的参数
  explicit MyString(const std::string &str);                // 使用std::string构造MyString
  MyString(const MyString &other);                          // 拷贝构造函数
  MyString(MyString &&other) noexcept;                      // 移动构造函数
  auto operator=(const MyString &other) -> MyString &;      // 赋值运算符
  auto operator=(MyString &&other) noexcept -> MyString &;  // 移动赋值运算符
  [[nodiscard]] auto Length() const -> size_t;

  [[nodiscard]] auto Get() const -> const char *;

  virtual ~MyString();  // 虚析构函数，释放s_data指向的指针

  friend auto operator+(const MyString &lhs, const MyString &rhs) -> MyString;

  friend auto operator<<(std::ostream &os, const MyString &str) -> std::ostream &;

  friend auto operator>>(std::istream &is, MyString &str) -> std::istream &;

  friend auto operator<=>(const MyString &lhs, const MyString& rhs) -> std::partial_ordering;

  friend auto operator==(const MyString &lhs, const MyString& rhs) -> bool;

  friend void Swap(MyString &lhs, MyString &rhs);

  auto operator!() -> const MyString &;

  auto operator[](int idx) -> char &;

  auto operator[](int idx) const -> const char &;

  auto operator()(const MyString &substr) const -> int;

  auto operator()(const char *substr) const -> int;

  auto operator+=(const char *rhs) -> MyString &;

 private:
  char *s_data_;
};

#endif  // MYSTRING_H
