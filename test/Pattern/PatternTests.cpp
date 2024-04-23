#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Pattern.hpp"
#include "StripePattern.hpp"
#include "GradientPattern.hpp"
#include "RingPattern.hpp"
#include "CheckerPattern.hpp"
#include "Matrix.hpp"
#include "Tuple.hpp"
#include "Transformations.hpp"
#include "Color.hpp"
#include "Shape.hpp"

using namespace raytracer;
using namespace utility;

const auto black = Color(0, 0, 0);
const auto white = Color(1, 1, 1);

class MockTestPattern {
public:
  MockTestPattern() = default;
  utility::Color drawPatternAt(const utility::Tuple& point) const noexcept {
    return utility::Color(point.x(), point.y(), point.z());
  }
};

static bool operator==([[maybe_unused]]const MockTestPattern& lhs, [[maybe_unused]]const MockTestPattern& rhs) noexcept{
  return true;
}

class TestPatternTests : public ::testing::Test {
protected:
  material::Pattern defaultPattern = material::Pattern(MockTestPattern{});
};

/* =========== Creation Test =========== */
TEST_F(TestPatternTests, default_pattern_creation) {
  EXPECT_TRUE((defaultPattern.transform() == Matrix<4,4>::identity()));
}

TEST_F(TestPatternTests, assigning_transformation) {
  const auto t = transformations::translation(1, 2, 3);
  defaultPattern.setTransform(t);
  EXPECT_EQ(defaultPattern.transform(), t);
}

/* =========== Pattern at shape Tests =========== */
class MockTestShape : public geometry::ShapeBase {
public:
  MOCK_METHOD(std::vector<geometry::Intersection>, localIntersect, (const utility::Ray& ray), (const, noexcept));
  MOCK_METHOD(utility::Tuple, localNormalAt, (const utility::Tuple &point), (const, noexcept));
};

TEST_F(TestPatternTests, pattern_with_object_transformation) {
  const auto s = std::make_shared<MockTestShape>();
  s->setTransform(transformations::scaling(2, 2, 2));

  const auto c = defaultPattern.drawPatternAt(s.get(), Point(2, 3, 4));
  EXPECT_EQ(c, utility::Color(1, 1.5, 2));
}

TEST_F(TestPatternTests, pattern_with_pattern_transformation) {
  defaultPattern.setTransform(transformations::scaling(2, 2, 2));
  const auto s = std::make_shared<MockTestShape>();

  const auto c = defaultPattern.drawPatternAt(s.get(), Point(2, 3, 4));
  EXPECT_EQ(c, utility::Color(1, 1.5, 2));
}

TEST_F(TestPatternTests, pattern_with_both_object_and_pattern_transformation) {
  defaultPattern.setTransform(transformations::translation(0.5, 1, 1.5));
  const auto s = std::make_shared<MockTestShape>();
  s->setTransform(transformations::scaling(2, 2, 2));

  const auto c = defaultPattern.drawPatternAt(s.get(), Point(2.5, 3, 3.5));
  EXPECT_EQ(c, utility::Color(0.75, 0.5, 0.25));
}

/* =========== Stripe Pattern Tests =========== */
TEST(stripe_pattern_test, stripe_pattern_is_constant_in_y) {
  const auto pattern = material::StripePattern(white, black);
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 1, 0)));
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 2, 0)));
}

TEST(stripe_pattern_test, stripe_pattern_is_constant_in_z) {
  const auto pattern = material::StripePattern(white, black);
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 1)));
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 2)));
}

TEST(stripe_pattern_test, stripe_pattern_alternates_in_x) {
  const auto pattern = material::StripePattern(white, black);
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0.9, 0, 0)));
  EXPECT_EQ(pattern.b, pattern.drawPatternAt(Point(1, 0, 0)));
  EXPECT_EQ(pattern.b, pattern.drawPatternAt(Point(-0.1, 0, 0)));
  EXPECT_EQ(pattern.b, pattern.drawPatternAt(Point(-1, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(-1.1, 0, 0)));
}

/* =========== Gradient Pattern Tests =========== */
TEST(gradient_pattern_test, linear_gradient_pattern) {
  const auto pattern = material::GradientPattern(white, black);
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 0)));
  EXPECT_EQ(Color(0.75, 0.75, 0.75), pattern.drawPatternAt(Point(0.25, 0, 0)));
  EXPECT_EQ(Color(0.5, 0.5, 0.5), pattern.drawPatternAt(Point(0.5, 0, 0)));
  EXPECT_EQ(Color(0.25, 0.25, 0.25), pattern.drawPatternAt(Point(0.75, 0, 0)));
}

/* =========== Ring Pattern Tests =========== */
TEST(ring_pattern_test, ring_pattern_should_extend_in_both_x_and_z) {
  const auto pattern = material::RingPattern(white, black);
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 0)));
  EXPECT_EQ(pattern.b, pattern.drawPatternAt(Point(1, 0, 0)));
  EXPECT_EQ(pattern.b, pattern.drawPatternAt(Point(0, 0, 1)));
  EXPECT_EQ(pattern.b, pattern.drawPatternAt(Point(0.708, 0, 0.708)));
}

/* =========== Checker Pattern Tests =========== */
TEST(checker_pattern_test, checker_pattern_should_repeat_in_x) {
  const auto pattern = material::CheckerPattern(white, black);
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0.99, 0, 0)));
  EXPECT_EQ(pattern.b, pattern.drawPatternAt(Point(1.01, 0, 0)));
}

TEST(checker_pattern_test, checker_pattern_should_repeat_in_y) {
  const auto pattern = material::CheckerPattern(white, black);
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0.99, 0)));
  EXPECT_EQ(pattern.b, pattern.drawPatternAt(Point(0, 1.01, 0)));
}

TEST(checker_pattern_test, checker_pattern_should_repeat_in_z) {
  const auto pattern = material::CheckerPattern(white, black);
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 0)));
  EXPECT_EQ(pattern.a, pattern.drawPatternAt(Point(0, 0, 0.99)));
  EXPECT_EQ(pattern.b, pattern.drawPatternAt(Point(0, 0, 1.01)));
}