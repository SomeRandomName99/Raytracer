#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <limits>
#include <optional>

#include "World.hpp"
#include "Light.hpp"
#include "Shape.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Material.hpp"
#include "Transformations.hpp"
#include "Intersections.hpp"
#include "Pattern.hpp"

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
    s1->material_ = material::Material(utility::Color(0.8, 1.0, 0.6), std::nullopt, 0.1, 0.7, 0.2);
    s1->radius_ = 1;

    s2->setTransform(utility::transformations::scaling(0.5, 0.5, 0.5));
    s2->radius_ = 0.5;

    world.lights_.push_back(this->light);
    world.objects_.emplace_back(s1);
    world.objects_.emplace_back(s2);
  }

  const scene::PointLight light{utility::Color(1,1,1), utility::Point(-10,10,-10)};

 std::shared_ptr<geometry::Sphere> s1 = geometry::normalSphere();
 std::shared_ptr<geometry::Sphere> s2 = geometry::normalSphere();

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
  const auto& shape = world.objects_.front();
  const auto i = geometry::Intersection(shape.get(), 4);
  const auto comps = prepareComputations(i, r);
  const auto c = world.shadeHit(comps);

  EXPECT_EQ(c, utility::Color(0.380661189, 0.475826486, 0.28549589));
}

TEST_F(defaultWorldTests, ShadingIntersectionInside) {
  world.lights_.at(0).position_ = utility::Point(0,0.25,0);
  const auto r = utility::Ray(utility::Point(0,0,0), utility::Vector(0,0,1));
  const auto& shape = world.objects_.back();
  const auto i = geometry::Intersection(shape.get(), 0.5);
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
  outer->material().setAmbient(1);
  auto &inner = world.objects_.back();
  inner->material().setAmbient(1);
  const auto r = utility::Ray(utility::Point(0,0,0.75), utility::Vector(0,0,-1));
  const auto c = world.colorAt(r);

  EXPECT_EQ(c, inner->material().surfaceColor());
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

TEST_F(defaultWorldTests, ShadeHitWithShadow){
  // World setup
  scene::World world{};
  world.lights_.emplace_back(scene::PointLight{utility::Color{1,1,1}, utility::Point(0,0,-10)});
  world.objects_.emplace_back(geometry::normalSphere());
  world.objects_.emplace_back(geometry::normalSphere());
  world.objects_.at(1)->setTransform(utility::transformations::translation(0,0,10));

  utility::Ray ray{utility::Point(0,0,5), utility::Vector(0,0,1)};
  const auto intersection = geometry::Intersection(world.objects_.at(1).get(), 4);
  const auto computation  = prepareComputations(intersection, ray);

  const auto color = world.shadeHit(computation);
  EXPECT_EQ(color, utility::Color(0.1,0.1,0.1));
}

TEST(world_shadow_tests, hitOffsetsPointOfIntersection){
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));
  auto shape = geometry::Sphere();
  shape.setTransform(utility::transformations::translation(0,0,1));
  const auto i = geometry::Intersection(&shape, 5);
  const auto comps = prepareComputations(i, r);

  EXPECT_LT(comps.overPoint.z(), -SHADOW_OFFSET / 2);
  EXPECT_GT(comps.point.z(), comps.overPoint.z());
}

// =================== Reflection Tests ===================
TEST_F(defaultWorldTests, ReflectedColorForNonReflectiveMaterial){
  const auto r = utility::Ray(utility::Point(0,0,0), utility::Vector(0,0,1));
  auto shape = world.objects_.at(1);
  shape->material().setAmbient(1);
  const auto i = geometry::Intersection(shape.get(), 1);
  const auto comps = prepareComputations(i, r);
  const auto color = world.reflectedColor(comps);

  EXPECT_EQ(color, utility::Color(0,0,0));
}

TEST_F(defaultWorldTests, ReflectedColorForReflectiveMaterial){
  auto shape = geometry::normalPlane();
  shape->material().setReflectance(0.5);
  shape->setTransform(utility::transformations::translation(0,-1,0));
  world.objects_.emplace_back(shape);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  const auto i = geometry::Intersection(shape.get(), sqrt(2));
  const auto comps = prepareComputations(i, r);
  const auto color = world.reflectedColor(comps);

  EXPECT_EQ(color, utility::Color(0.190330, 0.237914, 0.1427480));
}

TEST_F(defaultWorldTests, ShadeHitWithReflectiveMaterial){
  auto shape = geometry::normalPlane();
  shape->material().setReflectance(0.5);
  shape->setTransform(utility::transformations::translation(0,-1,0));
  world.objects_.emplace_back(shape);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  const auto i = geometry::Intersection(shape.get(), sqrt(2));
  const auto comps = prepareComputations(i, r);
  const auto color = world.shadeHit(comps);

  EXPECT_EQ(color, utility::Color(0.8767562, 0.9243389, 0.8291734));
}

