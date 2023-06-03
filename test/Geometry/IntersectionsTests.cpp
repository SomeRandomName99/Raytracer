#include <gtest/gtest.h>

#include "Intersections.hpp"
#include "Sphere.hpp"

/* =========== Creation Test =========== */
TEST(intersection_tests, encapsulateTAndObject){
  const auto s = Sphere();
  const auto i = Intersection(3.5, s);

  EXPECT_FLOAT_EQ(i.t, 3.5);
  EXPECT_EQ(i.object, s);
}

/* =========== Aggregation Test =========== */
TEST(intersection_tests, AggregationOfIntersections){
  const auto s = Sphere();
  const auto i1 = Intersection(1, s);
  const auto i2 = Intersection(2, s);
  const auto xs = intersections(i1, i2);

  EXPECT_EQ(xs.size(), 2);
  EXPECT_EQ(xs[0].object, s);
  EXPECT_EQ(xs[1].object, s);
}

/* =========== Hit Test =========== */
TEST(hit_tests, AllIntersectionsPositiveT){
  const auto s = Sphere();
  const auto i1 = Intersection(1, s);
  const auto i2 = Intersection(2, s);
  const auto xs = intersections(i1, i2);

  EXPECT_EQ(*hit(xs), i1);
}

TEST(hit_tests, SomeIntersectionsNegativeT){
  const auto s = Sphere();
  const auto i1 = Intersection(-1, s);
  const auto i2 = Intersection( 1, s);
  const auto xs = intersections(i1, i2);

  EXPECT_EQ(*hit(xs), i2);
}

TEST(hit_tests, AllIntersectionsNegativeT){
  const auto s = Sphere();
  const auto i1 = Intersection(-2, s);
  const auto i2 = Intersection(-1, s);
  const auto xs = intersections(i1, i2);

  EXPECT_EQ(hit(xs), std::nullopt);
}

TEST(hit_tests, ReturnLowestNonNegativeT){
  const auto s = Sphere();
  const auto i1 = Intersection(5, s);
  const auto i2 = Intersection(7, s);
  const auto i3 = Intersection(-3, s);
  const auto i4 = Intersection(2, s);
  const auto xs = intersections(i1, i2, i3, i4);

  EXPECT_EQ(*hit(xs), i4);
}