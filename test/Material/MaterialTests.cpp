#include <gtest/gtest.h>

#include "Material.hpp"

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

