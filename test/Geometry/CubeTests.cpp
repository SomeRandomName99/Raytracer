#include <gtest/gtest.h>
#include <unordered_map>
#include <tuple>

#include "Cube.hpp"
#include "Intersections.hpp"
#include "Shape.hpp"

using namespace raytracer;
using namespace utility;
using namespace geometry;

// =================== Ray intersects each cube face ====================

class CubeFaceIntersectTest : public ::testing::TestWithParam<std::pair<const std::string, std::tuple<Ray, std::vector<Intersection>>>> {
public:
  static std::shared_ptr<Cube> c;
  static std::unordered_map<std::string, std::tuple<Ray, std::vector<Intersection>>> testCases; 
};

std::shared_ptr<Cube> CubeFaceIntersectTest::c = normalCube();
std::unordered_map<std::string, std::tuple<Ray, std::vector<Intersection>>> CubeFaceIntersectTest::testCases = {
  {"positiveXSide",     {Ray(Point(5, 0.5, 0), Vector(-1, 0, 0)), {Intersection(c.get(), 4), Intersection(c.get(), 6)}}},
  {"negativeXSide",     {Ray(Point(-5, 0.5, 0), Vector(1, 0, 0)), {Intersection(c.get(), 4), Intersection(c.get(), 6)}}},
  {"positiveYSide",     {Ray(Point(0.5, 5, 0), Vector(0, -1, 0)), {Intersection(c.get(), 4), Intersection(c.get(), 6)}}},
  {"negativeYSide",     {Ray(Point(0.5, -5, 0), Vector(0, 1, 0)), {Intersection(c.get(), 4), Intersection(c.get(), 6)}}},
  {"positiveZSide",     {Ray(Point(0.5, 0, 5), Vector(0, 0, -1)), {Intersection(c.get(), 4), Intersection(c.get(), 6)}}},
  {"negativeZSide",     {Ray(Point(0.5, 0, -5), Vector(0, 0, 1)), {Intersection(c.get(), 4), Intersection(c.get(), 6)}}},
  {"inside",            {Ray(Point(0, 0.5, 0), Vector(0, 0, 1)), {Intersection(c.get(), -1), Intersection(c.get(), 1)}}},
};

INSTANTIATE_TEST_SUITE_P(
  CubeTests,
  CubeFaceIntersectTest,
  ::testing::ValuesIn(CubeFaceIntersectTest::testCases),
  [](const auto& info) { return info.param.first; }
);

TEST_P(CubeFaceIntersectTest, rayIntersectsCube)
{
  auto [name, data] = GetParam();
  auto [ray, expectedIntersections] = data;
  auto xs = c->intersect(ray);
  ASSERT_EQ(expectedIntersections.size(), xs.size());
  for (size_t i = 0; i < xs.size(); ++i) {
    EXPECT_FLOAT_EQ(expectedIntersections.at(i).dist, xs.at(i).dist);
    EXPECT_TRUE(expectedIntersections.at(i).object == xs.at(i).object);
  }
}

// =================== Ray misses the cube ====================
class CubeMissTest : public ::testing::TestWithParam<Ray> {
public:
  static std::shared_ptr<Cube> c;
  static std::vector<Ray> testCases;
};

std::shared_ptr<Cube> CubeMissTest::c = normalCube();
std::vector<Ray> CubeMissTest:: testCases = {
  Ray(Point(-2, 0, 0), Vector(0.2673, 0.5345, 0.8018)),
  Ray(Point(0, -2, 0), Vector(0.8018, 0.2673, 0.5345)),
  Ray(Point(0, 0, -2), Vector(0.5345, 0.8018, 0.2673)),
  Ray(Point(2, 0, 2), Vector(0, 0, -1)),
  Ray(Point(0, 2, 2), Vector(0, -1, 0)),
  Ray(Point(2, 2, 0), Vector(-1, 0, 0)),
};

INSTANTIATE_TEST_SUITE_P(
  CubeTests,
  CubeMissTest,
  ::testing::ValuesIn(CubeMissTest::testCases)
);

TEST_P(CubeMissTest, rayMissesCube)
{
  auto xs = CubeMissTest::c->intersect(GetParam());
  ASSERT_EQ(0, xs.size());
}


// =================== Normal on the surface of the cube ====================
class CubeNormalTest : public ::testing::TestWithParam<std::pair<Tuple, Tuple>> {
public:
  static std::shared_ptr<Cube> c;
  static std::vector<std::pair<Tuple, Tuple>> testCases;
};

std::shared_ptr<Cube> CubeNormalTest::c = normalCube();
std::vector<std::pair<Tuple, Tuple>> CubeNormalTest::testCases = {
  {Point(1, 0.5, -0.8), Vector(1, 0, 0)},
  {Point(-1, -0.2, 0.9), Vector(-1, 0, 0)},
  {Point(-0.4, 1, -0.1), Vector(0, 1, 0)},
  {Point(0.3, -1, -0.7), Vector(0, -1, 0)},
  {Point(-0.6, 0.3, 1), Vector(0, 0, 1)},
  {Point(0.4, 0.4, -1), Vector(0, 0, -1)},
  {Point(1, 1, 1), Vector(1, 0, 0)},
  {Point(-1, -1, -1), Vector(-1, 0, 0)},
};

INSTANTIATE_TEST_SUITE_P(
  CubeTests,
  CubeNormalTest,
  ::testing::ValuesIn(CubeNormalTest::testCases)
);

TEST_P(CubeNormalTest, normalOnSurface)
{
  auto [point, expectedNormal] = GetParam();
  auto normal = CubeNormalTest::c->normalAt(point);
  EXPECT_TRUE(expectedNormal == normal);
}