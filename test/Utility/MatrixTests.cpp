#include <gtest/gtest.h>
#include <iostream>

#include "Matrix.hpp"

using namespace raytracer;
using namespace utility;

TEST(matrix_tests, Matrix_creation_4x4) {
  const Matrix<4,4>M1{1.0f, 2.0f, 3.0f, 4.0f, 
                      5.5f, 6.5f, 7.5f, 8.5f, 
                      9.0f, 10.0f, 11.0f, 12.0f, 
                      13.5f, 14.5f, 15.5f, 16.5f};

  EXPECT_EQ(M1.at(0,0), 1);
  EXPECT_EQ(M1.at(0,3), 4);
  EXPECT_EQ(M1.at(1,0), 5.5f);
  EXPECT_EQ(M1.at(1,2), 7.5f);
  EXPECT_EQ(M1.at(2,2), 11);
  EXPECT_EQ(M1.at(3,0), 13.5f);
  EXPECT_EQ(M1.at(3,2), 15.5f);
}

TEST(matrix_tests, Matrix_creation_2x2) {
  const Matrix<2,2>M1{-3.0f, 5.0f,
                       1.0f,-2.0f};

  EXPECT_EQ(M1.at(0,0), -3);
  EXPECT_EQ(M1.at(0,1), 5);
  EXPECT_EQ(M1.at(1,0), 1);
  EXPECT_EQ(M1.at(1,1), -2);
}

TEST(matrix_tests, Matrix_creation_3x3) {
  const Matrix<3,3>M1{-3.0f, 5.0f, 0.0f,
                       1.0f,-2.0f, -7.0f,
                       0.0f, 1.0f, 1.0f};

  EXPECT_EQ(M1.at(0,0), -3);
  EXPECT_EQ(M1.at(1,1), -2);
  EXPECT_EQ(M1.at(2,2), 1);
}

TEST(matrix_tests, Matrix_equality_identical) {
  const Matrix<4,4>M1{1.0f, 2.0f, 3.0f, 4.0f,
                      5.0f, 6.0f, 7.0f, 8.0f,
                      9.0f, 8.0f, 7.0f, 6.0f,
                      5.0f, 4.0f, 3.0f, 2.0f};

  const Matrix<4,4>M2{1.0f, 2.0f, 3.0f, 4.0f,
                      5.0f, 6.0f, 7.0f, 8.0f,
                      9.0f, 8.0f, 7.0f, 6.0f,
                      5.0f, 4.0f, 3.0f, 2.0f};

  EXPECT_EQ(M1, M2);
}

TEST(matrix_tests, Matrix_equality_different) {
  const Matrix<4,4>M1{1.0f, 2.0f, 3.0f, 4.0f,
                      5.0f, 6.0f, 7.0f, 8.0f,
                      9.0f, 8.0f, 7.0f, 6.0f,
                      5.0f, 4.0f, 3.0f, 2.0f};

  const Matrix<4,4>M2{2.0f, 3.0f, 4.0f, 5.0f, 
                      6.0f, 7.0f, 8.0f, 9.0f, 
                      8.0f, 7.0f, 6.0f, 5.0f, 
                      4.0f, 3.0f, 2.0f, 1.0f};

  EXPECT_NE(M1, M2);
}

TEST(matrix_tests, Matrix_multiplication) {
  const Matrix<4,4>M1{1.0f, 2.0f, 3.0f, 4.0f,
                      5.0f, 6.0f, 7.0f, 8.0f,
                      9.0f, 8.0f, 7.0f, 6.0f,
                      5.0f, 4.0f, 3.0f, 2.0f};

  const Matrix<4,4>M2{-2.0f, 1.0f, 2.0f, 3.0f,
                       3.0f, 2.0f, 1.0f,-1.0f,
                       4.0f, 3.0f, 6.0f, 5.0f,
                       1.0f, 2.0f, 7.0f, 8.0f};

  const Matrix<4,4>M3{20.0f, 22.0f, 50.0f, 48.0f,
                      44.0f, 54.0f,114.0f,108.0f,
                      40.0f, 58.0f,110.0f,102.0f,
                      16.0f, 26.0f, 46.0f, 42.0f};
  auto M4 = M1 * M2;

  EXPECT_EQ(M3, M4);
}

