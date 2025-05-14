#include <gtest/gtest.h>
#include <iostream>

#include "AABB.hpp"

using namespace raytracer;
using namespace utility;

TEST(AABBTests, AABBCreation) {
  const auto aabb = AABB(Point(1, 2, 3), Point(4, 5, 6));
  EXPECT_EQ(aabb.min.x(), 1);
  EXPECT_EQ(aabb.min.y(), 2);
  EXPECT_EQ(aabb.min.z(), 3);
  EXPECT_EQ(aabb.max.x(), 4);
  EXPECT_EQ(aabb.max.y(), 5);
  EXPECT_EQ(aabb.max.z(), 6);
}

TEST(AABBTests, AABBCreationDefaultConstructor) {
  const auto aabb = AABB();
  EXPECT_EQ(aabb.min.x(), 0);
  EXPECT_EQ(aabb.min.y(), 0);
  EXPECT_EQ(aabb.min.z(), 0);
  EXPECT_EQ(aabb.max.x(), 0);
  EXPECT_EQ(aabb.max.y(), 0);
  EXPECT_EQ(aabb.max.z(), 0);
}

TEST(AABBTests, AABBIntersection) {
  const auto aabb = AABB(Point(1, 2, 3), Point(4, 5, 6));
  const auto ray = Ray(Point(0, 0, 0), Vector(1, 1, 1));
  
  EXPECT_TRUE(aabb.intersect(ray));
}
