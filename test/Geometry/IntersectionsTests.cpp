#include <gtest/gtest.h>

#include <memory>

#include "Intersections.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Tuple.hpp"
#include "Shape.hpp"

using namespace raytracer;
using namespace utility;

Tuple testVector = Vector(0, 0, 0);
/* =========== Creation Test =========== */
TEST(intersection_tests, encapsulateTAndObject){
  const auto s = geometry::Sphere();
  const auto i = geometry::Intersection(&s, 3.5);

  EXPECT_FLOAT_EQ(i.dist, 3.5);
  EXPECT_EQ(i.object, &s);
}

/* =========== Aggregation Test =========== */
TEST(intersection_tests, AggregationOfIntersections){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(&s, 1);
  const auto i2 = geometry::Intersection(&s, 2);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(xs.size(), 2);
  EXPECT_TRUE(xs[0].object->material() == s.material_);
  EXPECT_TRUE(xs[1].object->material() == s.material_);
}

/* =========== Hit Test =========== */
TEST(hit_tests, AllIntersectionsPositiveT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(&s, 1);
  const auto i2 = geometry::Intersection(&s, 2);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(*hit(xs), i1);
}

TEST(hit_tests, SomeIntersectionsNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(&s, -1);
  const auto i2 = geometry::Intersection(&s,  1);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(*hit(xs), i2);
}

TEST(hit_tests, AllIntersectionsNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(&s, -2);
  const auto i2 = geometry::Intersection(&s, -1);
  const auto xs = geometry::intersections(i1, i2);

  EXPECT_EQ(hit(xs), std::nullopt);
}

TEST(hit_tests, ReturnLowestNonNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(&s, 5);
  const auto i2 = geometry::Intersection(&s, 7);
  const auto i3 = geometry::Intersection(&s, -3);
  const auto i4 = geometry::Intersection(&s, 2);
  const auto xs = geometry::intersections(i1, i2, i3, i4);

  EXPECT_EQ(*hit(xs), i4);
}

/* =========== Prepare Computations Test =========== */
TEST(prepare_computations_tests, PrepareComputations){
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto i = geometry::Intersection(&s,   4);
  const auto comps = geometry::prepareComputations(i,r);

  EXPECT_EQ(comps.intersection.dist, i.dist);
  EXPECT_TRUE(comps.intersection.object->material() == i.object->material());
  EXPECT_EQ(comps.point, Point(0, 0, -1));
  EXPECT_EQ(comps.eyeVector, Vector(0, 0, -1));
  EXPECT_EQ(comps.normalVector, Vector(0, 0, -1));
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
  const auto i = geometry::Intersection(&s,   1);
  const auto comps = geometry::prepareComputations(i,r);

  EXPECT_EQ(comps.point, Point(0, 0, 1));
  EXPECT_EQ(comps.eyeVector, Vector(0, 0, -1));
  EXPECT_TRUE(comps.inside);
  EXPECT_EQ(comps.normalVector, Vector(0, 0, -1));
}

TEST(prepare_computations_tests, PrepareComputationsReflectionVector){
  const auto r = Ray(Point(0, 0, -1), Vector(0, -sqrt(2)/2, sqrt(2)/2));
  const auto s = geometry::Plane();
  const auto i = geometry::Intersection(&s,   sqrt(2));
  const auto comps = geometry::prepareComputations(i,r);

  EXPECT_EQ(comps.reflectVector, Vector(0, sqrt(2)/2, sqrt(2)/2));
}