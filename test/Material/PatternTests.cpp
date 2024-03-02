#include <gtest/gtest.h>

#include "Pattern.hpp"
#include "Tuple.hpp"

using namespace raytracer;
using namespace utility;

const auto black = Color(0, 0, 0);
const auto white = Color(1, 1, 1);

/* =========== Creation Test =========== */
TEST(pattern_tests, stripe_pattern_creation) {
  const auto pattern = material::StripePattern(white, black);
  EXPECT_EQ(pattern.a, white);
  EXPECT_EQ(pattern.b, black);
}

/* =========== Stripe Pattern Tests =========== */
TEST(pattern_tests, stripe_pattern_is_constant_in_y) {
  const auto pattern = material::StripePattern(white, black);
  EXPECT_EQ(pattern.a, stripe_at(pattern, Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, stripe_at(pattern, Point(0, 1, 0)));
  EXPECT_EQ(pattern.a, stripe_at(pattern, Point(0, 2, 0)));
}

TEST(pattern_tests, stripe_pattern_is_constant_in_z) {
  const auto pattern = material::StripePattern(white, black);
  EXPECT_EQ(pattern.a, stripe_at(pattern, Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, stripe_at(pattern, Point(0, 0, 1)));
  EXPECT_EQ(pattern.a, stripe_at(pattern, Point(0, 0, 2)));
}

TEST(pattern_tests, stripe_pattern_alternates_in_x) {
  const auto pattern = material::StripePattern(white, black);
  EXPECT_EQ(pattern.a, stripe_at(pattern, Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, stripe_at(pattern, Point(0.9, 0, 0)));
  EXPECT_EQ(pattern.b, stripe_at(pattern, Point(1, 0, 0)));
  EXPECT_EQ(pattern.b, stripe_at(pattern, Point(-0.1, 0, 0)));
  EXPECT_EQ(pattern.b, stripe_at(pattern, Point(-1, 0, 0)));
  EXPECT_EQ(pattern.a, stripe_at(pattern, Point(-1.1, 0, 0)));
}

