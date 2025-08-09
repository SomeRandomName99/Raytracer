#include <gtest/gtest.h>

#include <memory>

#include "Intersections.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Tuple.hpp"
#include "Shape.hpp"
#include "Arena.hpp"

using namespace raytracer;
using namespace utility;

class IntersectionTest : public ::testing::Test {
protected:
  Arena<geometry::Intersection> xs;
  
  IntersectionTest() : xs(MB(1)) {}
  
  void SetUp() override {
    // Clear arena before each test to ensure clean state
    xs.clear();
  }
  
  void TearDown() override {}
};

Tuple testVector = Vector(0, 0, 0);
/* =========== Creation Test =========== */
TEST(intersection_tests, encapsulateTAndObject){
  const auto s = geometry::Sphere();
  const auto i = geometry::Intersection(&s, 3.5);

  EXPECT_FLOAT_EQ(i.dist, 3.5);
  EXPECT_EQ(i.object, &s);
}

/* =========== Aggregation Test =========== */
TEST_F(IntersectionTest, AggregationOfIntersections){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(&s, 1);
  const auto i2 = geometry::Intersection(&s, 2);
  xs.pushBack(i1);
  xs.pushBack(i2);

  EXPECT_EQ(xs.size, 2);
  EXPECT_TRUE(xs[0].object->material() == s.material());
  EXPECT_TRUE(xs[1].object->material() == s.material());
}

/* =========== Hit Test =========== */
TEST_F(IntersectionTest, AllIntersectionsPositiveT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(&s, 1);
  const auto i2 = geometry::Intersection(&s, 2);
  xs.pushBack(i1);
  xs.pushBack(i2);

  EXPECT_EQ(*hit(xs), i1);
}

TEST_F(IntersectionTest, SomeIntersectionsNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(&s, -1);
  const auto i2 = geometry::Intersection(&s,  1);
  xs.pushBack(i1);
  xs.pushBack(i2);

  EXPECT_EQ(*hit(xs), i2);
}

TEST_F(IntersectionTest, AllIntersectionsNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(&s, -2);
  const auto i2 = geometry::Intersection(&s, -1);
  xs.pushBack(i1);
  xs.pushBack(i2);

  EXPECT_EQ(hit(xs), std::nullopt);
}

TEST_F(IntersectionTest, ReturnLowestNonNegativeT){
  const auto s = geometry::Sphere();
  const auto i1 = geometry::Intersection(&s, 5);
  const auto i2 = geometry::Intersection(&s, 7);
  const auto i3 = geometry::Intersection(&s, -3);
  const auto i4 = geometry::Intersection(&s, 2);
  xs.pushBack(i1);
  xs.pushBack(i2);
  xs.pushBack(i3);
  xs.pushBack(i4);

  EXPECT_EQ(*hit(xs), i4);
}

/* =========== Prepare Computations Test =========== */
TEST_F(IntersectionTest, PrepareComputations){
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto i = geometry::Intersection(&s, 4);
  xs.pushBack(i);
  const auto comps = geometry::prepareComputations(i, r, xs);

  EXPECT_EQ(comps.intersection.dist, i.dist);
  EXPECT_TRUE(comps.intersection.object->material() == i.object->material());
  EXPECT_EQ(comps.point, Point(0, 0, -1));
  EXPECT_EQ(comps.eyeVector, Vector(0, 0, -1));
  EXPECT_EQ(comps.normalVector, Vector(0, 0, -1));
}

TEST_F(IntersectionTest, PrepareComputationsIntersectionOutside){
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  s.intersect(r, xs);
  const auto comps = geometry::prepareComputations(xs[0], r, xs);

  EXPECT_FALSE(comps.inside);
}

TEST_F(IntersectionTest, PrepareComputationsInterIsectionInside){
  const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto i = geometry::Intersection(&s, 1);
  xs.pushBack(i);  // Add to arena for the function call
  const auto comps = geometry::prepareComputations(i, r, xs);

  EXPECT_EQ(comps.point, Point(0, 0, 1));
  EXPECT_EQ(comps.eyeVector, Vector(0, 0, -1));
  EXPECT_TRUE(comps.inside);
  EXPECT_EQ(comps.normalVector, Vector(0, 0, -1));
}