TEST(matrix_tests, Matrix_vector_multiply) {
  const Matrix<4,4>M1{1.0f, 2.0f, 3.0f, 4.0f,
                      2.0f, 4.0f, 4.0f, 2.0f,
                      8.0f, 6.0f, 4.0f, 1.0f,
                      0.0f, 0.0f, 0.0f, 1.0f};

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
  const Matrix<4,4> M1{0.0f, 9.0f, 3.0f, 0.0f,
                       9.0f, 8.0f, 0.0f, 8.0f, 
                       1.0f, 8.0f, 5.0f, 3.0f,
                       0.0f, 0.0f, 5.0f, 8.0f};
  const Matrix<4,4> M2 = M1.transpose();
  const Matrix<4,4> M3{0.0f, 9.0f, 1.0f, 0.0f,
                       9.0f, 8.0f, 8.0f, 0.0f,
                       3.0f, 0.0f, 5.0f, 5.0f,
                       0.0f, 8.0f, 3.0f, 8.0f};
  EXPECT_EQ(M3, M2);
}

TEST(matrix_tests, Matrix_transpose_identity) {
  const Matrix<4,4> M1 = Matrix<4,4>::identity();

  EXPECT_EQ(M1, M1.transpose());
}

TEST(matrix_tests, Matrix_determinant) {
  const Matrix<2,2> M1{ 1.0f, 5.0f,
                       -3.0f, 2.0f};
  const auto determinantM1 = 17.0f;

  EXPECT_EQ(M1.determinant(), determinantM1);
}

TEST(matrix_tests, Matrix_3x3_to_2x2_submatrix){
  const Matrix<3,3> M1{ 1.0f, 5.0f, 0.0f,
                       -3.0f, 2.0f, 7.0f,
                        0.0f, 6.0f,-3.0f};
  const Matrix<2,2> subMatrix{-3.0f, 2.0f,
                               0.0f, 6.0f};

  EXPECT_EQ(submatrix(M1, 0, 2), subMatrix);
}

TEST(matrix_tests, Matrix_4x4_to_3x3_submatrix){
  const Matrix<4,4> M1{-6.0f, 1.0f, 1.0f, 6.0f,
                        -8.0f, 5.0f, 8.0f, 6.0f,
                        -1.0f, 0.0f, 8.0f, 2.0f,
                        -7.0f, 1.0f,-1.0f, 1.0f};

  const Matrix<3,3> subMatrix{-6.0f, 1.0f, 6.0f,
                              -8.0f, 8.0f, 6.0f,
                              -7.0f,-1.0f, 1.0f};

  EXPECT_EQ(submatrix(M1, 2, 1), subMatrix);
}

TEST(matrix_tests, Matrix_3x3_cofactor){
  const Matrix<3,3> M1{ 3.0f, 5.0f, 0.0f,
                        2.0f,-1.0f,-7.0f,
                        6.0f,-1.0f, 5.0f};

  const Matrix<3,3> subMatrix{-6.0f, 1.0f, 6.0f,
                              -8.0f, 8.0f, 6.0f,
                              -7.0f,-1.0f, 1.0f};

  EXPECT_EQ(minor(M1,0,0), -12);
  EXPECT_EQ(minor(M1,0,0), cofactor(M1,0,0));

  EXPECT_EQ(minor(M1,1,0), 25);
  EXPECT_EQ(minor(M1,1,0), -cofactor(M1,1,0));
}

TEST(matrix_tests, Matrix_3x3_determinant){
  const Matrix<3,3> M1{ 1.0f, 2.0f, 6.0f,
                       -5.0f, 8.0f,-4.0f,
                        2.0f, 6.0f, 4.0f};

  EXPECT_EQ(cofactor(M1,0,0), 56);
  EXPECT_EQ(cofactor(M1,0,1), 12);
  EXPECT_EQ(cofactor(M1,0,2), -46);
  EXPECT_EQ(M1.determinant(), -196);
}

TEST(matrix_tests, Matrix_4x4_determinant){
  const Matrix<4,4> M1{-2.0f,-8.0f, 3.0f, 5.0f,
                       -3.0f, 1.0f, 7.0f, 3.0f,
                        1.0f, 2.0f,-9.0f, 6.0f,
                       -6.0f, 7.0f, 7.0f,-9.0f};

  EXPECT_EQ(cofactor(M1,0,0), 690);
  EXPECT_EQ(cofactor(M1,0,1), 447);
  EXPECT_EQ(cofactor(M1,0,2), 210);
  EXPECT_EQ(cofactor(M1,0,3),  51);
  EXPECT_EQ(M1.determinant(),-4071);
}

TEST(matrix_tests, Matrix_invertible_true){
  const Matrix<4,4> M1{ 6.0f, 4.0f, 4.0f, 4.0f,
                        5.0f, 5.0f, 7.0f, 6.0f,
                        4.0f,-9.0f, 3.0f,-7.0f,
                        9.0f, 1.0f, 7.0f,-6.0f};

  EXPECT_EQ(M1.determinant(), -2120);
  EXPECT_TRUE(M1.invertible());
}

