#include <gtest/gtest.h>
#include <memory>
#include <limits>

#include "World.hpp"
#include "Light.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Material.hpp"
#include "Transformations.hpp"
#include "Intersections.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Creation Test =========== */
TEST(world_tests, Creation){
  const auto world = scene::World();

  EXPECT_EQ(world.lights_.size(), 0);
  EXPECT_EQ(world.objects_.size(), 0);
}

class defaultWorldTests : public ::testing::Test {
protected:
  void SetUp() override {
    this->s1.material_ = material::Material(utility::Color(0.8, 1.0, 0.6), std::nullopt, 0.1, 0.7, 0.2);
    this->s1.radius_ = 1;

    this->s2.setTransform(utility::transformations::scaling(0.5, 0.5, 0.5));
    this->s2.radius_ = 0.5;

    this->world.lights_.push_back(this->light);
    this->world.objects_.emplace_back(this->s1);
    this->world.objects_.emplace_back(this->s2);
  }

  const scene::PointLight light{utility::Color(1,1,1), utility::Point(-10,10,-10)};

  geometry::Sphere s1{};
  geometry::Sphere s2{};

  scene::World world{};
};

TEST_F(defaultWorldTests, intersectWorldWithRay){
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));

  const auto xs = world.intersect(r);
  EXPECT_EQ(xs.size(), 4);
  EXPECT_EQ(xs[0].dist, 4);
  EXPECT_EQ(xs[1].dist, 4.5);
  EXPECT_EQ(xs[2].dist, 5.5);
  EXPECT_EQ(xs[3].dist, 6);
}

/* =========== Shading Tests =========== */
TEST_F(defaultWorldTests, ShadingIntersection) {
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));
  const auto shape = world.objects_.front();
  const auto i = geometry::Intersection(shape.normalAt(r.position(4)),  &shape.material(), &shape.inverseTransform(),  4);
  const auto comps = prepareComputations(i, r);
  const auto c = world.shadeHit(comps);

  EXPECT_EQ(c, utility::Color(0.380661189, 0.475826486, 0.28549589));
}

TEST_F(defaultWorldTests, ShadingIntersectionInside) {
  world.lights_.at(0).position_ = utility::Point(0,0.25,0);
  const auto r = utility::Ray(utility::Point(0,0,0), utility::Vector(0,0,1));
  const auto shape = world.objects_.back();
  const auto i = geometry::Intersection(shape.normalAt(r.position(0.5)),  &shape.material(), &shape.inverseTransform(),  0.5);
  const auto comps = prepareComputations(i, r);
  const auto c = world.shadeHit(comps);

  EXPECT_EQ(c, utility::Color(0.904984452, 0.904984452, 0.904984452));
}

TEST_F(defaultWorldTests, RayMisses) {
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,1,0));
  const auto c = world.colorAt(r);

  EXPECT_EQ(c, utility::Color(0,0,0));
}

TEST_F(defaultWorldTests, RayHits) {
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));
  const auto c = world.colorAt(r);

  EXPECT_EQ(c, utility::Color(0.380661189, 0.475826486, 0.28549589));
}

TEST_F(defaultWorldTests, IntersectionBehindRay) {
  auto &outer = world.objects_.front();
  outer.material().setAmbient(1);
  auto &inner = world.objects_.back();
  inner.material().setAmbient(1);
  const auto r = utility::Ray(utility::Point(0,0,0.75), utility::Vector(0,0,-1));
  const auto c = world.colorAt(r);

  EXPECT_EQ(c, inner.material().surfaceColor());
}

// =================== Shadow Tests ===================
TEST(world_shadow_tests, NoShadowWhenNothingIsCollinearWithPointAndLight){
  const auto w = scene::defaultWorld();
  const auto p = utility::Point(0,10,0);

  EXPECT_FALSE(w.isShadowed(w.lights_.front(),p ));
}

TEST(world_shadow_tests, ShadowWhenObjectIsBetweenPointAndLight){
  const auto w = scene::defaultWorld();
  const auto p = utility::Point(10,-10,10);

  EXPECT_TRUE(w.isShadowed(w.lights_.front(), p));
}

TEST(world_shadow_tests, NoShadowWhenObjectIsBehindLight){
  const auto w = scene::defaultWorld();
  const auto p = utility::Point(-20,20,-20);

  EXPECT_FALSE(w.isShadowed(w.lights_.front(), p));
}

TEST(world_shadow_tests, NoShadowWhenObjectIsBehindPoint){
  const auto w = scene::defaultWorld();
  const auto p = utility::Point(-2,2,-2);

  EXPECT_FALSE(w.isShadowed(w.lights_.front(), p));
}

