#include <gtest/gtest.h>

#include "Transformations.hpp"
#include "Ray.hpp"

using namespace raytracer;
using namespace utility;

TEST(ray_tests, ray_creation) {
  const auto origin = Point(1, 2, 3);
  const auto direction = Vector(4, 5, 6);
  const Ray ray{origin, direction};

  EXPECT_EQ(origin, ray.origin_);
  EXPECT_EQ(direction, ray.direction_);
}

TEST(ray_tests, distance_computation) {
  const Ray r{Point(2, 3, 4), Vector(1, 0, 0)};

  EXPECT_EQ(r.position(0),   Point(2, 3, 4));
  EXPECT_EQ(r.position(1),   Point(3, 3, 4));
  EXPECT_EQ(r.position(-1),  Point(1, 3, 4));
  EXPECT_EQ(r.position(2.5f), Point(4.5f, 3, 4));
}

TEST(ray_tests, TranslateRay) {
  const Ray r{Point(1, 2, 3), Vector(0, 1, 0)};
  const auto m = transformations::translation(3, 4, 5);
  const auto r2 = transform(r, m);

  EXPECT_EQ(r2.origin_,      Point(4, 6, 8));
  EXPECT_EQ(r2.direction_,   Vector(0, 1, 0));
}

TEST(ray_tests, ScaleRay) {
  const Ray r{Point(1, 2, 3), Vector(0, 1, 0)};
  const auto m  = transformations::scaling(2, 3, 4);
  const auto r2 = transform(r, m);

  EXPECT_EQ(r2.origin_,      Point(2, 6, 12));
  EXPECT_EQ(r2.direction_,   Vector(0, 3, 0));
}