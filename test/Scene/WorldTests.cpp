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
#include "Arena.hpp"

using namespace raytracer;
using namespace utility;

class WorldTest : public ::testing::Test {
protected:
  Arena<geometry::Intersection> xs;
  
  WorldTest() : xs(MB(1)) {}
  
  void SetUp() override {
    // Clear arena before each test to ensure clean state
    xs.clear();
  }
  
  void TearDown() override {}
};

/* =========== Creation Test =========== */
TEST(world_tests, Creation){
  const auto world = scene::World();

  EXPECT_EQ(world.lights_.size(), 0);
  EXPECT_EQ(world.objects_.size(), 0);
}

class defaultWorldTests : public ::testing::Test {
protected:
  Arena<geometry::Intersection> xs;
  
  defaultWorldTests() : xs(MB(1)) {}
  
  void SetUp() override {
    // Clear arena before each test to ensure clean state
    xs.clear();
    
    s1->setMaterial(material::Material(utility::Color(0.8, 1.0, 0.6), 0.1, 0.7, 0.2));
    s1->radius_ = 1;

    s2->setTransform(utility::transformations::scaling(0.5, 0.5, 0.5));
    s2->radius_ = 0.5;

    world.lights_.push_back(this->light);
    world.objects_.emplace_back(s1);
    world.objects_.emplace_back(s2);
  }

  const scene::PointLight light{utility::Color(1,1,1), utility::Point(-10,10,-10)};

 std::shared_ptr<geometry::Sphere> s1 = geometry::makeSphere();
 std::shared_ptr<geometry::Sphere> s2 = geometry::makeSphere();

  scene::World world{};
};

TEST_F(defaultWorldTests, intersectWorldWithRay){
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));

  world.intersect(r, xs);
  EXPECT_EQ(xs.size, 4);
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
  xs.pushBack(i);
  const auto comps = prepareComputations(i, r, xs);
  const auto c = world.shadeHit(comps, xs);

  EXPECT_EQ(c, utility::Color(0.380661189, 0.475826486, 0.28549589));
}

TEST_F(defaultWorldTests, ShadingIntersectionInside) {
  world.lights_.at(0).position_ = utility::Point(0,0.25,0);
  const auto r = utility::Ray(utility::Point(0,0,0), utility::Vector(0,0,1));
  const auto& shape = world.objects_.back();
  const auto i = geometry::Intersection(shape.get(), 0.5);
  xs.pushBack(i);
  const auto comps = prepareComputations(i, r, xs);
  const auto c = world.shadeHit(comps, xs);

  EXPECT_EQ(c, utility::Color(0.904984452, 0.904984452, 0.904984452));
}

TEST_F(defaultWorldTests, RayMisses) {
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,1,0));
  const auto c = world.colorAt(r, xs);

  EXPECT_EQ(c, utility::Color(0,0,0));
}

TEST_F(defaultWorldTests, RayHits) {
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));
  const auto c = world.colorAt(r, xs);

  EXPECT_EQ(c, utility::Color(0.380661189, 0.475826486, 0.28549589));
}

TEST_F(defaultWorldTests, IntersectionBehindRay) {
  auto &outer = world.objects_.front();
  outer->material().setAmbient(1);
  auto &inner = world.objects_.back();
  inner->material().setAmbient(1);
  const auto r = utility::Ray(utility::Point(0,0,0.75), utility::Vector(0,0,-1));
  const auto c = world.colorAt(r, xs);

  EXPECT_EQ(c, inner->material().surfaceColor());
}

// =================== Shadow Tests ===================
TEST_F(WorldTest, NoShadowWhenNothingIsCollinearWithPointAndLight){
  const auto w = scene::defaultWorld();
  const auto p = utility::Point(0,10,0);

  EXPECT_FALSE(w.isShadowed(w.lights_.front(), p, xs));
}

TEST_F(WorldTest, ShadowWhenObjectIsBetweenPointAndLight){
  const auto w = scene::defaultWorld();
  const auto p = utility::Point(10,-10,10);

  EXPECT_TRUE(w.isShadowed(w.lights_.front(), p, xs));
}

TEST_F(WorldTest, NoShadowWhenObjectIsBehindLight){
  const auto w = scene::defaultWorld();
  const auto p = utility::Point(-20,20,-20);

  EXPECT_FALSE(w.isShadowed(w.lights_.front(), p, xs));
}

TEST_F(WorldTest, NoShadowWhenObjectIsBehindPoint){
  const auto w = scene::defaultWorld();
  const auto p = utility::Point(-2,2,-2);

  EXPECT_FALSE(w.isShadowed(w.lights_.front(), p, xs));
}

