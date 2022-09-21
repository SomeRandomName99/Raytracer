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

TEST(matrix_test, Matrix_multiplication) {
  const Matrix<4,4>M1{1.0, 2.0, 3.0, 4.0,
                      5.0, 6.0, 7.0, 8.0,
                      9.0, 8.0, 7.0, 6.0,
                      5.0, 4.0, 3.0, 2.0};

  const Matrix<4,4>M2{-2.0, 1.0, 2.0, 3.0,
                       3.0, 2.0, 1.0,-1.0,
                       4.0, 3.0, 6.0, 5.0,
                       1.0, 2.0, 7.0, 8.0};

  const Matrix<4,4>M3{20.0, 22.0, 50.0, 48.0,
                      44.0, 54.0,114.0,108.0,
                      40.0, 58.0,110.0,102.0,
                      16.0, 26.0, 46.0, 42.0};
  auto M4 = M1 * M2;

  EXPECT_EQ(M3, M4);
}

TEST(matrix_test, Matrix_vector_multiply) {
  const Matrix<4,4>M1{1.0, 2.0, 3.0, 4.0,
                      2.0, 4.0, 4.0, 2.0,
                      8.0, 6.0, 4.0, 1.0,
                      0.0, 0.0, 0.0, 1.0};

  const Tuple T1{1.0f,2.0f,3.0f,1.0f};
  const Tuple T3{18.0f,24.0f,33.0f,1.0f};

  Tuple T2 = M1 * T1;

  EXPECT_EQ(T2, T3);
}

TEST(matrix_test, Matrix_vector_multiply_identity) {
  const auto M1 = Matrix<4,4>::identity();

  const Tuple T1{1.0f,2.0f,3.0f,1.0f};

  Tuple T2 = M1 * T1;

  EXPECT_EQ(T2, T1);
}