#include <gtest/gtest.h>
#include <utility>
#include <vector>
#include <unordered_map>


#include "Cylinder.hpp"
#include "Intersections.hpp"
#include "Shape.hpp"
#include "Arena.hpp"

using namespace raytracer;
using namespace utility;
using namespace geometry;

class CylinderTest : public ::testing::Test {
protected:
  Arena<geometry::Intersection> xs;
  
  CylinderTest() : xs(MB(1)) {}
  
  void SetUp() override {
    // Clear arena before each test to ensure clean state
    xs.clear();
  }
  
  void TearDown() override {}
};

// =================== Ray intersects cylinder ====================
class CylinderMissTest : public CylinderTest, public ::testing::WithParamInterface<std::pair<Tuple, Tuple>> {
public:
  static std::shared_ptr<Cylinder> c;
  static std::vector<std::pair<Tuple, Tuple>> testCases;
};

std::shared_ptr<Cylinder> CylinderMissTest::c = makeCylinder();
std::vector<std::pair<Tuple, Tuple>> CylinderMissTest::testCases = {
  {Point(1, 0, 0),  Vector(0, 1, 0)},
  {Point(0, 0, 0),  Vector(0, 1, 0)},
  {Point(0, 0, -5), Vector(1, 1, 1)},
};

INSTANTIATE_TEST_SUITE_P(
  CylinderTests,
  CylinderMissTest,
  ::testing::ValuesIn(CylinderMissTest::testCases)
);

TEST_P(CylinderMissTest, rayMissesCylinder)
{
  auto [origin, direction] = GetParam();
  auto ray = Ray(origin, direction.normalize());
  c->intersect(ray, xs);
  EXPECT_EQ(0, xs.size);
}

class CylinderIntersectTest : public CylinderTest, public ::testing::WithParamInterface<std::tuple<Tuple, Tuple, double, double>>{
public:
  static std::shared_ptr<Cylinder> c;
  static std::vector<std::tuple<Tuple, Tuple, double, double>> testCases;
};

std::shared_ptr<Cylinder> CylinderIntersectTest::c = makeCylinder();
std::vector<std::tuple<Tuple, Tuple, double, double>> CylinderIntersectTest::testCases = {
  {Point(1, 0, -5), Vector(0, 0, 1), 5, 5},
  {Point(0, 0, -5), Vector(0, 0, 1), 4, 6},
  {Point(0.5, 0, -5), Vector(0.1, 1, 1), 6.807982, 7.0887232},
};

INSTANTIATE_TEST_SUITE_P(
  CylinderTests,
  CylinderIntersectTest,
  ::testing::ValuesIn(CylinderIntersectTest::testCases)
);

TEST_P(CylinderIntersectTest, rayIntersectsCylinder)
{
  auto [origin, direction, t0, t1] = GetParam();
  auto ray = Ray(origin, direction.normalize());
  c->intersect(ray, xs);
  EXPECT_EQ(2, xs.size);
  EXPECT_FLOAT_EQ(t0, xs[0].dist);
  EXPECT_FLOAT_EQ(t1, xs[1].dist);
}

// =================== Normal on the surface of the cylinder ====================
class CylinderNormalTest : public ::testing::TestWithParam<std::pair<Tuple, Tuple>> {
public:
  static std::shared_ptr<Cylinder> c;
  static std::vector<std::pair<Tuple, Tuple>> testCases;
};

std::shared_ptr<Cylinder> CylinderNormalTest::c = makeCylinder();
std::vector<std::pair<Tuple, Tuple>> CylinderNormalTest::testCases = {
  {Point(1, 0, 0),  Vector(1, 0, 0)},
  {Point(0, 5, -1), Vector(0, 0, -1)},
  {Point(0, -2, 1), Vector(0, 0, 1)},
  {Point(-1, 1, 0), Vector(-1, 0, 0)},
};

INSTANTIATE_TEST_SUITE_P(
  CylinderTests,
  CylinderNormalTest,
  ::testing::ValuesIn(CylinderNormalTest::testCases)
);

TEST_P(CylinderNormalTest, normalOnCylinder)
{
  auto [point, normal] = GetParam();
  auto n = c->normalAt(point);
  EXPECT_EQ(normal, n);
}

// =================== Truncated cylinder tests ====================
TEST(CylinderTests, DefaultCylinderIsNotTruncated){
  auto cylinder = makeCylinder();
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), cylinder->minimum_);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), cylinder->maximum_);
}

class CylinderWithTruncationIntersectionTest : public CylinderTest, public ::testing::WithParamInterface<std::pair<const std::string, std::tuple<Tuple, Tuple, double>>>{
public:
  static std::shared_ptr<Cylinder> c;
  static std::unordered_map<std::string, std::tuple<Tuple, Tuple, double>> testCases;
};

