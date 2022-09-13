#include <gtest/gtest.h>
#include <iostream>

#include "include/matrix.h"

TEST(matrix_test, Matrix_creation_4x4) {
  const Matrix<4,4>M1{1.0, 2.0, 3.0, 4.0, 
                      5.5, 6.5, 7.5, 8.5, 
                      9.0, 10.0, 11.0, 12.0, 
                      13.5, 14.5, 15.5, 16.5};

  EXPECT_EQ(M1.at(0,0), 1);
  EXPECT_EQ(M1.at(0,3), 4);
  EXPECT_EQ(M1.at(1,0), 5.5);
  EXPECT_EQ(M1.at(1,2), 7.5);
  EXPECT_EQ(M1.at(2,2), 11);
  EXPECT_EQ(M1.at(3,0), 13.5);
  EXPECT_EQ(M1.at(3,2), 15.5);
}

TEST(matrix_test, Matrix_creation_2x2) {
  const Matrix<2,2>M1{-3.0, 5.0,
                       1.0,-2.0};

  EXPECT_EQ(M1.at(0,0), -3);
  EXPECT_EQ(M1.at(0,1), 5);
  EXPECT_EQ(M1.at(1,0), 1);
  EXPECT_EQ(M1.at(1,1), -2);
}

TEST(matrix_test, Matrix_creation_3x3) {
  const Matrix<3,3>M1{-3.0, 5.0, 0.0,
                       1.0,-2.0, -7.0,
                       0.0, 1.0, 1.0};

  EXPECT_EQ(M1.at(0,0), -3);
  EXPECT_EQ(M1.at(1,1), -2);
  EXPECT_EQ(M1.at(2,2), 1);
}

TEST(matrix_test, Matrix_equality_identical) {
  const Matrix<4,4>M1{1.0, 2.0, 3.0, 4.0,
                      5.0, 6.0, 7.0, 8.0,
                      9.0, 8.0, 7.0, 6.0,
                      5.0, 4.0, 3.0, 2.0};

  const Matrix<4,4>M2{1.0, 2.0, 3.0, 4.0,
                      5.0, 6.0, 7.0, 8.0,
                      9.0, 8.0, 7.0, 6.0,
                      5.0, 4.0, 3.0, 2.0};

  EXPECT_EQ(M1, M2);
}

TEST(matrix_test, Matrix_equality_different) {
  const Matrix<4,4>M1{1.0, 2.0, 3.0, 4.0,
                      5.0, 6.0, 7.0, 8.0,
                      9.0, 8.0, 7.0, 6.0,
                      5.0, 4.0, 3.0, 2.0};

  const Matrix<4,4>M2{2.0, 3.0, 4.0, 5.0, 
                      6.0, 7.0, 8.0, 9.0, 
                      8.0, 7.0, 6.0, 5.0, 
                      4.0, 3.0, 2.0, 1.0};

  EXPECT_NE(M1, M2);
}