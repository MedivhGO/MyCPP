//  Created by Lee on 2017/3/25.
//  Copyright © 2017年 ls. All rights reserved.
//  Modify by Lee on 2022/9/2.

#ifndef MYCPPIMPLEMENT_MYMATRIX_H
#define MYCPPIMPLEMENT_MYMATRIX_H

#include <vector>
#include <iostream>
#include <random>

using std::vector;
using std::cout;
using std::endl;

template<typename T>
class MyMatrix {
public:
    MyMatrix();

    MyMatrix(unsigned row, unsigned col);

    MyMatrix(int dimension);

    MyMatrix(vector<T> &r);

    MyMatrix(const std::initializer_list<T>& il);

    MyMatrix(const std::initializer_list<std::initializer_list<T>>& il);

    ~MyMatrix();

    const vector<T> &operator[](unsigned row) const;

    vector<T> &operator[](unsigned row);

    unsigned getRow() const;

    unsigned getCol() const;

    MyMatrix<T> getRowMyMatrix(unsigned r) const;

    MyMatrix<T> getColMyMatrix(unsigned c) const;

    void randomFillMatrix(int low, int hight);

    void showMatrix();

    void showDiagonal();

    bool isSquare() const;

    friend MyMatrix<T> operator+(const MyMatrix<T> &A, const MyMatrix<T> &B) {
        MyMatrix<T> C(A.getRow(), A.getCol());
        for (unsigned i = 0; i < C.getRow(); i++) {
            for (unsigned j = 0; j < C.getCol(); j++) {
                C[i][j] = A[i][j] + B[i][j];
            }
        }
        return C;
    }

    friend MyMatrix<T> operator-(const MyMatrix<T> &A, const MyMatrix<T> &B) {
        MyMatrix<T> C(A.getRow(), A.getCol());
        for (unsigned i = 0; i < C.getRow(); i++) {
            for (unsigned j = 0; j < C.getCol(); j++) {
                C[i][j] = A[i][j] - B[i][j];
            }
        }
        return C;
    }

    friend MyMatrix<T> operator*(const MyMatrix<T> &A, const MyMatrix<T> &B) {
        unsigned i, j, k;
        MyMatrix<T> C(A.getRow(), B.getCol());
        for (i = 0; i < A.getRow(); i++) {
            for (j = 0; j < B.getCol(); j++) {
                for (k = 0; k < A.getCol(); k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return C;
    }

    friend MyMatrix<T> operator*(const T &e, const MyMatrix<T> &A) {
        MyMatrix<T> C(A.getRow(), A.getCol());
        for (unsigned i = 0; i < C.getRow(); i++) {
            for (unsigned j = 0; j < C.getCol(); j++) {
                C[i][j] = A[i][j] * e;
            }
        }
        return C;
    }

    friend MyMatrix<T> operator/(const MyMatrix<T> &A, const T &e) {
        MyMatrix<T> C(A.getRow(), A.getCol());
        for (unsigned i = 0; i < C.getRow(); i++) {
            for (unsigned j = 0; j < C.getCol(); j++) {
                C[i][j] = A[i][j] / e;
            }
        }
        return C;
    }

    friend bool operator==(const MyMatrix<T> &A, const MyMatrix<T> &B) {
        if (A.getCol() != B.getCol() || A.getRow() != B.getRow()) {
            return false;
        }
        for (unsigned i = 0; i < A.getRow(); i++) {
            for (unsigned j = 0; j < A.getCol(); j++) {
                if (A[i][j] != B[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    friend bool operator!=(const MyMatrix<T> &A, const MyMatrix<T> &B) {
        return !(A == B);
    }

private:
    void setDim(unsigned row, unsigned col);

private:
    vector<vector<T>> m_elem;
    int m_dimension;
};

template<typename T>
MyMatrix<T>::MyMatrix() {
    setDim(1, 1);
}

template<typename T>
MyMatrix<T>::MyMatrix(unsigned row, unsigned col) {
    setDim(row, col);
}

template<typename T>
MyMatrix<T>::MyMatrix(int dimension) {
    this->m_dimension = dimension;
    this->m_elem.resize(dimension);
    for (int i = 0; i < dimension; ++i) {
        m_elem[i].resize(dimension);
    }
}

template<typename T>
MyMatrix<T>::MyMatrix(vector<T> &r) {
    setDim(r.size(), 1);
    m_elem[0] = r;
}

template<typename T>
MyMatrix<T>::MyMatrix(const std::initializer_list<T>& il) {
    setDim(il.size(), 1);
    m_elem[0] = il;
}

template<typename T>
MyMatrix<T>::MyMatrix(const std::initializer_list<std::initializer_list<T>>& il) {
    std::vector<std::initializer_list<T> > setVec = il;
    for (typename std::vector<std::initializer_list<T> >::iterator i = setVec.begin(); i != setVec.end(); i++) {
        m_elem.push_back(std::vector<T>(*i));
    }
}

template<typename T>
MyMatrix<T>::~MyMatrix() {
    for (unsigned i = 0; i < m_elem.size(); i++) {
        m_elem[i].clear();
        m_elem[i].shrink_to_fit();
    }
    m_elem.clear();
    m_elem.shrink_to_fit();
}

template<typename T>
void MyMatrix<T>::setDim(unsigned row, unsigned col) {
    m_elem.resize(row);
    for (unsigned i = 0; i < row; i++) {
        m_elem[i].resize(col);
    }
}

template<typename T>
const vector<T> &MyMatrix<T>::operator[](unsigned row) const {
    return m_elem[row];
}

template<typename T>
vector<T> &MyMatrix<T>::operator[](unsigned row) {
    return m_elem[row];
}

template<typename T>
unsigned MyMatrix<T>::getRow() const {
    return m_elem.size();
}

template<typename T>
unsigned MyMatrix<T>::getCol() const {
    return getRow() ? m_elem[0].size() : 0;
}

template<typename T>
MyMatrix<T> MyMatrix<T>::getRowMyMatrix(unsigned r) const {
    MyMatrix<T> R(1, getCol());
    for (unsigned i = 0; i < getCol(); i++) {
        R[0][i] = m_elem[r - 1][i];
    }
    return R;
}

template<typename T>
MyMatrix<T> MyMatrix<T>::getColMyMatrix(unsigned c) const {
    MyMatrix<T> C(getRow(), 1);
    for (unsigned i = 0; i < getRow(); i++) {
        C[i][0] = m_elem[i][c - 1];
    }
    return C;
}

template<typename T>
void MyMatrix<T>::randomFillMatrix(int low, int high) {
    std::random_device r;
    std::mt19937 mtgen(r());
    std::uniform_int_distribution<> dist(low, high);
    for (int i = 0; i < getRow(); ++i) {
        for (int j = 0; j < getCol(); ++j) {
            m_elem[i][j] = dist(mtgen);
        }
    }
}

template<typename T>
void MyMatrix<T>::showMatrix() {
    for (unsigned i = 0; i < m_elem.size(); i++) {
        for (unsigned j = 0; j < m_elem[i].size(); j++) {
            cout << m_elem[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

template<typename T>
void MyMatrix<T>::showDiagonal() {
    if (!isSquare()) {
        return;
    }
    for (unsigned i = 0; i < m_elem.size(); i++) {
        cout << m_elem[i][i] << " ";
    }
    cout << endl;
}

template<typename T>
bool MyMatrix<T>::isSquare() const {
    return getRow() == getCol();
}

#endif //MYCPPIMPLEMENT_MYMATRIX_H
