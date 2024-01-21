#include <gtest/gtest.h>
#include <numbers>
#include <cmath>

#include "Ray.hpp"
#include "Sphere.hpp"
#include "Intersections.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Intersection tests =========== */
TEST(sphere_tests, raySphereInterSectionAtTwoPoints) {
  const auto r = Ray::Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto xs = geometry::intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).t, 4.0);
  EXPECT_FLOAT_EQ(xs.at(1).t, 6.0);
}

TEST(sphere_tests, raySphereInterSectionAtATangent) {
  const auto r = Ray::Ray(Point(0, 1, -5), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto xs = geometry::intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).t, 5.0);
  EXPECT_FLOAT_EQ(xs.at(1).t, 5.0);
}

TEST(sphere_tests, rayMissesSphere) {
  const auto r = Ray::Ray(Point(0, 2, -5), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto xs = geometry::intersect(s,r);

  EXPECT_EQ(0, xs.size());
}

TEST(sphere_tests, rayOriginatesInsideSphere) {
  const auto r = Ray::Ray(Point(0, 0, 0), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto xs = geometry::intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).t, -1.0);
  EXPECT_FLOAT_EQ(xs.at(1).t,  1.0);
}

TEST(sphere_tests, sphereBehindRay) {
  const auto r = Ray::Ray(Point(0, 0, 5), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto xs = geometry::intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).t, -6.0);
  EXPECT_FLOAT_EQ(xs.at(1).t, -4.0);
}

TEST(sphere_tests, setObjectOnIntersection) {
  const auto r = Ray::Ray(Point(0, 0, 5), Vector(0, 0, 1));
  const auto s = geometry::Sphere();
  const auto xs = geometry::intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_EQ(xs.at(0).object, s);
  EXPECT_EQ(xs.at(1).object, s);
}

/* =========== Transform tests =========== */
TEST(sphere_tests, defaultTransformation) {
  const auto s = geometry::Sphere();

  EXPECT_EQ(s.transformation, (Matrix<4,4>::identity()));
}

TEST(sphere_tests, SetTransform) {
  auto s = geometry::Sphere();
  const auto t = transformations::translation(2, 3, 4);

  s.setTransform(t);

  EXPECT_EQ(s.transformation, t);
}

TEST(sphere_tests, IntersectingScaledSphereWithRay) {
  const auto r = Ray::Ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto s = geometry::Sphere();
  s.setTransform(transformations::scaling(2, 2, 2));
  const auto xs = geometry::intersect(s,r);

  EXPECT_EQ(2, xs.size());
  EXPECT_EQ(xs.at(0).t, 3);
  EXPECT_EQ(xs.at(1).t, 7);
}

TEST(sphere_tests, IntersectingTranslatedSphereWithRay) {
  const auto r = Ray::Ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto s = geometry::Sphere();
  s.setTransform(transformations::translation(5, 0, 0));
  const auto xs = geometry::intersect(s,r);

  EXPECT_EQ(0, xs.size());
}

/* =========== Intersection tests =========== */
TEST(sphere_tests, NormalOnSphereXAxis) {
  auto s = geometry::Sphere();

  EXPECT_EQ(s.normalAt(Point(1, 0, 0)), Vector(1, 0, 0));
}

TEST(sphere_tests, NormalOnSphereYAxis) {
  auto s = geometry::Sphere();

  EXPECT_EQ(s.normalAt(Point(0, 1, 0)), Vector(0, 1, 0));
}

TEST(sphere_tests, NormalOnSphereZAxis) {
  auto s = geometry::Sphere();

  EXPECT_EQ(s.normalAt(Point(0, 0, 1)), Vector(0, 0, 1));
}

TEST(sphere_tests, NormalOnSphereNonAxialPoint) {
  auto s = geometry::Sphere();

  EXPECT_EQ(s.normalAt(Point(std::sqrt(3)/3, std::sqrt(3)/3, std::sqrt(3)/3)), 
            Vector(std::sqrt(3)/3, std::sqrt(3)/3, std::sqrt(3)/3));
}

TEST(sphere_tests, NormalOnSphereNormalized) {
  auto s = geometry::Sphere();
  auto n = s.normalAt(Point(std::sqrt(3)/3, std::sqrt(3)/3, std::sqrt(3)/3));

  EXPECT_EQ(n, n.normalize());
}

TEST(sphere_tests, NormalOnATranslatedSphere) {
  auto s = geometry::Sphere();
  s.setTransform(transformations::translation(0, 1, 0));
  auto n = s.normalAt(Point(0, 1+std::sqrt(2)/2, -std::sqrt(2)/2));

  EXPECT_EQ(n, Vector(0, std::sqrt(2)/2, -std::sqrt(2)/2));
}

TEST(sphere_tests, NormalOnATransformedSphere) {
  auto s = geometry::Sphere();
  s.setTransform(transformations::scaling(1, 0.5, 1) * transformations::rotation_z(std::numbers::pi/5));
  auto n = s.normalAt(Point(0, std::sqrt(2)/2, -std::sqrt(2)/2));

  EXPECT_EQ(n, Vector(0, 0.9701425, -0.2425356));
}