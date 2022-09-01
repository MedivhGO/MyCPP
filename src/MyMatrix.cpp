#include <iostream>

#include "MyMatrix.h"

using std::cout;
using std::endl;

template<typename T>
MyMatrix<T>::MyMatrix() {
    setDim(1, 1);
}

template<typename T>
MyMatrix<T>::MyMatrix(unsigned row, unsigned col) {
    setDim(row, col);
}

template<typename T>
MyMatrix<T>::MyMatrix(vector<T> &r) {
    setDim(r.size(), 1);
    elem[0] = r;
}

template<typename T>
MyMatrix<T>::~MyMatrix() {
    for (unsigned i = 0; i < elem.size(); i++) {
        elem[i].clear();
        elem[i].shrink_to_fit();
    }
    elem.clear();
    elem.shrink_to_fit();
}

template<typename T>
void MyMatrix<T>::setDim(unsigned row, unsigned col) {
    elem.resize(row);
    for (unsigned i = 0; i < row; i++) {
        elem[i].resize(col);
    }
}

template<typename T>
const vector<T> &MyMatrix<T>::operator[](unsigned row) const {
    return elem[row];
}

template<typename T>
vector<T> &MyMatrix<T>::operator[](unsigned row) {
    return elem[row];
}

template<typename T>
unsigned MyMatrix<T>::getRow() const {
    return elem.size();
}

template<typename T>
unsigned MyMatrix<T>::getCol() const {
    return getRow() ? elem[0].size() : 0;
}

template<typename T>
MyMatrix<T> MyMatrix<T>::getRowMyMatrix(unsigned r) const {
    MyMatrix<T> R(1, getCol());
    for (unsigned i = 0; i < getCol(); i++) {
        R[0][i] = elem[r - 1][i];
    }
    return R;
}

template<typename T>
MyMatrix<T> MyMatrix<T>::getColMyMatrix(unsigned c) const {
    MyMatrix<T> C(getRow(), 1);
    for (unsigned i = 0; i < getRow(); i++) {
        C[i][0] = elem[i][c - 1];
    }
    return C;
}

template<typename T>
void MyMatrix<T>::showMatrix() {
    for (unsigned i = 0; i < elem.size(); i++) {
        for (unsigned j = 0; j < elem[i].size(); j++) {
            cout << elem[i][j] << " ";
        }
        cout << endl;
    }
}

template<typename T>
bool MyMatrix<T>::isSquare() const {
    return getRow() == getCol();
}

template<typename T>
MyMatrix<T> operator+(const MyMatrix<T> &A, const MyMatrix<T> &B) {
    MyMatrix<T> C(A.getRow(), A.getCol());
    for (unsigned i = 0; i < C.getRow(); i++) {
        for (unsigned j = 0; j < C.getCol(); j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

template<typename T>
MyMatrix<T> operator-(const MyMatrix<T> &A, const MyMatrix<T> &B) {
    MyMatrix<T> C(A.getRow(), A.getCol());
    for (unsigned i = 0; i < C.getRow(); i++) {
        for (unsigned j = 0; j < C.getCol(); j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

template<typename T>
MyMatrix<T> operator*(const MyMatrix<T> &A, const MyMatrix<T> &B) {
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

template<typename T>
MyMatrix<T> operator*(const T &e, const MyMatrix<T> &A) {
    MyMatrix<T> C(A.getRow(), A.getCol());
    for (unsigned i = 0; i < C.getRow(); i++) {
        for (unsigned j = 0; j < C.getCol(); j++) {
            C[i][j] = A[i][j] * e;
        }
    }
    return C;
}

template<typename T>
MyMatrix<T> operator/(const MyMatrix<T> &A, const T &e) {
    MyMatrix<T> C(A.getRow(), A.getCol());
    for (unsigned i = 0; i < C.getRow(); i++) {
        for (unsigned j = 0; j < C.getCol(); j++) {
            C[i][j] = A[i][j] / e;
        }
    }
    return C;
}