TEST_F(defaultWorldTests, colorAtWithMutuallyReflectiveSurfaces){
  auto testWorld = scene::World();

  auto lower = geometry::normalPlane();
  lower->material().setReflectance(1);
  lower->setTransform(utility::transformations::translation(0,-1,0));

  auto upper = geometry::normalPlane();
  upper->material().setReflectance(1);
  upper->setTransform(utility::transformations::translation(0,1,0));

  testWorld.lights_.emplace_back(scene::PointLight{utility::Color{1,1,1}, utility::Point(0,0,0)});
  testWorld.objects_.emplace_back(upper);
  testWorld.objects_.emplace_back(lower);

  const auto r = utility::Ray(utility::Point(0,0,0), utility::Vector(0,1,0));
  [[maybe_unused]] const auto color = testWorld.colorAt(r); // should terminate without raising exceptions
}

TEST_F(defaultWorldTests, reflectedColorAtMaximumRecursionDepth){
  auto shape = geometry::normalPlane();
  shape->material().setReflectance(0.5);
  shape->setTransform(utility::transformations::translation(0,-1,0));
  world.objects_.emplace_back(shape);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  const auto i = geometry::Intersection(shape.get(), sqrt(2));
  const auto comps = prepareComputations(i, r);
  const auto color = world.reflectedColor(comps, 0);

  EXPECT_EQ(color, utility::Color(0,0,0));
}

// =================== Refraction Tests ===================
TEST_F(defaultWorldTests, RefractedColorWithOpaqueSurface){
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));
  auto shape = world.objects_.at(0);
  const auto xs = geometry::intersections(geometry::Intersection(shape.get(), 4), geometry::Intersection(shape.get(), 6));
  const auto comps = prepareComputations(xs[0], r);
  const auto color = world.refractedColor(comps, 5);

  EXPECT_EQ(color, utility::Color(0,0,0));
}

TEST_F(defaultWorldTests, RefractedColorAtMaximumRecursionDepth){
  auto shape = world.objects_.at(0);
  shape->material().setTransparency(1.0);
  shape->material().setRefractiveIndex(1.5);
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));
  const auto xs = geometry::intersections(geometry::Intersection(shape.get(), 4), geometry::Intersection(shape.get(), 6));
  const auto comps = prepareComputations(xs[0], r);
  const auto color = world.refractedColor(comps, 0);

  EXPECT_EQ(color, utility::Color(0,0,0));
}

TEST_F(defaultWorldTests, RefractedColorUnderTotalInternalReflection){
  auto shape = world.objects_.at(0);
  shape->material().setTransparency(1.0);
  shape->material().setRefractiveIndex(1.5);
  const auto r = utility::Ray(utility::Point(0,0,sqrt(2)/2), utility::Vector(0,1,0));
  const auto xs = geometry::intersections(geometry::Intersection(shape.get(), -sqrt(2)/2), geometry::Intersection(shape.get(), sqrt(2)/2));
  const auto comps = prepareComputations(xs[1], r, xs);
  const auto color = world.refractedColor(comps);

  EXPECT_EQ(color, utility::Color(0,0,0));
}
class MockTestPattern {
public:
  MockTestPattern() = default;
  utility::Color drawPatternAt(const utility::Tuple& point) const noexcept {
    return utility::Color(point.x(), point.y(), point.z());
  }
};
static bool operator==([[maybe_unused]]const MockTestPattern& lhs, [[maybe_unused]]const MockTestPattern& rhs) noexcept{
  return true;
}
TEST_F(defaultWorldTests, RefractedColorWithARefractedRay){
  auto defaultPattern = material::Pattern(MockTestPattern{});
  
  auto a = world.objects_.at(0);
  a->material().setAmbient(1);
  a->material().setPattern(defaultPattern);

  auto b = world.objects_.at(1);
  b->material().setTransparency(1.0);
  b->material().setRefractiveIndex(1.5);

  const auto r = utility::Ray(utility::Point(0,0,0.1), utility::Vector(0,1,0));
  const auto xs = geometry::intersections(geometry::Intersection(a.get(), -0.9899), 
                                          geometry::Intersection(b.get(), -0.4899), 
                                          geometry::Intersection(b.get(), 0.4899), 
                                          geometry::Intersection(a.get(), 0.9899));
  const auto comps = prepareComputations(xs[2], r, xs);
  const auto color = world.refractedColor(comps);

  EXPECT_EQ(color, utility::Color(0,0.998885,0.047217));
}

TEST_F(defaultWorldTests, shadeHitWithTransparentMaterial){
  auto floor = geometry::normalPlane();
  floor->setTransform(utility::transformations::translation(0,-1,0));
  floor->material().setTransparency(0.5);
  floor->material().setRefractiveIndex(1.5);

  auto ball = geometry::normalSphere();
  ball->setTransform(utility::transformations::translation(0,-3.5,-0.5));
  ball->material().setAmbient(0.5);
  ball->material().setSurfaceColor(utility::Color(1,0,0));

  world.objects_.emplace_back(floor);
  world.objects_.emplace_back(ball);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  const auto xs = geometry::intersections(geometry::Intersection(floor.get(), sqrt(2)));
  const auto comps = prepareComputations(xs[0], r, xs);
  const auto color = world.shadeHit(comps);

  EXPECT_EQ(color, utility::Color(0.936425, 0.686425, 0.686425));
}