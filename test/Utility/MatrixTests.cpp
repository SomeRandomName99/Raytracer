#include <gtest/gtest.h>
#include <iostream>

#include "Matrix.hpp"

TEST(matrix_tests, Matrix_creation_4x4) {
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

TEST(matrix_tests, Matrix_creation_2x2) {
  const Matrix<2,2>M1{-3.0, 5.0,
                       1.0,-2.0};

  EXPECT_EQ(M1.at(0,0), -3);
  EXPECT_EQ(M1.at(0,1), 5);
  EXPECT_EQ(M1.at(1,0), 1);
  EXPECT_EQ(M1.at(1,1), -2);
}

TEST(matrix_tests, Matrix_creation_3x3) {
  const Matrix<3,3>M1{-3.0, 5.0, 0.0,
                       1.0,-2.0, -7.0,
                       0.0, 1.0, 1.0};

  EXPECT_EQ(M1.at(0,0), -3);
  EXPECT_EQ(M1.at(1,1), -2);
  EXPECT_EQ(M1.at(2,2), 1);
}

TEST(matrix_tests, Matrix_equality_identical) {
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

TEST(matrix_tests, Matrix_equality_different) {
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

TEST(matrix_tests, Matrix_multiplication) {
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

TEST(matrix_tests, Matrix_vector_multiply) {
  const Matrix<4,4>M1{1.0, 2.0, 3.0, 4.0,
                      2.0, 4.0, 4.0, 2.0,
                      8.0, 6.0, 4.0, 1.0,
                      0.0, 0.0, 0.0, 1.0};

  const Tuple T1{1.0f,2.0f,3.0f,1.0f};
  const Tuple T3{18.0f,24.0f,33.0f,1.0f};

  Tuple T2 = M1 * T1;

  EXPECT_EQ(T2, T3);
}

TEST(matrix_tests, Matrix_vector_multiply_identity) {
  const auto M1 = Matrix<4,4>::identity();

  const Tuple T1{1.0f,2.0f,3.0f,1.0f};

  Tuple T2 = M1 * T1;

  EXPECT_EQ(T2, T1);
}

TEST(matrix_tests, Matrix_transpose) {
  const Matrix<4,4> M1{0.0, 9.0, 3.0, 0.0,
                       9.0, 8.0, 0.0, 8.0, 
                       1.0, 8.0, 5.0, 3.0,
                       0.0, 0.0, 5.0, 8.0};
  const Matrix<4,4> M2 = M1.transpose();
  const Matrix<4,4> M3{0.0, 9.0, 1.0, 0.0,
                       9.0, 8.0, 8.0, 0.0,
                       3.0, 0.0, 5.0, 5.0,
                       0.0, 8.0, 3.0, 8.0};
  EXPECT_EQ(M3, M2);
}

TEST(matrix_tests, Matrix_transpose_identity) {
  const Matrix<4,4> M1 = Matrix<4,4>::identity();

  EXPECT_EQ(M1, M1.transpose());
}

TEST(matrix_tests, Matrix_determinant) {
  const Matrix<2,2> M1{ 1.0, 5.0,
                       -3.0, 2.0};
  const auto determinantM1 = 17.0f;

  EXPECT_EQ(M1.determinant(), determinantM1);
}

TEST(matrix_tests, Matrix_3x3_to_2x2_submatrix){
  const Matrix<3,3> M1{ 1.0, 5.0, 0.0,
                       -3.0, 2.0, 7.0,
                        0.0, 6.0,-3.0};
  const Matrix<2,2> subMatrix{-3.0, 2.0,
                               0.0, 6.0};

  EXPECT_EQ(submatrix(M1, 0, 2), subMatrix);
}

TEST(matrix_tests, Matrix_4x4_to_3x3_submatrix){
  const Matrix<4,4> M1{-6.0, 1.0, 1.0, 6.0,
                        -8.0, 5.0, 8.0, 6.0,
                        -1.0, 0.0, 8.0, 2.0,
                        -7.0, 1.0,-1.0, 1.0};

  const Matrix<3,3> subMatrix{-6.0, 1.0, 6.0,
                              -8.0, 8.0, 6.0,
                              -7.0,-1.0, 1.0};

  EXPECT_EQ(submatrix(M1, 2, 1), subMatrix);
}

TEST(matrix_tests, Matrix_3x3_cofactor){
  const Matrix<3,3> M1{ 3.0, 5.0, 0.0,
                        2.0,-1.0,-7.0,
                        6.0,-1.0, 5.0};

  const Matrix<3,3> subMatrix{-6.0, 1.0, 6.0,
                              -8.0, 8.0, 6.0,
                              -7.0,-1.0, 1.0};

  EXPECT_EQ(minor(M1,0,0), -12);
  EXPECT_EQ(minor(M1,0,0), cofactor(M1,0,0));

  EXPECT_EQ(minor(M1,1,0), 25);
  EXPECT_EQ(minor(M1,1,0), -cofactor(M1,1,0));
}

TEST(matrix_tests, Matrix_3x3_determinant){
  const Matrix<3,3> M1{ 1.0, 2.0, 6.0,
                       -5.0, 8.0,-4.0,
                        2.0, 6.0, 4.0};

  EXPECT_EQ(cofactor(M1,0,0), 56);
  EXPECT_EQ(cofactor(M1,0,1), 12);
  EXPECT_EQ(cofactor(M1,0,2), -46);
  EXPECT_EQ(M1.determinant(), -196);
}

TEST(matrix_tests, Matrix_4x4_determinant){
  const Matrix<4,4> M1{-2.0,-8.0, 3.0, 5.0,
                       -3.0, 1.0, 7.0, 3.0,
                        1.0, 2.0,-9.0, 6.0,
                       -6.0, 7.0, 7.0,-9.0};

  EXPECT_EQ(cofactor(M1,0,0), 690);
  EXPECT_EQ(cofactor(M1,0,1), 447);
  EXPECT_EQ(cofactor(M1,0,2), 210);
  EXPECT_EQ(cofactor(M1,0,3),  51);
  EXPECT_EQ(M1.determinant(),-4071);
}

TEST(matrix_tests, Matrix_invertible_true){
  const Matrix<4,4> M1{ 6.0, 4.0, 4.0, 4.0,
                        5.0, 5.0, 7.0, 6.0,
                        4.0,-9.0, 3.0,-7.0,
                        9.0, 1.0, 7.0,-6.0};

  EXPECT_EQ(M1.determinant(), -2120);
  EXPECT_TRUE(M1.invertible());
}

TEST(matrix_tests, Matrix_invertible_false){
  const Matrix<4,4> M1{-4.0, 2.0,-2.0,-3.0,
                        9.0, 6.0, 2.0, 6.0,
                        0.0,-5.0, 1.0,-5.0,
                        0.0, 0.0, 0.0, 0.0};

  EXPECT_EQ(M1.determinant(), 0);
  EXPECT_FALSE(M1.invertible());
}

TEST(matrix_tests, Matrix_inverse_thorough){
  const Matrix<4,4> M1{-5.0, 2.0, 6.0,-8.0,
                        1.0,-5.0, 1.0, 8.0,
                        7.0, 7.0,-6.0,-7.0,
                        1.0,-3.0, 7.0, 4.0};
  auto M2 = inverse(M1);

  std::array<double, 16> cofactors{116, 240, 128,-24,
                                  -430,-775,-236, 277,
                                  -42, -119,-28,  105,
                                  -278,-433,-160, 163};
  std::transform(cofactors.cbegin(), cofactors.cend(), 
                 cofactors.begin(), 
                 [M1](const double& arrayElement){return arrayElement/M1.determinant();});
  const Matrix<4,4> M3{cofactors};

  EXPECT_EQ(M1.determinant(), 532);
  EXPECT_EQ(cofactor(M1, 2, 3), -160);
  EXPECT_EQ(M2.at(3,2), -160.0/532);
  EXPECT_EQ(cofactor(M1, 3, 2), 105);
  EXPECT_EQ(M2.at(2,3), 105.0/532);
  EXPECT_EQ(M3, M2);
}

TEST(matrix_tests, Matrix_inverse_Simple1){
  const Matrix<4,4> M1{ 8.0,-5.0, 9.0, 2.0,
                        7.0, 5.0, 6.0, 1.0,
                       -6.0, 0.0, 9.0, 6.0,
                       -3.0, 0.0,-9.0,-4.0};
  auto M2 = inverse(M1);

  std::array<double, 16> cofactors{  90,    90,   165,   315,
                                    45,   -72,   -15,   -18,
                                  -210,  -210,  -255,  -540,
                                   405,   405,   450,  1125};
  std::transform(cofactors.cbegin(), cofactors.cend(), 
                 cofactors.begin(), 
                 [M1](const double& arrayElement){return arrayElement/M1.determinant();});
  const Matrix<4,4> M3{cofactors};

  EXPECT_EQ(M3, M2);
}

TEST(matrix_tests, Matrix_inverse_Simple2){
  const Matrix<4,4> M1{ 9.0, 3.0, 0.0, 9.0,
                       -5.0,-2.0,-6.0,-3.0,
                       -4.0, 9.0, 6.0, 4.0,
                       -7.0, 6.0, 6.0, 2.0};
  auto M2 = inverse(M1);

  std::array<double, 16> cofactors{ -66.0,  -126.0,   234.0,  -360.0,
                                   -126.0,    54.0,   594.0,  -540.0,
                                    -47.0,  -237.0,  -177.0,   210.0,
                                    288.0,   108.0,  -432.0,   540.0};
  std::transform(cofactors.cbegin(), cofactors.cend(), 
                 cofactors.begin(), 
                 [M1](const double& arrayElement){return arrayElement/M1.determinant();});
  const Matrix<4,4> M3{cofactors};

  EXPECT_EQ(M3, M2);
}

TEST(matrix_tests, Matrix_multiply_product_by_inverse){
  const Matrix<4,4> M1{ 3.0,-9.0, 7.0, 3.0,
                        3.0,-8.0, 2.0,-9.0,
                       -4.0, 4.0, 4.0, 1.0,
                       -6.0, 5.0,-1.0, 1.0};
  const Matrix<4,4> M2{ 8.0, 2.0, 2.0, 2.0,
                        3.0,-1.0, 7.0, 0.0,
                        7.0, 0.0, 5.0, 4.0,
                        6.0,-2.0, 0.0, 5.0};
  auto M3 = M1*M2;
  auto M4 = M3*inverse(M2);
  EXPECT_TRUE(M1==M4);
}