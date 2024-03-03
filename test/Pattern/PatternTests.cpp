#include <gtest/gtest.h>

#include "Pattern.hpp"
#include "Tuple.hpp"
#include "Sphere.hpp"
#include "Transformations.hpp"

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

TEST(pattern_tests, stripe_pattern_with_object_transformation) {
  const auto object = std::make_shared<geometry::Sphere>();
  object->setTransform(transformations::scaling(2, 2, 2));
  const auto pattern = material::StripePattern(white, black);
  // const auto c = stripe_at_object(pattern, object, Point(1.5, 0, 0));
  // EXPECT_EQ(c, white);
}

TEST(pattern_tests, stripe_pattern_with_pattern_transformation) {
  const auto object = std::make_shared<geometry::Sphere>();
  auto pattern = material::StripePattern(white, black);
  pattern.setTransform(transformations::scaling(2, 2, 2));
  // const auto c = stripe_at_object(pattern, object, Point(1.5, 0, 0));
  // EXPECT_EQ(c, white);
}

TEST(pattern_tests, stripe_pattern_with_object_and_pattern_transformation) {
  const auto object = std::make_shared<geometry::Sphere>();
  object->setTransform(transformations::scaling(2, 2, 2));
  auto pattern = material::StripePattern(white, black);
  pattern.setTransform(transformations::translation(0.5, 0, 0));
  // const auto c = stripe_at_object(pattern, object, Point(2.5, 0, 0));
  // EXPECT_EQ(c, white);
}