std::shared_ptr<Cylinder> CylinderWithTruncationIntersectionTest::c = std::make_shared<Cylinder>(1, 2);
std::unordered_map<std::string, std::tuple<Tuple, Tuple, double>> CylinderWithTruncationIntersectionTest::testCases = {
  {"RayFromInsideEscapesThroughTheTop", {Point(0, 1.5, 0), Vector(0.1, 1, 0), 0}},
  {"RayPerpendicularToYAxisAboveCylinder", {Point(0, 3, -5), Vector(0, 0, 1), 0}},
  {"RayPerpendicularToYAxisBelowCylinder", {Point(0, 0, -5), Vector(0, 0, 1), 0}},
  {"PerpendicularToTopEdge", {Point(0, 2, -5), Vector(0, 0, 1), 0}},
  {"PerpendicularToBottomEdge", {Point(0, 1, -5), Vector(0, 0, 1), 0}},
  {"RayIntersectsThroughTheMiddle", {Point(0, 1.5, -2), Vector(0, 0, 1), 2}},
};

INSTANTIATE_TEST_SUITE_P(
  CylinderTests,
  CylinderWithTruncationIntersectionTest,
  ::testing::ValuesIn(CylinderWithTruncationIntersectionTest::testCases),
  [](const auto& info) { return info.param.first; }
);

TEST_P(CylinderWithTruncationIntersectionTest, CylinderWithTruncation){
  auto [name, testData] = GetParam();
  auto[origin, direction, intersectionCount] = testData;
  auto cylinder = std::make_shared<Cylinder>(1, 2);
  EXPECT_FLOAT_EQ(1, cylinder->minimum_);
  EXPECT_FLOAT_EQ(2, cylinder->maximum_);

  auto ray = Ray(origin, direction.normalize());
  cylinder->intersect(ray, xs);
  EXPECT_EQ(intersectionCount, xs.size);
}

// =================== Closed cylinder tests ====================
TEST(CylinderTests, DefaultCylinderNotClosed){
  auto cylinder = makeCylinder();
  EXPECT_FALSE(cylinder->closed_);
}

class ClosedCylinderIntersectionTest : public CylinderTest, public ::testing::WithParamInterface<std::pair<const std::string, std::tuple<Tuple, Tuple, double>>>{
public:
  static std::shared_ptr<Cylinder> c;
  static std::unordered_map<std::string, std::tuple<Tuple, Tuple, double>> testCases;
};

std::shared_ptr<Cylinder> ClosedCylinderIntersectionTest::c = std::make_shared<Cylinder>(1, 2, true);
std::unordered_map<std::string, std::tuple<Tuple, Tuple, double>> ClosedCylinderIntersectionTest::testCases = {
  {"RayAboveTheCylinderGoesThroughCaps", {Point(0, 3, 0), Vector(0, -1, 0), 2}},
  {"RayAboveTheCylinderAndGoesDiagonallyThrough", {Point(0, 3, -2), Vector(0, -1, 2), 2}},
  {"RayBelowTheCylinderAndGoesDiagonallyThrough", {Point(0, 0, -2), Vector(0,  1, 2), 2}},
  {"RayAboveCylinderExitThourghBottomCorner", {Point(0,  4, -2), Vector(0, -1, 1), 2}},
  {"RayBelowCylinderExitThourghTopCorner",    {Point(0, -1, -2), Vector(0,  1, 1), 2}},
};

INSTANTIATE_TEST_SUITE_P(
  CylinderTests,
  ClosedCylinderIntersectionTest,
  ::testing::ValuesIn(ClosedCylinderIntersectionTest::testCases),
  [](const auto& info) { return info.param.first; }
);

TEST_P(ClosedCylinderIntersectionTest, ClosedCylinder){
  auto [name, testData] = GetParam();
  auto[origin, direction, intersectionCount] = testData;
  auto cylinder = std::make_shared<Cylinder>(1, 2, true);
  EXPECT_FLOAT_EQ(1, cylinder->minimum_);
  EXPECT_FLOAT_EQ(2, cylinder->maximum_);
  EXPECT_TRUE(cylinder->closed_);

  auto ray = Ray(origin, direction.normalize());
  cylinder->intersect(ray, xs);
  EXPECT_EQ(intersectionCount, xs.size);
}

// =================== Normal on the surface of the closed cylinder ====================
class ClosedCylinderNormalTest : public ::testing::TestWithParam<std::pair<Tuple, Tuple>> {
public:
  static std::shared_ptr<Cylinder> c;
  static std::vector<std::pair<Tuple, Tuple>> testCases;
};

std::shared_ptr<Cylinder> ClosedCylinderNormalTest::c = std::make_shared<Cylinder>(1, 2, true);
std::vector<std::pair<Tuple, Tuple>> ClosedCylinderNormalTest::testCases = {
  {Point(0, 1, 0),  Vector(0, -1, 0)},
  {Point(0.5, 1, 0), Vector(0, -1, 0)},
  {Point(0, 1, 0.5), Vector(0, -1, 0)},
  {Point(0, 2, 0), Vector(0, 1, 0)},
  {Point(0.5, 2, 0), Vector(0, 1, 0)},
  {Point(0, 2, 0.5), Vector(0, 1, 0)},
};

INSTANTIATE_TEST_SUITE_P(
  CylinderTests,
  ClosedCylinderNormalTest,
  ::testing::ValuesIn(ClosedCylinderNormalTest::testCases)
);

TEST_P(ClosedCylinderNormalTest, normalOnClosedCylinder)
{
  auto [point, normal] = GetParam();
  auto n = c->normalAt(point);
  EXPECT_EQ(normal, n);
}