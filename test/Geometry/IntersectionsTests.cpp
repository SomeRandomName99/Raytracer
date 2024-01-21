#include <gtest/gtest.h>

#include "Intersections.hpp"
#include "Sphere.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Creation Test =========== */
TEST(intersection_tests, encapsulateTAndObject){
  const auto s = geometry::Sphere();
  const auto i = geometry::Intersection(3.5, s);

  EXPECT_FLOAT_EQ(i.t, 3.5);
  EXPECT_EQ(i.object, s);
}

/* =========== Aggregation Test =========== */
TEST(intersection_tests, AggregationOfIntersections){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(1, s);
  const auto i2 = geometry::Intersection(2, s);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(xs.size(), 2);
  EXPECT_EQ(xs[0].object, s);
  EXPECT_EQ(xs[1].object, s);
}

/* =========== Hit Test =========== */
TEST(hit_tests, AllIntersectionsPositiveT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(1, s);
  const auto i2 = geometry::Intersection(2, s);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(*hit(xs), i1);
}

TEST(hit_tests, SomeIntersectionsNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(-1, s);
  const auto i2 = geometry::Intersection( 1, s);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(*hit(xs), i2);
}

TEST(hit_tests, AllIntersectionsNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(-2, s);
  const auto i2 = geometry::Intersection(-1, s);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(hit(xs), std::nullopt);
}

TEST(hit_tests, ReturnLowestNonNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(5, s);
  const auto i2 = geometry::Intersection(7, s);
  const auto i3 = geometry::Intersection(-3, s);
  const auto i4 = geometry::Intersection(2, s);
  const auto xs = geometry::intersections(i1, i2, i3, i4);

  EXPECT_EQ(*hit(xs), i4);
}