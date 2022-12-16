#include <gtest/gtest.h>
#include <numbers>

#include "include/transformations.h"
#include "include/tuple.h"

/* =========== Translation =========== */
TEST(transformations_test, multiplication_translation_matrix){
  auto transform = translation(5,-3,2);
  auto p         = Point(-3,4,5);

  auto transformedPoint = transform * p;

  EXPECT_EQ(transformedPoint, Point(2,1,7));
}

TEST(transformations_test, translation_multiplication_inverse_translation_matrix){
  auto transform = translation(5,-3, 2);
  auto transform_inverse = inverse(transform);
  auto p         = Point(-3,4,5);

  auto transformedPoint = transform_inverse * p;

  EXPECT_EQ(transformedPoint, Point(-8,7,3));
}

TEST(transformations_test, multiplication_does_not_affect_vectors){
  auto transform = translation(5,-3,2);
  auto v         = Vector(-3,4,5);

  auto transformedVector = transform * v;

  EXPECT_EQ(transformedVector, Vector(-3,4,5));
}

/* =========== Scaling =========== */
TEST(transformations_test, scaling_point){
  auto transform = scaling(2,3,4);
  auto p         = Point(-4,6,8);

  auto transformedPoint = transform * p;

  EXPECT_EQ(transformedPoint, Point(-8,18,32));
}

TEST(transformations_test, scaling_vector){
  auto transform = scaling(2,3,4);
  auto v         = Vector(-4,6,8);

  auto transformedVector = transform * v;

  EXPECT_EQ(transformedVector, Vector(-8,18,32));
}

TEST(transformations_test, scaling_multiplication_inverse_translation_matrix){
  auto transform         = scaling(2,3,4);
  auto transform_inverse = inverse(transform);
  auto v                 = Vector(-4,6,8);

  auto transformedVector = transform_inverse * v;

  EXPECT_EQ(transformedVector, Vector(-2,2,2));
}

TEST(transformations_test, reflections_backwards){
  auto transform         = scaling(-1,1,1);
  auto p                 = Point(2,3,4);

  auto transformedVector = transform * p;

  EXPECT_EQ(transformedVector, Point(-2,3,4));
}

/* =========== Rotation =========== */
TEST(transformations_test, rotate_x_axis){
  auto p                         = Point(0,1,0);
  auto halfQuarterTransform      = rotation_x(std::numbers::pi/4);
  auto fullQuarterTransform      = rotation_x(std::numbers::pi/2);


  auto halfQuarter = halfQuarterTransform * p;
  auto fullQuarter = fullQuarterTransform * p;


  EXPECT_EQ(halfQuarter, Point(0,std::sqrt(2)/2,std::sqrt(2)/2));
  EXPECT_EQ(fullQuarter, Point(0,0,1));
}

TEST(transformations_test, rotate_y_axis){
  auto p                         = Point(0,0,1);
  auto halfQuarterTransform      = rotation_y(std::numbers::pi/4);
  auto fullQuarterTransform      = rotation_y(std::numbers::pi/2);


  auto halfQuarter = halfQuarterTransform * p;
  auto fullQuarter = fullQuarterTransform * p;


  EXPECT_EQ(halfQuarter, Point(std::sqrt(2)/2,0,std::sqrt(2)/2));
  EXPECT_EQ(fullQuarter, Point(1,0,0));
}

TEST(transformations_test, rotate_z_axis){
  auto p                         = Point(0,1,0);
  auto halfQuarterTransform      = rotation_z(std::numbers::pi/4);
  auto fullQuarterTransform      = rotation_z(std::numbers::pi/2);


  auto halfQuarter = halfQuarterTransform * p;
  auto fullQuarter = fullQuarterTransform * p;


  EXPECT_EQ(halfQuarter, Point(-std::sqrt(2)/2,std::sqrt(2)/2,0));
  EXPECT_EQ(fullQuarter, Point(-1,0,0));
}