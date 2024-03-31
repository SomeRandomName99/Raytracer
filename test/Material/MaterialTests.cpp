#include <gtest/gtest.h>

#include "Material.hpp"
#include "Pattern.hpp"
#include "StripePattern.hpp"
#include "Light.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Creation Test =========== */
TEST(material_tests, default_material_creation) {
  const auto m = material::Material();
  EXPECT_TRUE(m.surfaceColor() == utility::Color(1.0, 1.0, 1.0));
  EXPECT_FLOAT_EQ(m.ambient(), 0.1);
  EXPECT_FLOAT_EQ(m.diffuse(), 0.9);
  EXPECT_FLOAT_EQ(m.specular(), 0.9);
  EXPECT_FLOAT_EQ(m.shininess(), 200.0);
  EXPECT_FLOAT_EQ(m.reflectance(), 0);
}

TEST(material_tests, lightingWithAMaterialApplied){
  auto m = material::Material();
  m.setPattern(material::StripePattern(utility::Color(1, 1, 1), utility::Color(0, 0, 0)));
  m.setAmbient(1);
  m.setDiffuse(0);
  m.setSpecular(0);

  auto s = std::make_shared<geometry::Shape>(geometry::Sphere());
  s->setMaterial(m);

  const auto eyeVector = utility::Vector(0, 0, -1);
  const auto normalVector = utility::Vector(0, 0, -1);
  const auto light = scene::PointLight(utility::Color(1, 1, 1), utility::Point(0, 0, -10));

  EXPECT_EQ(scene::lighting(s, light, utility::Point(0.9, 0, 0), eyeVector, normalVector, false), utility::Color(1, 1, 1));
  EXPECT_EQ(scene::lighting(s, light, utility::Point(1.1, 0, 0), eyeVector, normalVector, false), utility::Color(0, 0, 0));
}

