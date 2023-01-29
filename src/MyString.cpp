//
// Created by Lee on 2022/9/1.
//
#include <cstring>
#include <vector>

#include "MyLog.h"
#include "MyString.h"

// 空实参初始化的时候需要将s_data_指向一个空字符。用一个字符数组初始化时，复制该数组。
MyString::MyString(const char *str) {
  if (str == nullptr) {
    s_data_ = new char[1];
    *s_data_ = '\0';
  } else {
    size_t len = strlen(str);
    s_data_ = new char[len + 1];
    strcpy(s_data_, str);
  }
  LOG_DEBUG("%s %s", "Construct A Variable:", s_data_);
}

MyString::MyString(const std::string &str) {
  size_t len = str.length();
  s_data_ = new char[len + 1];
  strcpy(s_data_, str.c_str());
  LOG_DEBUG("%s %s", "Construct A Variable:", s_data_);
}

MyString::MyString(const MyString &other) {  // 拷贝构造函数
  if (this == &other) {
    return;
  }
  size_t len = strlen(other.s_data_);
  s_data_ = new char[len + 1];
  strcpy(s_data_, other.s_data_);
}

MyString::MyString(MyString &&other) noexcept {
  if (this == &other) {
    return;
  }
  // 拷贝指针
  s_data_ = other.s_data_;
  // 参数指针置空
  other.s_data_ = nullptr;
}

auto MyString::operator=(const MyString &other) -> MyString & {
  if (this == &other) {
    return *this;  // 防止自赋值
  }
  delete[] s_data_;                    // 删除原来string s_data_的内存空间
  size_t len = strlen(other.s_data_);  // 求右侧串长
  s_data_ = new char[len + 1];         // 给左侧申请新的空间
  strcpy(s_data_, other.s_data_);      // 赋值
  return *this;                        // 返回
}

auto MyString::operator=(MyString &&other) noexcept -> MyString & {
  if (this == &other) {
    return *this;
  }
  // 销毁原有数据
  delete[] s_data_;
  // 拷贝指针
  s_data_ = other.s_data_;
  // 参数指针置空
  other.s_data_ = nullptr;
  return *this;
}

auto MyString::Length() const -> size_t { return strlen(s_data_); }

auto MyString::Get() const -> const char * { return s_data_; }

MyString::~MyString() {
  LOG_DEBUG("%s %s", "Release A Variable: ", s_data_);
  delete[] s_data_;
  s_data_ = nullptr;
}

// friend operator functions
auto operator+(const MyString &lhs, const MyString &rhs) -> MyString {
  size_t len1 = lhs.Length();
  size_t len2 = rhs.Length();
  size_t total_len = len1 + len2;
  char tmp[total_len + 1];
  char *next = strcpy(tmp, lhs.Get());
  strcat(next, rhs.Get());
  return MyString(tmp);
}

auto operator<<(std::ostream &os, const MyString &str) -> std::ostream & {
  os << str.Get();
  return os;
}

auto operator>>(std::istream &is, MyString &str) -> std::istream & {
  std::string tmp;
  is >> tmp;  // 拷贝一次
  // str = MyString(tmp); // 拷贝两次, 一共拷贝了3次
  str = std::move(MyString(tmp));  // 拷贝1次, 一共2次
  return is;
}

auto operator==(const MyString &lhs, const MyString &rhs) -> bool { return strcmp(lhs.Get(), rhs.Get()) == 0; }

auto operator!=(const MyString &lhs, const MyString &rhs) -> bool { return !(lhs == rhs); }

auto operator>=(const MyString &lhs, const MyString &rhs) -> bool { return (strcmp(lhs.Get(), rhs.Get()) >= 0); }
auto operator<=(const MyString &lhs, const MyString &rhs) -> bool { return (strcmp(lhs.Get(), rhs.Get()) <= 0); }

void Swap(MyString &lhs, MyString &rhs) {
  char *t = lhs.s_data_;
  lhs.s_data_ = rhs.s_data_;
  rhs.s_data_ = t;
}

// member operator function
auto MyString::operator!() -> const MyString & {
  size_t len = Length();
  for (int i = 0; i < len; i++) {
    if (s_data_[i] >= 'a' && s_data_[i] <= 'z') {
      s_data_[i] -= 32;  // +32转换为大写
    }
  }
  return *this;
}

auto MyString::operator[](int idx) -> char & { return s_data_[idx]; }

auto MyString::operator[](int idx) const -> const char & { return s_data_[idx]; }

auto MyString::operator()(const MyString &substr) const -> int {
  // KMP
  const MyString &ss = *this;
  const MyString &pp = substr;
  size_t n = ss.Length();
  size_t m = pp.Length();
  if (m == 0) {
    return 0;
  }
  // 设置哨兵
  MyString s = MyString(" ") + ss;
  MyString p = MyString(" ") + pp;
  std::vector<int> next(m + 1);
  // 预处理next数组
  for (int i = 2, j = 0; i <= m; i++) {
    while ((j != 0) and p[i] != p[j + 1]) {
      j = next[j];
    }
    if (p[i] == p[j + 1]) {
      j++;
    }
    next[i] = j;
  }
  // 匹配过程
  for (int i = 1, j = 0; i <= n; i++) {
    while ((j != 0) and s[i] != p[j + 1]) {
      j = next[j];
    }
    if (s[i] == p[j + 1]) {
      j++;
    }
    if (j == m) {
      return i - m;
    }
  }
  return -1;
}

auto MyString::operator()(const char *substr) const -> int { return MyString::operator()(MyString(substr)); }

auto MyString::operator+=(const char *rhs) -> MyString & {
  size_t len = strlen(rhs);
  size_t old_len = strlen(s_data_);
  char *new_data = new char[len + old_len + 1];
  char *next = strcpy(new_data, s_data_);
  strcat(next, rhs);
  // 销毁原有数据
  delete[] s_data_;
  // 拷贝指针
  s_data_ = new_data;
  return *this;
}