TEST_F(defaultWorldTests, ShadeHitWithShadow){
  // World setup
  scene::World world{};
  world.lights_.emplace_back(scene::PointLight{utility::Color{1,1,1}, utility::Point(0,0,-10)});
  world.objects_.emplace_back(geometry::makeSphere());
  world.objects_.emplace_back(geometry::makeSphere());
  world.objects_.at(1)->setTransform(utility::transformations::translation(0,0,10));

  utility::Ray ray{utility::Point(0,0,5), utility::Vector(0,0,1)};
  const auto intersection = geometry::Intersection(world.objects_.at(1).get(), 4);
  xs.pushBack(intersection);
  const auto computation  = prepareComputations(intersection, ray, xs);

  const auto color = world.shadeHit(computation, xs);
  EXPECT_EQ(color, utility::Color(0.1,0.1,0.1));
}

TEST_F(WorldTest, HitOffsetsPointOfIntersection){
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));
  auto shape = geometry::Sphere();
  shape.setTransform(utility::transformations::translation(0,0,1));
  const auto i = geometry::Intersection(&shape, 5);
  xs.pushBack(i);
  const auto comps = prepareComputations(i, r, xs);

  EXPECT_LT(comps.overPoint.z(), -SHADOW_OFFSET / 2);
  EXPECT_GT(comps.point.z(), comps.overPoint.z());
}

TEST_F(WorldTest, NoSHadowWhenObjectHasNoShadowProperty){
  const auto w = scene::defaultWorld();
  const auto p = utility::Point(10,-10,10);

  std::ranges::for_each(w.objects_, [](auto& obj){ obj->setShadow(false); });

  EXPECT_FALSE(w.isShadowed(w.lights_.front(), p, xs));
}

// =================== Reflection Tests ===================
TEST_F(defaultWorldTests, ReflectedColorForNonReflectiveMaterial){
  const auto r = utility::Ray(utility::Point(0,0,0), utility::Vector(0,0,1));
  auto shape = world.objects_.at(1);
  shape->material().setAmbient(1);
  const auto i = geometry::Intersection(shape.get(), 1);
  xs.pushBack(i);
  const auto comps = prepareComputations(i, r, xs);
  const auto color = world.reflectedColor(comps, xs);

  EXPECT_EQ(color, utility::Color(0,0,0));
}

TEST_F(defaultWorldTests, ReflectedColorForReflectiveMaterial){
  auto shape = geometry::makePlane();
  shape->material().setReflectance(0.5);
  shape->setTransform(utility::transformations::translation(0,-1,0));
  world.objects_.emplace_back(shape);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  const auto i = geometry::Intersection(shape.get(), sqrt(2));
  xs.pushBack(i);
  const auto comps = prepareComputations(i, r, xs);
  const auto color = world.reflectedColor(comps, xs);

  EXPECT_EQ(color, utility::Color(0.1903477, 0.2379347, 0.1427608));
}

TEST_F(defaultWorldTests, ShadeHitWithReflectiveMaterial){
  auto shape = geometry::makePlane();
  shape->material().setReflectance(0.5);
  shape->setTransform(utility::transformations::translation(0,-1,0));
  world.objects_.emplace_back(shape);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  const auto i = geometry::Intersection(shape.get(), sqrt(2));
  xs.pushBack(i);
  const auto comps = prepareComputations(i, r, xs);
  const auto color = world.shadeHit(comps, xs);

  EXPECT_EQ(color, utility::Color(0.87677319, 0.92436014, 0.82918625));
}

TEST_F(defaultWorldTests, colorAtWithMutuallyReflectiveSurfaces){
  auto testWorld = scene::World();

  auto lower = geometry::makePlane();
  lower->material().setReflectance(1);
  lower->setTransform(utility::transformations::translation(0,-1,0));

  auto upper = geometry::makePlane();
  upper->material().setReflectance(1);
  upper->setTransform(utility::transformations::translation(0,1,0));

  testWorld.lights_.emplace_back(scene::PointLight{utility::Color{1,1,1}, utility::Point(0,0,0)});
  testWorld.objects_.emplace_back(upper);
  testWorld.objects_.emplace_back(lower);

  const auto r = utility::Ray(utility::Point(0,0,0), utility::Vector(0,1,0));
  [[maybe_unused]] const auto color = testWorld.colorAt(r, xs); // should terminate without raising exceptions
}

