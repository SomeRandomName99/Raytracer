#include <gtest/gtest.h>

#include "include/ray.h"
#include "include/sphere.h"
#include "include/intersections.h"

/* =========== Intersection tests =========== */
TEST(sphere_tests, raySphereInterSectionAtTwoPoints) {
  const auto r = Ray::Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).t, 4.0);
  EXPECT_FLOAT_EQ(xs.at(1).t, 6.0);
}

TEST(sphere_tests, raySphereInterSectionAtATangent) {
  const auto r = Ray::Ray(Point(0, 1, -5), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).t, 5.0);
  EXPECT_FLOAT_EQ(xs.at(1).t, 5.0);
}

TEST(sphere_tests, rayMissesSphere) {
  const auto r = Ray::Ray(Point(0, 2, -5), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = intersect(s,r);

  EXPECT_EQ(0, xs.size());
}

TEST(sphere_tests, rayOriginatesInsideSphere) {
  const auto r = Ray::Ray(Point(0, 0, 0), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).t, -1.0);
  EXPECT_FLOAT_EQ(xs.at(1).t,  1.0);
}

TEST(sphere_tests, sphereBehindRay) {
  const auto r = Ray::Ray(Point(0, 0, 5), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).t, -6.0);
  EXPECT_FLOAT_EQ(xs.at(1).t, -4.0);
}

TEST(sphere_tests, setObjectOnIntersection) {
  const auto r = Ray::Ray(Point(0, 0, 5), Vector(0, 0, 1));
  const auto s = Sphere();
  const auto xs = intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_EQ(xs.at(0).object, s);
  EXPECT_EQ(xs.at(1).object, s);
}