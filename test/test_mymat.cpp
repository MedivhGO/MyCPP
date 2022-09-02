//
// Created by Lee on 2022/9/2.
//
#include <gtest/gtest.h>

#include "MyMatrix.h"

TEST(MyMatrixTest, init) {
    MyMatrix<int> m(5, 5);
    m.showMatrix();
    EXPECT_EQ(5, m.getCol());
    EXPECT_EQ(true, m.isSquare());
}