#include <gtest/gtest.h>
#include <cmath>

#include "Light.hpp"
#include "Color.hpp"
#include "Tuple.hpp"
#include "Material.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Creation Test =========== */
TEST(pointLight_tests, Creation){
  const auto intensity = Color(1,1,1);
  const auto position  = Point(1,1,1);

  const auto lightPoint = scene::PointLight(intensity, position);

  EXPECT_EQ(lightPoint.intensity_, intensity);
  EXPECT_EQ(lightPoint.position_, position);
}

/* =========== Light Material Interaction Tests =========== */
class lightMaterialInteractionTests : public ::testing::Test {
protected:
  const material::Material m{};
  const Tuple position = Point(0,0,0);
};

TEST_F(lightMaterialInteractionTests, EyeBetweenLightAndSurface){
  const auto eyeVector = Vector(0,0,-1);
  const auto normalVector = Vector(0,0,-1);
  const auto light = scene::PointLight(Color(1,1,1), Point(0,0,-10));

  const auto result = scene::lighting(m, light, position, eyeVector, normalVector, false);

  EXPECT_TRUE(result  == Color(1.9f,1.9f,1.9f));
}

TEST_F(lightMaterialInteractionTests, EyeBetweenLightAndSurface45){
  const auto eyeVector = Vector(0,std::sqrt(2)/2,-std::sqrt(2)/2);
  const auto normalVector = Vector(0,0,-1);
  const auto light = scene::PointLight(Color(1,1,1), Point(0,0,-10));

  const auto result = scene::lighting(m, light, position, eyeVector, normalVector, false);

  EXPECT_TRUE(result  == Color(1.0f,1.0f,1.0f));
}

TEST_F(lightMaterialInteractionTests, EyeOppositeSurfaceLightOffset45){
  const auto eyeVector = Vector(0,0,-1);
  const auto normalVector = Vector(0,0,-1);
  const auto light = scene::PointLight(Color(1,1,1), Point(0,10,-10));

  const auto result = scene::lighting(m, light, position, eyeVector, normalVector, false);

  const float colorCompExpected = 0.1f + 0.9f * std::sqrt(2)/2;
  EXPECT_TRUE(result  == Color(colorCompExpected,colorCompExpected,colorCompExpected));
}

TEST_F(lightMaterialInteractionTests, EyeInPathOfReflectionVector){
  const auto eyeVector = Vector(0,-std::sqrt(2)/2,-std::sqrt(2)/2);
  const auto normalVector = Vector(0,0,-1);
  const auto light = scene::PointLight(Color(1,1,1), Point(0,10,-10));

  const auto result = scene::lighting(m, light, position, eyeVector, normalVector, false);

  const float colorCompExpected = 1.6363852f;
  EXPECT_TRUE(result  == Color(colorCompExpected,colorCompExpected,colorCompExpected));
}

TEST_F(lightMaterialInteractionTests, LightBehindSurface){
  const auto eyeVector = Vector(0,0,-1);
  const auto normalVector = Vector(0,0,-1);
  const auto light = scene::PointLight(Color(1,1,1), Point(0,0,10));

  const auto result = scene::lighting(m, light, position, eyeVector, normalVector, false);

  EXPECT_TRUE(result  == Color(0.1f,0.1f,0.1f));
}

// ================== shadow Tests ==================
TEST_F(lightMaterialInteractionTests, shadowWithObjectBetweenPointAndLight){
  const auto light = scene::PointLight(Color(1,1,1), Point(0,0,-10));
  const auto position = Point(0,0,0);
  const auto eyeVector = Vector(0,0,-1);
  const auto normalVector = Vector(0,0,-1);
  const auto inShadow = true;

  const auto result = scene::lighting(m, light, position, eyeVector, normalVector, inShadow);

  EXPECT_TRUE(result  == Color(0.1f,0.1f,0.1f));
}