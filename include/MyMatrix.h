//  Created by Lee on 2017/3/25.
//  Copyright © 2017年 ls. All rights reserved.
//  Modify by Lee on 2022/9/2.

#ifndef MYCPPIMPLEMENT_MYMATRIX_H
#define MYCPPIMPLEMENT_MYMATRIX_H

#include <vector>

using std::vector;

template<typename T>
class MyMatrix {
public:
    MyMatrix();

    MyMatrix(unsigned row, unsigned col); // 指定行列构造

    MyMatrix(vector<T> &r); // 由vector作为行向量构造矩阵

    ~MyMatrix();

    const vector<T> &operator[](unsigned row) const;

    vector<T> &operator[](unsigned row);

    unsigned getRow() const;

    unsigned getCol() const;

    MyMatrix<T> getRowMyMatrix(unsigned r) const; //取第r行向量

    MyMatrix<T> getColMyMatrix(unsigned c) const; //取第c列向量

    void showMatrix();

    friend MyMatrix<T> operator+(const MyMatrix<T> &A, const MyMatrix<T> &B); //矩阵加法

    friend MyMatrix<T> operator-(const MyMatrix<T> &A, const MyMatrix<T> &B); //矩阵减法

    friend MyMatrix<T> operator*(const MyMatrix<T> &A, const MyMatrix<T> &B);  // 矩阵乘法

    friend MyMatrix<T> operator*(const T &e, const MyMatrix<T> &A);            // 矩阵数乘

    friend MyMatrix<T> operator/(const MyMatrix<T> &A, const T &e);            // 矩阵除以一个数

    bool isSquare() const;  //判断是否为方阵

private:
    void setDim(unsigned row, unsigned col);

private:
    vector<vector<T>> elem;
};

#endif //MYCPPIMPLEMENT_MYMATRIX_H
