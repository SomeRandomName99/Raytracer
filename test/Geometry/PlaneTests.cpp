#include <gtest/gtest.h>

#include "Plane.hpp"
#include "Intersections.hpp"
#include "Shape.hpp"

using namespace raytracer;
using namespace utility;

TEST(plane_tests, normalOfPlaneIsConstantEverywhere) {
  auto p = geometry::Plane();
  const auto n1 = p.normalAt(Point(0, 0, 0));
  const auto n2 = p.normalAt(Point(10, 0, -10));
  const auto n3 = p.normalAt(Point(-5, 0, 150));

  EXPECT_EQ(n1, Vector(0, 1, 0));
  EXPECT_EQ(n2, Vector(0, 1, 0));
  EXPECT_EQ(n3, Vector(0, 1, 0));
}

TEST(plane_tests, intersectWithRayParallelToPlane) {
  auto p = geometry::Plane();
  const auto r = Ray(Point(0, 10, 0), Vector(0, 0, 1));
  const auto xs = p.intersect(r);

  EXPECT_EQ(0, xs.size());
}

TEST(plane_tests, intersectWithCoplanarRay) {
  auto p = geometry::Plane();
  const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  const auto xs = p.intersect(r);

  EXPECT_EQ(0, xs.size());
}

TEST(plane_tests, intersectWithRayFromAbove) {
  auto p = geometry::Plane();
  const auto r = Ray(Point(0, 1, 0), Vector(0, -1, 0));
  const auto xs = p.intersect(r);

  EXPECT_EQ(1, xs.size());
  EXPECT_FLOAT_EQ(1, xs.at(0).dist);
  EXPECT_TRUE(p.material() == xs.at(0).object->material());
}

TEST(plane_tests, intersectWithRayFromBelow) {
  auto p = geometry::Plane();
  const auto r = Ray(Point(0, -1, 0), Vector(0, 1, 0));
  const auto xs = p.intersect(r);

  EXPECT_EQ(1, xs.size());
  EXPECT_FLOAT_EQ(1, xs.at(0).dist);
  EXPECT_TRUE(p.material() == xs.at(0).object->material());
}