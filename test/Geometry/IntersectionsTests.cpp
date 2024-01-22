#include <gtest/gtest.h>
#include <memory>

#include "Intersections.hpp"
#include "Sphere.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Creation Test =========== */
TEST(intersection_tests, encapsulateTAndObject){
  const auto s = geometry::Sphere();
  const auto i = geometry::Intersection(3.5, std::make_shared<geometry::Sphere>(s));

  EXPECT_FLOAT_EQ(i.t_, 3.5);
  EXPECT_TRUE(*i.object_ == s);
}

/* =========== Aggregation Test =========== */
TEST(intersection_tests, AggregationOfIntersections){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(1, std::make_shared<geometry::Sphere>(s));
  const auto i2 = geometry::Intersection(2, std::make_shared<geometry::Sphere>(s));
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(xs.size(), 2);
  EXPECT_TRUE(*xs[0].object_ == s);
  EXPECT_TRUE(*xs[1].object_ == s);
}

/* =========== Hit Test =========== */
TEST(hit_tests, AllIntersectionsPositiveT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(1, std::make_shared<geometry::Sphere>(s));
  const auto i2 = geometry::Intersection(2, std::make_shared<geometry::Sphere>(s));
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(*hit(xs), i1);
}

TEST(hit_tests, SomeIntersectionsNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(-1, std::make_shared<geometry::Sphere>(s));
  const auto i2 = geometry::Intersection( 1, std::make_shared<geometry::Sphere>(s));
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(*hit(xs), i2);
}

TEST(hit_tests, AllIntersectionsNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(-2, std::make_shared<geometry::Sphere>(s));
  const auto i2 = geometry::Intersection(-1, std::make_shared<geometry::Sphere>(s));
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(hit(xs), std::nullopt);
}

TEST(hit_tests, ReturnLowestNonNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(5, std::make_shared<geometry::Sphere>(s));
  const auto i2 = geometry::Intersection(7, std::make_shared<geometry::Sphere>(s));
  const auto i3 = geometry::Intersection(-3, std::make_shared<geometry::Sphere>(s));
  const auto i4 = geometry::Intersection(2, std::make_shared<geometry::Sphere>(s));
  const auto xs = geometry::intersections(i1, i2, i3, i4);

  EXPECT_EQ(*hit(xs), i4);
}