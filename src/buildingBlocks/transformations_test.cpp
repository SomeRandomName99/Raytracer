#include <gtest/gtest.h>

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