TEST(world_shadow_tests, ShadeHitWithShadow){
  // World setup
  scene::World world{};
  world.lights_.emplace_back(scene::PointLight{utility::Color{1,1,1}, utility::Point(0,0,-10)});
  world.objects_.emplace_back(geometry::Sphere{});
  world.objects_.emplace_back(geometry::Sphere{});
  world.objects_.at(1).setTransform(utility::transformations::translation(0,0,10));

  utility::Ray ray{utility::Point(0,0,5), utility::Vector(0,0,1)};
  const auto intersection = geometry::Intersection(world.objects_.at(1).normalAt(ray.position(4)), &world.objects_.at(1).material(), &world.objects_.at(1).inverseTransform(),  4);
  const auto computation  = prepareComputations(intersection, ray);

  const auto color = world.shadeHit(computation);
  EXPECT_EQ(color, utility::Color(0.1,0.1,0.1));
}

TEST(world_shadow_tests, hitOffsetsPointOfIntersection){
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));
  auto shape = geometry::Sphere();
  shape.setTransform(utility::transformations::translation(0,0,1));
  const auto i = geometry::Intersection( shape.normalAt(r.position(5)),  &shape.material(), &shape.inverseTransform(),  5);
  const auto comps = prepareComputations(i, r);

  EXPECT_LT(comps.overPoint.z(), -SHADOW_OFFSET / 2);
  EXPECT_GT(comps.point.z(), comps.overPoint.z());
}

// =================== Reflection Tests ===================
TEST_F(defaultWorldTests, ReflectedColorForNonReflectiveMaterial){
  const auto r = utility::Ray(utility::Point(0,0,0), utility::Vector(0,0,1));
  auto shape = world.objects_.at(1);
  shape.material().setAmbient(1);
  const auto i = geometry::Intersection(shape.normalAt(r.position(1)),  &shape.material(), &shape.inverseTransform(),  1);
  const auto comps = prepareComputations(i, r);
  const auto color = world.reflectedColor(comps);

  EXPECT_EQ(color, utility::Color(0,0,0));
}

TEST_F(defaultWorldTests, ReflectedColorForReflectiveMaterial){
  auto shape = geometry::Plane();
  shape.material().setReflectance(0.5);
  shape.setTransform(utility::transformations::translation(0,-1,0));
  world.objects_.emplace_back(shape);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  const auto i = geometry::Intersection(shape.normalAt(r.position(sqrt(2))),  &shape.material(), &shape.inverseTransform(),  sqrt(2));
  const auto comps = prepareComputations(i, r);
  const auto color = world.reflectedColor(comps);

  EXPECT_EQ(color, utility::Color(0.190330, 0.237914, 0.1427480));
}

TEST_F(defaultWorldTests, ShadeHitWithReflectiveMaterial){
  auto shape = geometry::Plane();
  shape.material().setReflectance(0.5);
  shape.setTransform(utility::transformations::translation(0,-1,0));
  world.objects_.emplace_back(shape);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  const auto i = geometry::Intersection(shape.normalAt(r.position(sqrt(2))),  &shape.material(), &shape.inverseTransform(),  sqrt(2));
  const auto comps = prepareComputations(i, r);
  const auto color = world.shadeHit(comps);

  EXPECT_EQ(color, utility::Color(0.8767562, 0.9243389, 0.8291734));
}

TEST_F(defaultWorldTests, colorAtWithMutuallyReflectiveSurfaces){
  auto testWorld = scene::World();

  auto lower = geometry::Plane();
  lower.material().setReflectance(1);
  lower.setTransform(utility::transformations::translation(0,-1,0));

  auto upper = geometry::Plane();
  upper.material().setReflectance(1);
  upper.setTransform(utility::transformations::translation(0,1,0));

  testWorld.lights_.emplace_back(scene::PointLight{utility::Color{1,1,1}, utility::Point(0,0,0)});
  testWorld.objects_.emplace_back(lower);
  testWorld.objects_.emplace_back(upper);

  const auto r = utility::Ray(utility::Point(0,0,0), utility::Vector(0,1,0));
  const auto color = testWorld.colorAt(r); // should terminate without raising exceptions
}

TEST_F(defaultWorldTests, reflectedColorAtMaximumRecursionDepth){
  auto shape = geometry::Plane();
  shape.material().setReflectance(0.5);
  shape.setTransform(utility::transformations::translation(0,-1,0));
  world.objects_.emplace_back(shape);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  const auto i = geometry::Intersection(shape.normalAt(r.position(sqrt(2))),  &shape.material(), &shape.inverseTransform(),  sqrt(2));
  const auto comps = prepareComputations(i, r);
  const auto color = world.reflectedColor(comps, 0);

  EXPECT_EQ(color, utility::Color(0,0,0));
}