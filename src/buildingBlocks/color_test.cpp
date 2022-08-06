#include <gtest/gtest.h>
#include <iostream>

#include "include/color.h"

/* =========== Creation Test =========== */
TEST(color_test, color_creation_red_correct) {
  const auto color1 = Color(-0.5, 0.4, 1.7);
  EXPECT_FLOAT_EQ(color1.red(), -0.5f);
}

TEST(color_test, color_creation_green_correct) {
  const auto color1 = Color(-0.5, 0.4, 1.7);
  EXPECT_FLOAT_EQ(color1.green(), 0.4f);
}

TEST(color_test, color_creation_blue_correct) {
  const auto color1 = Color(-0.5, 0.4, 1.7);
  EXPECT_FLOAT_EQ(color1.blue(), 1.7f);
}

/* =========== Operations Test =========== */
TEST(color_test, adding_colors_correct) {
  const auto color1 = Color(0.9, 0.6, 0.75);
  const auto color2 = Color(0.7, 0.1, 0.25);
  const auto color3 = color1 + color2;
  EXPECT_FLOAT_EQ(color3.red(), 1.6f);
  EXPECT_FLOAT_EQ(color3.green(), 0.7f);
  EXPECT_FLOAT_EQ(color3.blue(), 1.0f);
}

TEST(color_test, subtracting_colors_correct) {
  const auto color1 = Color(0.9, 0.6, 0.75);
  const auto color2 = Color(0.7, 0.1, 0.25);
  const auto color3 = color1 - color2;
  EXPECT_FLOAT_EQ(color3.red(), 0.2f);
  EXPECT_FLOAT_EQ(color3.green(), 0.5f);
  EXPECT_FLOAT_EQ(color3.blue(), 0.5f);
}

TEST(color_test, scalar_multiplication_colors_correct) {
  const auto color1 = Color(0.2, 0.3, 0.4);
  const auto color2 = color1 * 2;
  EXPECT_FLOAT_EQ(color2.red(), 0.4f);
  EXPECT_FLOAT_EQ(color2.green(), 0.6f);
  EXPECT_FLOAT_EQ(color2.blue(), 0.8f);
}

TEST(color_test, hadamarad_product_colors_correct) {
  const auto color1 = Color(1, 0.2, 0.4);
  const auto color2 = Color(0.9, 1, 0.1);
  const auto color3 = color1 * color2;
  EXPECT_FLOAT_EQ(color3.red(), 0.9f);
  EXPECT_FLOAT_EQ(color3.green(), 0.2f);
  EXPECT_FLOAT_EQ(color3.blue(), 0.04f);
}

TEST(color_test, color_equality_equal_correct) {
  const auto color1 = Color(1, 0.2, 0.4);
  const auto color2 = Color(1, 0.2, 0.4);
  EXPECT_TRUE(color1==color2);
}

TEST(color_test, color_equality_unequal_correct) {
  const auto color1 = Color(1, 0.2, 0.4);
  const auto color2 = Color(0.9, 1, 0.1);
  EXPECT_FALSE(color1==color2);
}

/* =========== Creation Test =========== */
TEST(color_test, color_default_zeros) {
  const auto color1 = Color();
  EXPECT_EQ(color1, Color(0.0f, 0.0f, 0.0f));
}