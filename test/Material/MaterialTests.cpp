#include <gtest/gtest.h>

#include "Material.hpp"
#include "Pattern.hpp"
#include "Light.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Creation Test =========== */
TEST(material_tests, default_material_creation) {
  const auto m = material::Material();
  EXPECT_TRUE(m.surfaceColor_ == utility::Color(1.0, 1.0, 1.0));
  EXPECT_EQ(m.ambient_, 0.1);
  EXPECT_EQ(m.diffuse_, 0.9);
  EXPECT_EQ(m.specular_, 0.9);
  EXPECT_EQ(m.shininess_, 200.0);
}

TEST(material_tests, lightingWithAMaterialApplied){
  auto m = material::Material();
  m.pattern_ = material::StripePattern(utility::Color(1, 1, 1), utility::Color(0, 0, 0));
  m.ambient_ = 1;
  m.diffuse_ = 0;
  m.specular_ = 0;

  const auto eyeVector = utility::Vector(0, 0, -1);
  const auto normalVector = utility::Vector(0, 0, -1);
  const auto light = scene::PointLight(utility::Color(1, 1, 1), utility::Point(0, 0, -10));

  EXPECT_EQ(scene::lighting(m, light, utility::Point(0.9, 0, 0), eyeVector, normalVector, false), utility::Color(1, 1, 1));
  EXPECT_EQ(scene::lighting(m, light, utility::Point(1.1, 0, 0), eyeVector, normalVector, false), utility::Color(0, 0, 0));
}

