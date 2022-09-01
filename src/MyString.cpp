//
// Created by Lee on 2022/9/1.
//
#include <cstdio>
#include <cstring>
#include <vector>

#include "MyString.h"

using std::vector;

MyString::MyString(const char* str) { // 空实参初始化的时候需要将s_data指向一个空字符。用一个字符数组初始化时，复制该数组。
    if (str == nullptr) {
        s_data = new char[1];
        *s_data = '\0';
    } else {
        int len = strlen(str);
        s_data = new char[len+1];
        strcpy(s_data,str);
    }
}

MyString::MyString(const std::string& str) {
    int len = str.length();
    s_data = new char[len+1];
    strcpy(s_data, str.c_str());
}

MyString::MyString(const MyString& other) { // 拷贝构造函数
    if (this == &other) {
        return;
    }
    int len = strlen(other.s_data);
    s_data = new char[len+1];
    strcpy(s_data, other.s_data);
}

MyString::MyString(MyString &&other) {
    if (this == &other) {
        return;
    }
    // 拷贝指针
    s_data = other.s_data;
    // 参数针置空
    other.s_data = nullptr;
}

MyString& MyString::operator=(const MyString& other) {
    if (this == &other) {
        return *this; // 防止自赋值
    }
    delete[] s_data; // 删除原来string s_data的内存空间
    int len = strlen(other.s_data); // 求右侧串长
    s_data = new char[len+1]; // 给左侧申请新的空间
    strcpy(s_data, other.s_data); // 赋值
    return *this;//返回
}

MyString& MyString::operator=(MyString &&other) {
    if (this == &other) {
        return *this;
    }
    // 销毁原有数据
    delete[] s_data;
    // 拷贝指针
    s_data = other.s_data;
    // 参数指针置空
    other.s_data = nullptr;
    return *this;
}

int MyString::length() const{
    return strlen(s_data);
}

const char *MyString::get() const {
    return s_data;
}

MyString::~MyString() {
    delete[] s_data;
}

// friend operator functions
MyString operator+ (const MyString& lhs, const MyString& rhs) {
    int len1 = lhs.length();
    int len2 = rhs.length();
    int total_len = len1 + len2;
    char tmp[total_len+1];
    char* next = strcpy(tmp, lhs.get());
    strcat(next, rhs.get());
    return MyString(tmp);
}

std::ostream& operator<< (std::ostream& os, const MyString& str) {
    os << str.get();
    return os;
}

std::istream& operator>> (std::istream& is, MyString& str) {
    std::string tmp;
    is >> tmp; // 拷贝一次
    //str = MyString(tmp); // 拷贝两次, 一共拷贝了3次
    str = std::move(MyString(tmp)); // 拷贝1次, 一共2次
    return is;
}

bool operator==(const MyString& lhs, const MyString& rhs) {
    return strcmp(lhs.get(), rhs.get()) == 0;
}

bool operator!=(const MyString& lhs, const MyString& rhs) {
    return !(lhs == rhs);
}

bool operator>=(const MyString& lhs, const MyString& rhs) {
    return (strcmp(lhs.get(), rhs.get()) >= 0);
}
bool operator<=(const MyString& lhs, const MyString& rhs) {
    return (strcmp(lhs.get(), rhs.get()) <= 0);
}

// member operator function
const MyString& MyString::operator!() {
    int len = length();
    for(int i=0;i<len;i++){
        if(s_data[i]>='a'&& s_data[i] <= 'z' ){
            s_data[i]-=32; // +32转换为大写
        }
    }
    return *this;
}

char& MyString::operator[](int idx) {
    return s_data[idx];
}

const char& MyString::operator[](int idx) const {
    return s_data[idx];
}

int MyString::operator()(const MyString& substr) const {
    // KMP
    const MyString& ss = *this;
    const MyString& pp = substr;

    int n = ss.length(), m = pp.length();
    if(m == 0) return 0;
    //设置哨兵
    MyString s = " " + ss;
    MyString p = " " + pp;
    vector<int> next(m + 1);
    //预处理next数组
    for(int i = 2, j = 0; i <= m; i++){
        while(j and p[i] != p[j + 1]) j = next[j];
        if(p[i] == p[j + 1]) j++;
        next[i] = j;
    }
    //匹配过程
    for(int i = 1, j = 0; i <= n; i++){
        while(j and s[i] != p[j + 1]) j = next[j];
        if(s[i] == p[j + 1]) j++;
        if(j == m) return i - m;
    }
    return -1;
}




