//
// Created by Lee on 2022/9/1.
//

#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>

class MyString {
public:
    MyString(const char *str=nullptr);          // 一个参数的构造函数，当没有提供初始值时就采用默认的参数
    MyString(const std::string& str);           // 使用std::string构造MyString
    MyString(const MyString& other);            // 拷贝构造函数
    MyString(MyString&& other);                 // 移动构造函数
    MyString& operator=(const MyString& other); // 赋值运算符
    MyString& operator=(MyString&& other);      // 移动赋值运算符
    int length() const;
    const char* get() const;
    virtual ~MyString();                        // 虚析构函数，释放s_data指向的指针

    friend MyString operator+ (const MyString& lhs, const MyString& rhs);
    friend std::ostream& operator<< (std::ostream& os, const MyString& str);
    friend std::istream& operator>> (std::istream& is, MyString& str);
    friend bool operator==(const MyString& lhs, const MyString& rhs);
    friend bool operator!=(const MyString& lhs, const MyString& rhs);
    friend bool operator>=(const MyString& lhs, const MyString& rhs);
    friend bool operator<=(const MyString& lhs, const MyString& rhs);

    const MyString& operator!();
    char& operator[](int idx);
    const char& operator[](int idx) const;
    int operator()(const MyString& substr) const;

private:
    char* s_data;
};

#endif //MYSTRING_H