TEST_F(IntersectionTest, PrepareComputationsReflectionVector){
  const auto r = Ray(Point(0, 0, -1), Vector(0, -sqrt(2)/2, sqrt(2)/2));
  const auto s = geometry::Plane();
  const auto i = geometry::Intersection(&s, sqrt(2));
  xs.pushBack(i);  // Add to arena for the function call
  const auto comps = geometry::prepareComputations(i, r, xs);

  EXPECT_EQ(comps.reflectVector, Vector(0, sqrt(2)/2, sqrt(2)/2));
}

/* =========== Refraction Test =========== */
TEST_F(IntersectionTest, FindingRefractionIndicesAtVariousIntersections){
  auto a = geometry::glassSphere();
  auto b = geometry::glassSphere();
  auto c = geometry::glassSphere();

  a->setTransform(transformations::scaling(2, 2, 2));
  a->material().setRefractiveIndex(1.5);
  b->setTransform(transformations::translation(0, 0, -0.25));
  b->material().setRefractiveIndex(2.0);
  c->setTransform(transformations::translation(0, 0, 0.25));
  c->material().setRefractiveIndex(2.5);

  const auto r = Ray(Point(0, 0, -4), Vector(0, 0, 1));
  
  // Manually populate the Arena with intersections
  xs.pushBack(geometry::Intersection(a.get(), 2));
  xs.pushBack(geometry::Intersection(b.get(), 2.75));
  xs.pushBack(geometry::Intersection(c.get(), 3.25));
  xs.pushBack(geometry::Intersection(b.get(), 4.75));
  xs.pushBack(geometry::Intersection(c.get(), 5.25));
  xs.pushBack(geometry::Intersection(a.get(), 6));

  const auto expected = std::vector<double>{1.0, 1.5, 2.0, 2.5, 2.5, 1.5};
  for (size_t i = 0; i < xs.size; i++){
    const auto comps = geometry::prepareComputations(xs[i], r, xs);
    EXPECT_FLOAT_EQ(comps.n1, expected[i]);
    EXPECT_FLOAT_EQ(comps.n2, expected[(i+1)%expected.size()]);
  }
}

TEST_F(IntersectionTest, UnderPointOffset){
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto s = geometry::glassSphere();
  s->setTransform(transformations::translation(0, 0, 1));
  const auto i = geometry::Intersection(s.get(), 5);
  xs.pushBack(i);
  const auto comps = geometry::prepareComputations(i, r, xs);

  EXPECT_TRUE(comps.underPoint.z() > SHADOW_OFFSET/2);
  EXPECT_TRUE(comps.point.z() < comps.underPoint.z());
}

// =========== Schlick Approximation Test ===========
TEST_F(IntersectionTest, SchlickTotalInternalReflection){
  const auto s = geometry::glassSphere();
  const auto r = Ray(Point(0, 0, sqrt(2)/2), Vector(0, 1, 0));
  xs.pushBack(geometry::Intersection(s.get(), -sqrt(2)/2));
  xs.pushBack(geometry::Intersection(s.get(), sqrt(2)/2));
  const auto comps = geometry::prepareComputations(xs[1], r, xs);

  const auto reflectance = geometry::schlick(comps);
  EXPECT_FLOAT_EQ(reflectance, 1.0);
}

TEST_F(IntersectionTest, SchlickPerpendicularViewingAngle){
  const auto s = geometry::glassSphere();
  const auto r = Ray(Point(0, 0, 0), Vector(0, 1, 0));
  xs.pushBack(geometry::Intersection(s.get(), -1));
  xs.pushBack(geometry::Intersection(s.get(), 1));
  const auto comps = geometry::prepareComputations(xs[1], r, xs);

  const auto reflectance = geometry::schlick(comps);
  EXPECT_FLOAT_EQ(reflectance, 0.04);
}

TEST_F(IntersectionTest, SchlickSmallAngleAndN2GreaterThanN1){
  const auto s = geometry::glassSphere();
  const auto r = Ray(Point(0, 0.99, -2), Vector(0, 0, 1));
  xs.pushBack(geometry::Intersection(s.get(), 1.8589));
  const auto comps = geometry::prepareComputations(xs[0], r, xs);

  const auto reflectance = geometry::schlick(comps);
  EXPECT_FLOAT_EQ(reflectance, 0.4887308);
}