TEST_F(defaultWorldTests, reflectedColorAtMaximumRecursionDepth){
  auto shape = geometry::makePlane();
  shape->material().setReflectance(0.5);
  shape->setTransform(utility::transformations::translation(0,-1,0));
  world.objects_.emplace_back(shape);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  const auto i = geometry::Intersection(shape.get(), sqrt(2));
  xs.pushBack(i);
  const auto comps = prepareComputations(i, r, xs);
  const auto color = world.reflectedColor(comps, xs, 0);

  EXPECT_EQ(color, utility::Color(0,0,0));
}

// =================== Refraction Tests ===================
TEST_F(defaultWorldTests, RefractedColorWithOpaqueSurface){
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));
  auto shape = world.objects_.at(0);
  xs.pushBack(geometry::Intersection(shape.get(), 4));
  xs.pushBack(geometry::Intersection(shape.get(), 6));
  const auto comps = prepareComputations(xs[0], r, xs);
  const auto color = world.refractedColor(comps, xs, 5);

  EXPECT_EQ(color, utility::Color(0,0,0));
}

TEST_F(defaultWorldTests, RefractedColorAtMaximumRecursionDepth){
  auto shape = world.objects_.at(0);
  shape->material().setTransparency(1.0);
  shape->material().setRefractiveIndex(1.5);
  const auto r = utility::Ray(utility::Point(0,0,-5), utility::Vector(0,0,1));
  xs.pushBack(geometry::Intersection(shape.get(), 4));
  xs.pushBack(geometry::Intersection(shape.get(), 6));
  const auto comps = prepareComputations(xs[0], r, xs);
  const auto color = world.refractedColor(comps, xs, 0);

  EXPECT_EQ(color, utility::Color(0,0,0));
}

TEST_F(defaultWorldTests, RefractedColorUnderTotalInternalReflection){
  auto shape = world.objects_.at(0);
  shape->material().setTransparency(1.0);
  shape->material().setRefractiveIndex(1.5);
  const auto r = utility::Ray(utility::Point(0,0,sqrt(2)/2), utility::Vector(0,1,0));
  xs.pushBack(geometry::Intersection(shape.get(), -sqrt(2)/2));
  xs.pushBack(geometry::Intersection(shape.get(), sqrt(2)/2));
  const auto comps = prepareComputations(xs[1], r, xs);
  const auto color = world.refractedColor(comps, xs);

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
  xs.pushBack(geometry::Intersection(a.get(), -0.9899));
  xs.pushBack(geometry::Intersection(b.get(), -0.4899));
  xs.pushBack(geometry::Intersection(b.get(), 0.4899));
  xs.pushBack(geometry::Intersection(a.get(), 0.9899));
  const auto comps = prepareComputations(xs[2], r, xs);
  const auto color = world.refractedColor(comps, xs);

  EXPECT_EQ(color, utility::Color(0,0.99888324,0.04724672));
}

TEST_F(defaultWorldTests, shadeHitWithTransparentMaterial){
  auto floor = geometry::makePlane();
  floor->setTransform(utility::transformations::translation(0,-1,0));
  floor->material().setTransparency(0.5);
  floor->material().setRefractiveIndex(1.5);

  auto ball = geometry::makeSphere();
  ball->setTransform(utility::transformations::translation(0,-3.5,-0.5));
  ball->material().setAmbient(0.5);
  ball->material().setSurfaceColor(utility::Color(1,0,0));

  world.objects_.emplace_back(floor);
  world.objects_.emplace_back(ball);

  const auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));
  xs.pushBack(geometry::Intersection(floor.get(), sqrt(2)));
  const auto comps = prepareComputations(xs[0], r, xs);
  const auto color = world.shadeHit(comps, xs);

  EXPECT_EQ(color, utility::Color(0.936425, 0.686425, 0.686425));
}

TEST_F(defaultWorldTests, shadeHitWithReflectiveTransparentMaterial){
  auto r = utility::Ray(utility::Point(0,0,-3), utility::Vector(0,-sqrt(2)/2,sqrt(2)/2));

  auto floor = geometry::makePlane();
  floor->setTransform(utility::transformations::translation(0,-1,0));
  floor->material().setReflectance(0.5);
  floor->material().setTransparency(0.5);
  floor->material().setRefractiveIndex(1.5);

  auto ball = geometry::makeSphere();
  ball->setTransform(utility::transformations::translation(0,-3.5,-0.5));
  ball->material().setAmbient(0.5);
  ball->material().setSurfaceColor(utility::Color(1,0,0));

  world.objects_.emplace_back(floor);
  world.objects_.emplace_back(ball);

  xs.pushBack(geometry::Intersection(floor.get(), sqrt(2)));
  const auto comps = prepareComputations(xs[0], r, xs);
  const auto color = world.shadeHit(comps, xs);

  EXPECT_EQ(color, utility::Color(0.9339151, 0.6964342, 0.6924307));
}