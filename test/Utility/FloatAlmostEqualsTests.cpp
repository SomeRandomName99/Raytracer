#include <gtest/gtest.h>

#include "FloatAlmostEquals.hpp"

using namespace raytracer;
using namespace utility;

TEST(maths_tests, equal_true) {
  EXPECT_TRUE(floatNearlyEqual(1.0f,1.0f));
}

TEST(maths_tests, equal_false) {
  EXPECT_FALSE(floatNearlyEqual(1.0f,1.5f));
}