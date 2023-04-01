#include <gtest/gtest.h>

#include "include/ray.h"
#include "include/sphere.h"

TEST(sphere_test, raySphereInterSectionAtTwoPoints) {
  const auto r = Ray::Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0), 4.0);
  EXPECT_FLOAT_EQ(xs.at(1), 6.0);
}

TEST(sphere_test, raySphereInterSectionAtATangent) {
  const auto r = Ray::Ray(Point(0, 1, -5), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0), 5.0);
  EXPECT_FLOAT_EQ(xs.at(1), 5.0);
}

TEST(sphere_test, rayMissesSphere) {
  const auto r = Ray::Ray(Point(0, 2, -5), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(0, xs.size());
}

TEST(sphere_test, rayOriginatesInsideSphere) {
  const auto r = Ray::Ray(Point(0, 0, 0), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0), -1.0);
  EXPECT_FLOAT_EQ(xs.at(1),  1.0);
}

TEST(sphere_test, sphereBehindRay) {
  const auto r = Ray::Ray(Point(0, 0, 5), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0), -6.0);
  EXPECT_FLOAT_EQ(xs.at(1), -4.0);
}