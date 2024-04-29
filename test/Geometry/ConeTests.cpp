#include <gtest/gtest.h>
#include <tuple>

#include "Cone.hpp"
#include "Intersections.hpp"
#include "Shape.hpp"

using namespace raytracer;
using namespace utility;
using namespace geometry;

// =================== Ray intersects cone ====================
class ConeIntersectTest : public ::testing::TestWithParam<std::tuple<Tuple, Tuple, float, float>> {
public:
  static std::shared_ptr<Cone> c;
  static std::vector<std::tuple<Tuple, Tuple, float, float>> testCases;
};

std::shared_ptr<Cone> ConeIntersectTest::c = makeCone();
std::vector<std::tuple<Tuple, Tuple, float, float>> ConeIntersectTest::testCases = {
  {Point(0, 0, -5), Vector(0, 0, 1), 5, 5},
  {Point(0, 0, -5), Vector(1, 1, 1), 8.660254, 8.660254},
  {Point(1, 1, -5), Vector(-0.5, -1, 1), 4.550056, 49.449944},
};

INSTANTIATE_TEST_SUITE_P(
  ConeTests,
  ConeIntersectTest,
  ::testing::ValuesIn(ConeIntersectTest::testCases)
);

TEST_P(ConeIntersectTest, rayIntersectsCone)
{
  auto [origin, direction, t0, t1] = GetParam();
  auto ray = Ray(origin, direction.normalize());
  auto xs = c->intersect(ray);
  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(t0, xs.at(0).dist);
  EXPECT_FLOAT_EQ(t1, xs.at(1).dist);
}

TEST(ConeTests, intersectingConeWithRayParallelToOneHalf)
{
  auto c = makeCone();
  auto ray = Ray(Point(0, 0, -1), Vector(0, 1, 1).normalize());
  auto xs = c->intersect(ray);
  EXPECT_EQ(1, xs.size());
  EXPECT_FLOAT_EQ(0.3535533, xs.at(0).dist);
}

class ConeIntersectCapTest : public ::testing::TestWithParam<std::tuple<Tuple, Tuple, int>> {
public:
  static std::shared_ptr<Cone> c;
  static std::vector<std::tuple<Tuple, Tuple, int>> testCases;
};

std::shared_ptr<Cone> ConeIntersectCapTest::c = std::make_shared<Cone>(-0.5, 0.5, true);
std::vector<std::tuple<Tuple, Tuple, int>> ConeIntersectCapTest::testCases = {
  {Point(0, 0, -5), Vector(0, 1, 0), 0},
  {Point(0, 0, -0.25), Vector(0, 1, 1), 2},
  {Point(0, 0, -0.25), Vector(0, 1, 0), 4},
};

INSTANTIATE_TEST_SUITE_P(
  ConeTests,
  ConeIntersectCapTest,
  ::testing::ValuesIn(ConeIntersectCapTest::testCases)
);

TEST_P(ConeIntersectCapTest, rayIntersectsConeCaps)
{
  auto [origin, direction, count] = GetParam();
  auto ray = Ray(origin, direction.normalize());
  auto xs = c->intersect(ray);
  ASSERT_EQ(count, xs.size());
}

// =================== Normal on the surface of the cone ====================
class ConeNormalTest : public ::testing::TestWithParam<std::pair<Tuple, Tuple>> {
public:
  static std::shared_ptr<Cone> c;
  static std::vector<std::pair<Tuple, Tuple>> testCases;
};

std::shared_ptr<Cone> ConeNormalTest::c = makeCone();
std::vector<std::pair<Tuple, Tuple>> ConeNormalTest::testCases = {
  {Point(0, 0, 0), Vector(0, 0, 0)},
  {Point(1, 1, 1), Vector(1, -sqrt(2), 1)},
  {Point(-1, -1, 0), Vector(-1, 1, 0)},
};

INSTANTIATE_TEST_SUITE_P(
  ConeTests,
  ConeNormalTest,
  ::testing::ValuesIn(ConeNormalTest::testCases)
);

TEST_P(ConeNormalTest, normalOnCone)
{
  auto [objectPoint, expectedNormal] = GetParam();
  auto normal = ConeNormalTest::c->localNormalAt(objectPoint);
  EXPECT_TRUE(expectedNormal == normal);
}