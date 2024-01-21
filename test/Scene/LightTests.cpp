#include <gtest/gtest.h>

#include "Light.hpp"
#include "Color.hpp"
#include "Tuple.hpp"

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