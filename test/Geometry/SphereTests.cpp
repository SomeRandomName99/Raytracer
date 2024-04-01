#include <gtest/gtest.h>
#include <numbers>
#include <cmath>

#include "Ray.hpp"
#include "Sphere.hpp"
#include "Intersections.hpp"
#include "Material.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Transformations.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Intersection tests =========== */
TEST(sphere_tests, raySphereInterSectionAtTwoPoints) {
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto s = geometry::Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).dist, 4.0);
  EXPECT_FLOAT_EQ(xs.at(1).dist, 6.0);
}

TEST(sphere_tests, raySphereInterSectionAtATangent) {
  const auto r = Ray(Point(0, 1, -5), Vector(0, 0, 1));
  auto s = geometry::Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).dist, 5.0);
  EXPECT_FLOAT_EQ(xs.at(1).dist, 5.0);
}

TEST(sphere_tests, rayMissesSphere) {
  const auto r = Ray(Point(0, 2, -5), Vector(0, 0, 1));
  auto s = geometry::Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(0, xs.size());
}

TEST(sphere_tests, rayOriginatesInsideSphere) {
  const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  auto s = geometry::Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).dist, -1.0);
  EXPECT_FLOAT_EQ(xs.at(1).dist,  1.0);
}

TEST(sphere_tests, sphereBehindRay) {
  const auto r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
  auto s = geometry::Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(xs.at(0).dist, -6.0);
  EXPECT_FLOAT_EQ(xs.at(1).dist, -4.0);
}

TEST(sphere_tests, setMaterialOnIntersection) {
  const auto r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
  auto s = geometry::Sphere();
  const auto xs = s.intersect(r);

  EXPECT_EQ(2, xs.size());
  EXPECT_TRUE(xs.at(0).object->material() == s.material_);
  EXPECT_TRUE(xs.at(1).object->material() == s.material_);
}

TEST(sphere_tests, IntersectingScaledSphereWithRay) {
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto s = geometry::Sphere();
  s.setTransform(transformations::scaling(2, 2, 2));
  const auto xs = s.intersect(r);

  EXPECT_EQ(2, xs.size());
  EXPECT_EQ(xs.at(0).dist, 3);
  EXPECT_EQ(xs.at(1).dist, 7);
}

TEST(sphere_tests, IntersectingTranslatedSphereWithRay) {
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto s = geometry::Sphere();
  s.setTransform(transformations::translation(5, 0, 0));
  const auto xs = s.intersect(r);

  EXPECT_EQ(0, xs.size());
}

/* =========== Normal tests =========== */
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

TEST(sphere_tests, NormalOnTransformedSphere){
  auto s = geometry::Sphere();
  s.setTransform(transformations::scaling(1, 0.5, 1) * transformations::rotation_z(std::numbers::pi/5));
  const auto n = s.normalAt(Point(0, std::sqrt(2)/2, -std::sqrt(2)/2));

  EXPECT_EQ(n, Vector(0, 0.9701425, -0.2425356));
}

/* =========== Glass sphere tests =========== */
TEST(sphere_tests, GlassSphereDefaultTransform) {
  auto s = geometry::glassSphere();

  EXPECT_EQ(s->transformation(), (Matrix<4,4>::identity()));
  EXPECT_FLOAT_EQ(s->material().transparency(), 1.0);
  EXPECT_FLOAT_EQ(s->material().refractiveIndex(), 1.5);
}
