#include <gtest/gtest.h>

#include "World.hpp"
#include "Light.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "Transformations.hpp"

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
  s1.material  = material::Material(utility::Color(0.8, 1.0, 0.6), 0.1, 0.7, 0.2);
  s2.setTransform(utility::transformations::scaling(0.5, 0.5, 0.5));
  world.lights_.push_back(light);
  world.objects_.push_back(s1);
  world.objects_.push_back(s2);
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
  EXPECT_EQ(xs[0].t_, 4);
  EXPECT_EQ(xs[1].t_, 4.5);
  EXPECT_EQ(xs[2].t_, 5.5);
  EXPECT_EQ(xs[3].t_, 6);
}