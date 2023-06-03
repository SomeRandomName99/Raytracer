#include <gtest/gtest.h>
#include <numbers>

#include "Transformations.hpp"
#include "Tuple.hpp"

/* =========== Translation =========== */
TEST(transformations_tests, multiplication_translation_matrix){
  auto transform = transformations::translation(5,-3,2);
  auto p         = Point(-3,4,5);

  auto transformedPoint = transform * p;

  EXPECT_EQ(transformedPoint, Point(2,1,7));
}

TEST(transformations_tests, translation_multiplication_inverse_translation_matrix){
  auto transform = transformations::translation(5,-3, 2);
  auto transform_inverse = inverse(transform);
  auto p         = Point(-3,4,5);

  auto transformedPoint = transform_inverse * p;

  EXPECT_EQ(transformedPoint, Point(-8,7,3));
}

TEST(transformations_tests, multiplication_does_not_affect_vectors){
  auto transform = transformations::translation(5,-3,2);
  auto v         = Vector(-3,4,5);

  auto transformedVector = transform * v;

  EXPECT_EQ(transformedVector, Vector(-3,4,5));
}

/* =========== Scaling =========== */
TEST(transformations_tests, scaling_point){
  auto transform = transformations::scaling(2,3,4);
  auto p         = Point(-4,6,8);

  auto transformedPoint = transform * p;

  EXPECT_EQ(transformedPoint, Point(-8,18,32));
}

TEST(transformations_tests, scaling_vector){
  auto transform = transformations::scaling(2,3,4);
  auto v         = Vector(-4,6,8);

  auto transformedVector = transform * v;

  EXPECT_EQ(transformedVector, Vector(-8,18,32));
}

TEST(transformations_tests, scaling_multiplication_inverse_translation_matrix){
  auto transform         = transformations::scaling(2,3,4);
  auto transform_inverse = inverse(transform);
  auto v                 = Vector(-4,6,8);

  auto transformedVector = transform_inverse * v;

  EXPECT_EQ(transformedVector, Vector(-2,2,2));
}

TEST(transformations_tests, reflections_backwards){
  auto transform         = transformations::scaling(-1,1,1);
  auto p                 = Point(2,3,4);

  auto transformedVector = transform * p;

  EXPECT_EQ(transformedVector, Point(-2,3,4));
}

/* =========== Rotation =========== */
TEST(transformations_tests, rotate_x_axis){
  auto p                         = Point(0,1,0);
  auto halfQuarterTransform      = transformations::rotation_x(std::numbers::pi/4);
  auto fullQuarterTransform      = transformations::rotation_x(std::numbers::pi/2);


  auto halfQuarter = halfQuarterTransform * p;
  auto fullQuarter = fullQuarterTransform * p;


  EXPECT_EQ(halfQuarter, Point(0,std::sqrt(2)/2,std::sqrt(2)/2));
  EXPECT_EQ(fullQuarter, Point(0,0,1));
}

TEST(transformations_tests, rotate_y_axis){
  auto p                         = Point(0,0,1);
  auto halfQuarterTransform      = transformations::rotation_y(std::numbers::pi/4);
  auto fullQuarterTransform      = transformations::rotation_y(std::numbers::pi/2);


  auto halfQuarter = halfQuarterTransform * p;
  auto fullQuarter = fullQuarterTransform * p;


  EXPECT_EQ(halfQuarter, Point(std::sqrt(2)/2,0,std::sqrt(2)/2));
  EXPECT_EQ(fullQuarter, Point(1,0,0));
}

TEST(transformations_tests, rotate_z_axis){
  auto p                         = Point(0,1,0);
  auto halfQuarterTransform      = transformations::rotation_z(std::numbers::pi/4);
  auto fullQuarterTransform      = transformations::rotation_z(std::numbers::pi/2);


  auto halfQuarter = halfQuarterTransform * p;
  auto fullQuarter = fullQuarterTransform * p;


  EXPECT_EQ(halfQuarter, Point(-std::sqrt(2)/2,std::sqrt(2)/2,0));
  EXPECT_EQ(fullQuarter, Point(-1,0,0));
}

/* =========== Shearing =========== */
TEST(transformations_tests, shearing_x_y){
  auto transform = transformations::shearing(1,0,0,0,0,0);
  auto p         = Point(2,3,4);

  auto transformedVector = transform * p;

  EXPECT_EQ(transformedVector, Point(5,3,4));
}
TEST(transformations_tests, shearing_x_z){
  auto transform = transformations::shearing(0,1,0,0,0,0);
  auto p         = Point(2,3,4);

  auto transformedVector = transform * p;

  EXPECT_EQ(transformedVector, Point(6,3,4));
}
TEST(transformations_tests, shearing_y_x){
  auto transform = transformations::shearing(0,0,1,0,0,0);
  auto p         = Point(2,3,4);

  auto transformedVector = transform * p;

  EXPECT_EQ(transformedVector, Point(2,5,4));
}
TEST(transformations_tests, shearing_y_z){
  auto transform = transformations::shearing(0,0,0,1,0,0);
  auto p         = Point(2,3,4);

  auto transformedVector = transform * p;

  EXPECT_EQ(transformedVector, Point(2,7,4));
}
TEST(transformations_tests, shearing_z_x){
  auto transform = transformations::shearing(0,0,0,0,1,0);
  auto p         = Point(2,3,4);

  auto transformedVector = transform * p;

  EXPECT_EQ(transformedVector, Point(2,3,6));
}
TEST(transformations_tests, shearing_z_y){
  auto transform = transformations::shearing(0,0,0,0,0,1);
  auto p         = Point(2,3,4);

  auto transformedVector = transform * p;

  EXPECT_EQ(transformedVector, Point(2,3,7));
}

/* =========== Chained transformations =========== */
TEST(transformations_tests, transformations_applied_in_sequence){
  auto p = Point(1,0,1);
  auto A = transformations::rotation_x(std::numbers::pi/2);
  auto B = transformations::scaling(5,5,5);
  auto C = transformations::translation(10,5,7);

  auto p2 = A * p;
  EXPECT_EQ(p2, Point(1,-1,0));

  auto p3 = B * p2;
  EXPECT_EQ(p3, Point(5,-5,0));

  auto p4 = C * p3;
  EXPECT_EQ(p4, Point(15,0,7));
}

TEST(transformations_tests, chained_transformations){
  auto p = Point(1,0,1);
  auto A = transformations::rotation_x(std::numbers::pi/2);
  auto B = transformations::scaling(5,5,5);
  auto C = transformations::translation(10,5,7);

  // applied in reverse order to match the order above because
  // matrix multiplication is not commutative
  auto T = C * B * A;
  auto p2 = T * p;
  EXPECT_EQ(p2, Point(15,0,7));
}