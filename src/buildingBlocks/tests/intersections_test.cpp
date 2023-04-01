#include <gtest/gtest.h>

#include "include/intersections.h"
#include "include/sphere.h"

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