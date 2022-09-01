//
// Created by Lee on 2022/9/1.
//
#include <stdio.h>
#include <cstring>

#include "MyString.h"

MyString::MyString(const char* str) {//空实参初始化的时候需要将s_data指向一个空字符。用一个字符数组初始化时，复制该数组。
    if (str==nullptr) {
        s_data = new char[1];
        *s_data = '\0';
    } else {
        int len = strlen(str);
        s_data = new char[len+1];
        strcpy(s_data,str);
    }
}

MyString::MyString(const MyString& other) {//拷贝构造函数
    int len = strlen(other.s_data);
    s_data = new char[len+1];
    strcpy(s_data, other.s_data);
}

MyString& MyString::operator=(const MyString& other) {
    if (this == &other) return *this; //防止自赋值
    delete[] s_data;//删除原来string s_data的内存空间
    int len =strlen(other.s_data); //求右侧串长
    s_data = new char[len+1];//给左侧申请新的空间
    strcpy(s_data, other.s_data);//赋值
    return *this;//返回
}

int MyString::length() {
    return strlen(s_data);
}

char *MyString::get() {
    return s_data;
}

MyString::~MyString() {
    delete[] s_data;
}