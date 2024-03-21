#include <gtest/gtest.h>
#include <memory>

#include "Intersections.hpp"
#include "Sphere.hpp"
#include "Tuple.hpp"

using namespace raytracer;
using namespace utility;

Tuple testVector = Vector(0, 0, 0);
/* =========== Creation Test =========== */
TEST(intersection_tests, encapsulateTAndObject){
  const auto s = geometry::Sphere();
  const auto i = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  3.5);

  EXPECT_FLOAT_EQ(i.dist, 3.5);
  EXPECT_EQ(i.normalVector, testVector);
}

/* =========== Aggregation Test =========== */
TEST(intersection_tests, AggregationOfIntersections){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  1);
  const auto i2 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  2);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(xs.size(), 2);
  EXPECT_TRUE(*xs[0].material == s.material_);
  EXPECT_TRUE(*xs[1].material == s.material_);
}

/* =========== Hit Test =========== */
TEST(hit_tests, AllIntersectionsPositiveT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  1);
  const auto i2 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  2);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(*hit(xs), i1);
}

TEST(hit_tests, SomeIntersectionsNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  -1);
  const auto i2 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),   1);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(*hit(xs), i2);
}

TEST(hit_tests, AllIntersectionsNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  -2);
  const auto i2 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  -1);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(hit(xs), std::nullopt);
}

TEST(hit_tests, ReturnLowestNonNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  5);
  const auto i2 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  7);
  const auto i3 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  -3);
  const auto i4 = geometry::Intersection(testVector,  &s.material_, &s.inverseTransform(),  2);
  const auto xs = geometry::intersections(i1, i2, i3, i4);

  EXPECT_EQ(*hit(xs), i4);
}

/* =========== Prepare Computations Test =========== */
TEST(prepare_computations_tests, PrepareComputations){
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto i = geometry::Intersection(s.normalAt(r.position(4)),  &s.material_, &s.inverseTransform(),  4);
  const auto comps = geometry::prepareComputations(i,r);

  EXPECT_EQ(comps.intersection.dist, i.dist);
  EXPECT_TRUE(*comps.intersection.material == *i.material);
  EXPECT_EQ(comps.point, Point(0, 0, -1));
  EXPECT_EQ(comps.eyeVector, Vector(0, 0, -1));
  EXPECT_EQ(comps.intersection.normalVector, Vector(0, 0, -1));
}

TEST(prepare_computations_tests, PrepareComputationsIntersectionOutside){
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto i = s.intersect(r);
  const auto comps = geometry::prepareComputations(i.at(0),r);

  EXPECT_FALSE(comps.inside);
}

TEST(prepare_computations_tests, PrepareComputationsIntersectionInside){
  const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto i = geometry::Intersection(s.normalAt(r.position(1)),  &s.material_, &s.inverseTransform(),  1);
  const auto comps = geometry::prepareComputations(i,r);

  EXPECT_EQ(comps.point, Point(0, 0, 1));
  EXPECT_EQ(comps.eyeVector, Vector(0, 0, -1));
  EXPECT_TRUE(comps.inside);
  EXPECT_EQ(comps.intersection.normalVector, Vector(0, 0, -1));
}