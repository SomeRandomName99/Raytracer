#include <gtest/gtest.h>
#include <utility>
#include <vector>


#include "Cylinder.hpp"
#include "Intersections.hpp"
#include "Shape.hpp"

using namespace raytracer;
using namespace utility;
using namespace geometry;

// =================== Ray intersects cylinder ====================
class CylinderMissTest : public ::testing::TestWithParam<std::pair<Tuple, Tuple>> {
public:
  static std::shared_ptr<Cylinder> c;
  static std::vector<std::pair<Tuple, Tuple>> testCases;
};

std::shared_ptr<Cylinder> CylinderMissTest::c = normalCylinder();
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
  auto xs = c->intersect(ray);
  EXPECT_EQ(0, xs.size());
}

class CylinderIntersectTest : public ::testing::TestWithParam<std::tuple<Tuple, Tuple, double, double>>{
public:
  static std::shared_ptr<Cylinder> c;
  static std::vector<std::tuple<Tuple, Tuple, double, double>> testCases;
};

std::shared_ptr<Cylinder> CylinderIntersectTest::c = normalCylinder();
std::vector<std::tuple<Tuple, Tuple, double, double>> CylinderIntersectTest::testCases = {
  {Point(1, 0, -5), Vector(0, 0, 1), 5, 5},
  {Point(0, 0, -5), Vector(0, 0, 1), 4, 6},
  {Point(0.5, 0, -5), Vector(0.1, 1, 1), 6.807981, 7.088723},
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
  auto xs = c->intersect(ray);
  EXPECT_EQ(2, xs.size());
  EXPECT_FLOAT_EQ(t0, xs.at(0).dist);
  EXPECT_FLOAT_EQ(t1, xs.at(1).dist);
}

// =================== Normal on the surface of the cylinder ====================
class CylinderNormalTest : public ::testing::TestWithParam<std::pair<Tuple, Tuple>> {
public:
  static std::shared_ptr<Cylinder> c;
  static std::vector<std::pair<Tuple, Tuple>> testCases;
};

std::shared_ptr<Cylinder> CylinderNormalTest::c = normalCylinder();
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