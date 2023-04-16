#include <gtest/gtest.h>

#include "include/maths.h"


TEST(maths_tests, equal_true) {
  EXPECT_TRUE(floatEqual(1.0,1.0));
}

TEST(maths_tests, equal_false) {
  EXPECT_FALSE(floatEqual(1.0,1.5));
}