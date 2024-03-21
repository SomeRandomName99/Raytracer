#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "PatternT.hpp"
#include "StripePattern.hpp"
#include "GradientPattern.hpp"
#include "Matrix.hpp"
#include "Tuple.hpp"
#include "Transformations.hpp"
#include "Color.hpp"

using namespace raytracer;
using namespace utility;

const auto black = Color(0, 0, 0);
const auto white = Color(1, 1, 1);

class MockTestPattern : public material::PatternT<MockTestPattern> {
public:
  MOCK_METHOD(utility::Color, localPattern_at, (const utility::Tuple& point), (const, noexcept));
};
class TestPatternTests : public ::testing::Test {
protected:
  MockTestPattern defaultPattern{};

  TestPatternTests() {
    ON_CALL(defaultPattern, localPattern_at(testing::_)).WillByDefault(testing::Invoke([](const utility::Tuple& point) {
      return utility::Color(point.x(), point.y(), point.z());
    }));
  }
};

/* =========== Creation Test =========== */
TEST_F(TestPatternTests, default_pattern_creation) {
  EXPECT_TRUE((defaultPattern.transformation() == Matrix<4,4>::identity()));
}

TEST_F(TestPatternTests, assigning_transformation) {
  const auto t = transformations::translation(1, 2, 3);
  defaultPattern.setTransform(t);
  EXPECT_EQ(defaultPattern.transformation(), t);
}

/* =========== Pattern at shape Tests =========== */
TEST_F(TestPatternTests, pattern_with_object_transformation) {
  auto inverseTransformation = utility::inverse(transformations::scaling(2, 2, 2));
  const auto c = defaultPattern.pattern_at_object(inverseTransformation, Point(2, 3, 4));
  EXPECT_EQ(c, utility::Color(1, 1.5, 2));
}

TEST_F(TestPatternTests, pattern_with_pattern_transformation) {
  defaultPattern.setTransform(transformations::scaling(2, 2, 2));
  const auto c = defaultPattern.pattern_at_object(utility::Matrix<4,4>::identity(), Point(2, 3, 4));
  EXPECT_EQ(c, utility::Color(1, 1.5, 2));
}

TEST_F(TestPatternTests, pattern_with_both_object_and_pattern_transformation) {
  defaultPattern.setTransform(transformations::translation(0.5, 1, 1.5));
  auto inverseTransformation = utility::inverse(transformations::scaling(2, 2, 2));
  const auto c = defaultPattern.pattern_at_object(inverseTransformation, Point(2.5, 3, 3.5));
  EXPECT_EQ(c, utility::Color(0.75, 0.5, 0.25));
}

/* =========== Stripe Pattern Tests =========== */
TEST(stripe_pattern_test, stripe_pattern_is_constant_in_y) {
  const auto pattern = material::StripePattern(white, black);
  EXPECT_EQ(pattern.a, pattern.localPattern_at(Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.localPattern_at(Point(0, 1, 0)));
  EXPECT_EQ(pattern.a, pattern.localPattern_at(Point(0, 2, 0)));
}

TEST(stripe_pattern_test, stripe_pattern_is_constant_in_z) {
  const auto pattern = material::StripePattern(white, black);
  EXPECT_EQ(pattern.a, pattern.localPattern_at(Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.localPattern_at(Point(0, 0, 1)));
  EXPECT_EQ(pattern.a, pattern.localPattern_at(Point(0, 0, 2)));
}

TEST(stripe_pattern_test, stripe_pattern_alternates_in_x) {
  const auto pattern = material::StripePattern(white, black);
  EXPECT_EQ(pattern.a, pattern.localPattern_at(Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.localPattern_at(Point(0.9, 0, 0)));
  EXPECT_EQ(pattern.b, pattern.localPattern_at(Point(1, 0, 0)));
  EXPECT_EQ(pattern.b, pattern.localPattern_at(Point(-0.1, 0, 0)));
  EXPECT_EQ(pattern.b, pattern.localPattern_at(Point(-1, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.localPattern_at(Point(-1.1, 0, 0)));
}

/* =========== Gradient Pattern Tests =========== */
TEST(gradient_pattern_test, linear_gradient_pattern) {
  const auto pattern = material::GradientPattern(white, black);
  EXPECT_EQ(pattern.a, pattern.localPattern_at(Point(0, 0, 0)));
  EXPECT_EQ(Color(0.75, 0.75, 0.75), pattern.localPattern_at(Point(0.25, 0, 0)));
  EXPECT_EQ(Color(0.5, 0.5, 0.5), pattern.localPattern_at(Point(0.5, 0, 0)));
  EXPECT_EQ(Color(0.25, 0.25, 0.25), pattern.localPattern_at(Point(0.75, 0, 0)));
}