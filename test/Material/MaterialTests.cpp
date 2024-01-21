#include <gtest/gtest.h>

#include "Material.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Creation Test =========== */
TEST(material_tests, default_material_creation) {
  const auto m = material::Material();
  EXPECT_TRUE(m.surfaceColor_ == utility::Color(1.0f, 1.0f, 1.0f));
  EXPECT_EQ(m.ambient_, 0.1f);
  EXPECT_EQ(m.diffuse_, 0.9f);
  EXPECT_EQ(m.specular_, 0.9f);
  EXPECT_EQ(m.shininess_, 200.0f);
}

