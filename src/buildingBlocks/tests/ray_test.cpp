#include <gtest/gtest.h>

#include "include/transformations.h"
#include "include/ray.h"

using namespace Ray;

TEST(ray_tests, ray_creation) {
  const auto origin = Point(1, 2, 3);
  const auto direction = Vector(4, 5, 6);
  const Ray::Ray ray{origin, direction};

  EXPECT_EQ(origin, ray.origin);
  EXPECT_EQ(direction, ray.direction);
}

TEST(ray_tests, distance_computation) {
  const Ray::Ray r{Point(2, 3, 4), Vector(1, 0, 0)};

  EXPECT_EQ(Ray::position(r, 0),   Point(2, 3, 4));
  EXPECT_EQ(Ray::position(r, 1),   Point(3, 3, 4));
  EXPECT_EQ(Ray::position(r, -1),  Point(1, 3, 4));
  EXPECT_EQ(Ray::position(r, 2.5), Point(4.5, 3, 4));
}

TEST(ray_tests, TranslateRay) {
  const Ray::Ray r{Point(1, 2, 3), Vector(0, 1, 0)};
  const auto m = transformations::translation(3, 4, 5);
  const auto r2 = Ray::transform(r, m);

  EXPECT_EQ(r2.origin,      Point(4, 6, 8));
  EXPECT_EQ(r2.direction,   Vector(0, 1, 0));
}

TEST(ray_tests, ScaleRay) {
  const Ray::Ray r{Point(1, 2, 3), Vector(0, 1, 0)};
  const auto m = transformations::scaling(2, 3, 4);
  const auto r2 = Ray::transform(r, m);

  EXPECT_EQ(r2.origin,      Point(2, 6, 12));
  EXPECT_EQ(r2.direction,   Vector(0, 3, 0));
}