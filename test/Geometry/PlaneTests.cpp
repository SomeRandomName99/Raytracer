#include <gtest/gtest.h>

#include "Plane.hpp"
#include "Intersections.hpp"
#include "Shape.hpp"
#include "Arena.hpp"

using namespace raytracer;
using namespace utility;

class PlaneTest : public ::testing::Test {
protected:
  Arena<geometry::Intersection> xs;
  
  PlaneTest() : xs(MB(1)) {}
  
  void SetUp() override {
    // Clear arena before each test to ensure clean state
    xs.clear();
  }
  
  void TearDown() override {}
};

TEST(plane_tests, normalOfPlaneIsConstantEverywhere) {
  auto p = geometry::Plane();
  const auto n1 = p.normalAt(Point(0, 0, 0));
  const auto n2 = p.normalAt(Point(10, 0, -10));
  const auto n3 = p.normalAt(Point(-5, 0, 150));

  EXPECT_EQ(n1, Vector(0, 1, 0));
  EXPECT_EQ(n2, Vector(0, 1, 0));
  EXPECT_EQ(n3, Vector(0, 1, 0));
}

TEST_F(PlaneTest, intersectWithRayParallelToPlane) {
  auto p = geometry::Plane();
  const auto r = Ray(Point(0, 10, 0), Vector(0, 0, 1));
  p.intersect(r, xs);

  EXPECT_EQ(0, xs.size);
}

TEST_F(PlaneTest, intersectWithCoplanarRay) {
  auto p = geometry::Plane();
  const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  p.intersect(r, xs);

  EXPECT_EQ(0, xs.size);
}

TEST_F(PlaneTest, intersectWithRayFromAbove) {
  auto p = geometry::Plane();
  const auto r = Ray(Point(0, 1, 0), Vector(0, -1, 0));
  p.intersect(r, xs);

  EXPECT_EQ(1, xs.size);
  EXPECT_FLOAT_EQ(1, xs[0].dist);
  EXPECT_TRUE(p.material() == xs[0].object->material());
}

TEST_F(PlaneTest, intersectWithRayFromBelow) {
  auto p = geometry::Plane();
  const auto r = Ray(Point(0, -1, 0), Vector(0, 1, 0));
  p.intersect(r, xs);

  EXPECT_EQ(1, xs.size);
  EXPECT_FLOAT_EQ(1, xs[0].dist);
  EXPECT_TRUE(p.material() == xs[0].object->material());
}