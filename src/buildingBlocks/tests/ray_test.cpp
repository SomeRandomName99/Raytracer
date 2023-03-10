#include <gtest/gtest.h>

#include "include/ray.h"

using namespace Ray;

TEST(ray_test, ray_creation) {
  const auto origin = Point(1, 2, 3);
  const auto direction = Vector(4, 5, 6);
  const Ray::Ray ray{origin, direction};

  EXPECT_EQ(origin, ray.origin);
  EXPECT_EQ(direction, ray.direction);
}

TEST(ray_test, distance_compoutation) {
  const Ray::Ray r{Point(2, 3, 4), Vector(1, 0, 0)};

  EXPECT_EQ(position(r, 0),   Point(2, 3, 4));
  EXPECT_EQ(position(r, 1),   Point(3, 3, 4));
  EXPECT_EQ(position(r, -1),  Point(1, 3, 4));
  EXPECT_EQ(position(r, 2.5), Point(4.5, 3, 4));
}