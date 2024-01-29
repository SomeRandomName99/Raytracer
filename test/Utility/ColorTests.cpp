#include <gtest/gtest.h>
#include <iostream>

#include "Color.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Creation Test =========== */
TEST(color_tests, color_creation_red_green_blue_correct) {
  const auto color1 = Color(-0.5f, 0.4f, 1.7f);
  EXPECT_FLOAT_EQ(color1.red(), -0.5f);
  EXPECT_FLOAT_EQ(color1.green(), 0.4f);
  EXPECT_FLOAT_EQ(color1.blue(), 1.7f);
}

/* =========== Operations Test =========== */
TEST(color_tests, adding_colors_correct) {
  const auto color1 = Color(0.9f, 0.6f, 0.75f);
  const auto color2 = Color(0.7f, 0.1f, 0.25f);
  const auto color3 = color1 + color2;
  EXPECT_FLOAT_EQ(color3.red(), 1.6f);
  EXPECT_FLOAT_EQ(color3.green(), 0.7f);
  EXPECT_FLOAT_EQ(color3.blue(), 1.0f);
}

TEST(color_tests, subtracting_colors_correct) {
  const auto color1 = Color(0.9f, 0.6f, 0.75f);
  const auto color2 = Color(0.7f, 0.1f, 0.25f);
  const auto color3 = color1 - color2;
  EXPECT_FLOAT_EQ(color3.red(), 0.2f);
  EXPECT_FLOAT_EQ(color3.green(), 0.5f);
  EXPECT_FLOAT_EQ(color3.blue(), 0.5f);
}

TEST(color_tests, scalar_multiplication_colors_correct) {
  const auto color1 = Color(0.2f, 0.3f, 0.4f);
  const auto color2 = color1 * 2;
  EXPECT_FLOAT_EQ(color2.red(), 0.4f);
  EXPECT_FLOAT_EQ(color2.green(), 0.6f);
  EXPECT_FLOAT_EQ(color2.blue(), 0.8f);
}

TEST(color_tests, hadamarad_product_colors_correct) {
  const auto color1 = Color(1, 0.2f, 0.4f);
  const auto color2 = Color(0.9f, 1, 0.1f);
  const auto color3 = color1 * color2;
  EXPECT_FLOAT_EQ(color3.red(), 0.9f);
  EXPECT_FLOAT_EQ(color3.green(), 0.2f);
  EXPECT_FLOAT_EQ(color3.blue(), 0.04f);
}

TEST(color_tests, color_equality_equal_correct) {
  const auto color1 = Color(1, 0.2f, 0.4f);
  const auto color2 = Color(1, 0.2f, 0.4f);
  EXPECT_TRUE(color1==color2);
}

TEST(color_tests, color_equality_unequal_correct) {
  const auto color1 = Color(1, 0.2f, 0.4f);
  const auto color2 = Color(0.9f, 1, 0.1f);
  EXPECT_FALSE(color1==color2);
}

/* =========== Creation Test =========== */
TEST(color_tests, color_default_zeros) {
  const auto color1 = Color();
  EXPECT_EQ(color1, Color(0.0f, 0.0f, 0.0f));
}