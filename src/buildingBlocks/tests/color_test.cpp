#include <gtest/gtest.h>
#include <iostream>

#include "include/color.h"

/* =========== Creation Test =========== */
TEST(color_tests, color_creation_red_green_blue_correct) {
  const auto color1 = Color(-0.5, 0.4, 1.7);
  EXPECT_FLOAT_EQ(color1.red(), -0.5);
  EXPECT_FLOAT_EQ(color1.green(), 0.4);
  EXPECT_FLOAT_EQ(color1.blue(), 1.7);
}

/* =========== Operations Test =========== */
TEST(color_tests, adding_colors_correct) {
  const auto color1 = Color(0.9, 0.6, 0.75);
  const auto color2 = Color(0.7, 0.1, 0.25);
  const auto color3 = color1 + color2;
  EXPECT_FLOAT_EQ(color3.red(), 1.6);
  EXPECT_FLOAT_EQ(color3.green(), 0.7);
  EXPECT_FLOAT_EQ(color3.blue(), 1.0);
}

TEST(color_tests, subtracting_colors_correct) {
  const auto color1 = Color(0.9, 0.6, 0.75);
  const auto color2 = Color(0.7, 0.1, 0.25);
  const auto color3 = color1 - color2;
  EXPECT_FLOAT_EQ(color3.red(), 0.2);
  EXPECT_FLOAT_EQ(color3.green(), 0.5);
  EXPECT_FLOAT_EQ(color3.blue(), 0.5);
}

TEST(color_tests, scalar_multiplication_colors_correct) {
  const auto color1 = Color(0.2, 0.3, 0.4);
  const auto color2 = color1 * 2;
  EXPECT_FLOAT_EQ(color2.red(), 0.4);
  EXPECT_FLOAT_EQ(color2.green(), 0.6);
  EXPECT_FLOAT_EQ(color2.blue(), 0.8);
}

TEST(color_tests, hadamarad_product_colors_correct) {
  const auto color1 = Color(1, 0.2, 0.4);
  const auto color2 = Color(0.9, 1, 0.1);
  const auto color3 = color1 * color2;
  EXPECT_FLOAT_EQ(color3.red(), 0.9f);
  EXPECT_FLOAT_EQ(color3.green(), 0.2f);
  EXPECT_FLOAT_EQ(color3.blue(), 0.04f);
}

TEST(color_tests, color_equality_equal_correct) {
  const auto color1 = Color(1, 0.2, 0.4);
  const auto color2 = Color(1, 0.2, 0.4);
  EXPECT_TRUE(color1==color2);
}

TEST(color_tests, color_equality_unequal_correct) {
  const auto color1 = Color(1, 0.2, 0.4);
  const auto color2 = Color(0.9, 1, 0.1);
  EXPECT_FALSE(color1==color2);
}

/* =========== Creation Test =========== */
TEST(color_tests, color_default_zeros) {
  const auto color1 = Color();
  EXPECT_EQ(color1, Color(0.0, 0.0, 0.0));
}