TEST(matrix_tests, Matrix_invertible_false){
  const Matrix<4,4> M1{-4.0f, 2.0f,-2.0f,-3.0f,
                        9.0f, 6.0f, 2.0f, 6.0f,
                        0.0f,-5.0f, 1.0f,-5.0f,
                        0.0f, 0.0f, 0.0f, 0.0f};

  EXPECT_EQ(M1.determinant(), 0);
  EXPECT_FALSE(M1.invertible());
}

TEST(matrix_tests, Matrix_inverse_thorough){
  const Matrix<4,4> M1{-5.0f, 2.0f, 6.0f,-8.0f,
                        1.0f,-5.0f, 1.0f, 8.0f,
                        7.0f, 7.0f,-6.0f,-7.0f,
                        1.0f,-3.0f, 7.0f, 4.0f};
  auto M2 = inverse(M1);

  std::array<float, 16> cofactors{116, 240, 128,-24,
                                  -430,-775,-236, 277,
                                  -42, -119,-28,  105,
                                  -278,-433,-160, 163};
  std::transform(cofactors.cbegin(), cofactors.cend(), 
                 cofactors.begin(), 
                 [M1](const float& arrayElement){return arrayElement/M1.determinant();});
  const Matrix<4,4> M3{cofactors};

  EXPECT_EQ(M1.determinant(), 532);
  EXPECT_EQ(cofactor(M1, 2, 3), -160);
  EXPECT_EQ(M2.at(3,2), -160.0f/532);
  EXPECT_EQ(cofactor(M1, 3, 2), 105);
  EXPECT_EQ(M2.at(2,3), 105.0f/532);
  EXPECT_EQ(M3, M2);
}

TEST(matrix_tests, Matrix_inverse_Simple1){
  const Matrix<4,4> M1{ 8.0f,-5.0f, 9.0f, 2.0f,
                        7.0f, 5.0f, 6.0f, 1.0f,
                       -6.0f, 0.0f, 9.0f, 6.0f,
                       -3.0f, 0.0f,-9.0f,-4.0f};
  auto M2 = inverse(M1);

  std::array<float, 16> cofactors{  90,    90,   165,   315,
                                    45,   -72,   -15,   -18,
                                  -210,  -210,  -255,  -540,
                                   405,   405,   450,  1125};
  std::transform(cofactors.cbegin(), cofactors.cend(), 
                 cofactors.begin(), 
                 [M1](const float& arrayElement){return arrayElement/M1.determinant();});
  const Matrix<4,4> M3{cofactors};

  EXPECT_EQ(M3, M2);
}

TEST(matrix_tests, Matrix_inverse_Simple2){
  const Matrix<4,4> M1{ 9.0f, 3.0f, 0.0f, 9.0f,
                       -5.0f,-2.0f,-6.0f,-3.0f,
                       -4.0f, 9.0f, 6.0f, 4.0f,
                       -7.0f, 6.0f, 6.0f, 2.0f};
  auto M2 = inverse(M1);

  std::array<float, 16> cofactors{ -66.0f,  -126.0f,   234.0f,  -360.0f,
                                   -126.0f,    54.0f,   594.0f,  -540.0f,
                                    -47.0f,  -237.0f,  -177.0f,   210.0f,
                                    288.0f,   108.0f,  -432.0f,   540.0f};
  std::transform(cofactors.cbegin(), cofactors.cend(), 
                 cofactors.begin(), 
                 [M1](const float& arrayElement){return arrayElement/M1.determinant();});
  const Matrix<4,4> M3{cofactors};

  EXPECT_EQ(M3, M2);
}

TEST(matrix_tests, Matrix_multiply_product_by_inverse){
  const Matrix<4,4> M1{ 3.0f,-9.0f, 7.0f, 3.0f,
                        3.0f,-8.0f, 2.0f,-9.0f,
                       -4.0f, 4.0f, 4.0f, 1.0f,
                       -6.0f, 5.0f,-1.0f, 1.0f};
  const Matrix<4,4> M2{ 8.0f, 2.0f, 2.0f, 2.0f,
                        3.0f,-1.0f, 7.0f, 0.0f,
                        7.0f, 0.0f, 5.0f, 4.0f,
                        6.0f,-2.0f, 0.0f, 5.0f};
  auto M3 = M1*M2;
  auto M4 = M3*inverse(M2);
  EXPECT_TRUE(M1==M4);
}