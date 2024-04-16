#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

#include "Shape.hpp"
#include "Material.hpp"
#include "Intersections.hpp"
#include "Ray.hpp"
#include "Group.hpp"
#include "Sphere.hpp"

using namespace raytracer;
using namespace utility;

class MockTestShape : public geometry::ShapeBase {
public:
  MOCK_METHOD(std::vector<geometry::Intersection>, localIntersect, (const utility::Ray& ray), (const, noexcept));
  MOCK_METHOD(utility::Tuple, localNormalAt, (const utility::Tuple &point), (const, noexcept));
};

class TestShapeTests : public ::testing::Test {
protected:
  MockTestShape s{};

  TestShapeTests() {
    ON_CALL(s, localNormalAt(testing::_)).WillByDefault(testing::ReturnArg<0>());
  }
};

/* =========== Transform tests =========== */
TEST_F(TestShapeTests, defaultTransformation) {
  EXPECT_EQ(s.transformation(), (Matrix<4,4>::identity()));
}

TEST_F(TestShapeTests, SetTransform) {
  const auto t = transformations::translation(2, 3, 4);

  s.setTransform(t);

  EXPECT_EQ(s.transformation(), t);
}

/* =========== Material tests =========== */
TEST_F(TestShapeTests, DefaultMaterial) {
  EXPECT_TRUE(s.material() == material::Material());
}

TEST_F(TestShapeTests, AssignMaterial) {
  auto m = material::Material();
  m.setAmbient(1);
  s.setMaterial(m);

  EXPECT_TRUE(s.material() == m);
}

/* =========== Intersect tests =========== */
TEST_F(TestShapeTests, IntersectingScaledShapeWithRay) {
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  s.setTransform(transformations::scaling(2, 2, 2));

  auto transformedRay = Ray();
  EXPECT_CALL(s, localIntersect(testing::_)).WillOnce(
              testing::DoAll(testing::SaveArg<0>(&transformedRay),
       testing::Return(std::vector<geometry::Intersection>{})));
  const auto xs = s.intersect(r);

  EXPECT_EQ(transformedRay.origin_, Point(0, 0, -2.5));
  EXPECT_EQ(transformedRay.direction_, Vector(0, 0, 0.5));
}

TEST_F(TestShapeTests, IntersectingTranslatedShapeWithRay) {
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  s.setTransform(transformations::translation(5, 0, 0));

  auto transformedRay = Ray();
  EXPECT_CALL(s, localIntersect(testing::_)).WillOnce(
              testing::DoAll(testing::SaveArg<0>(&transformedRay),
       testing::Return(std::vector<geometry::Intersection>{})));
  const auto xs = s.intersect(r);

  EXPECT_EQ(transformedRay.origin_, Point(-5, 0, -5));
  EXPECT_EQ(transformedRay.direction_, Vector(0, 0, 1));
}

/* =========== NormalAt tests =========== */
TEST_F(TestShapeTests, NormalOnATranslatedShape) {
  s.setTransform(transformations::translation(0, 1, 0));
  auto n = s.normalAt(Point(0, 1+std::sqrt(2)/2, -std::sqrt(2)/2));

  EXPECT_EQ(n, Vector(0, std::sqrt(2)/2, -std::sqrt(2)/2));
}

TEST_F(TestShapeTests, NormalOnATransformedShape) {
  s.setTransform(transformations::scaling(1, 0.5, 1) * transformations::rotation_z(std::numbers::pi/5));
  auto n = s.normalAt(Point(0, std::sqrt(2)/2, -std::sqrt(2)/2));

  EXPECT_EQ(n, Vector(0, 0.9701425, -0.2425356));
}

/* =========== Parent tests =========== */
TEST_F(TestShapeTests, ParentAttributeOfShape) {
  EXPECT_EQ(s.parent(), nullptr);
}

/* =========== Helper functions tests =========== */
TEST(ShapeTests, ConvertingAPointFromWOrldToObjectSpace) {
  auto g1 = std::make_shared<geometry::Group>();
  g1->setTransform(transformations::rotation_y(std::numbers::pi/2));
  auto g2 = std::make_shared<geometry::Group>();
  g2->setTransform(transformations::scaling(2, 2, 2));
  g1->addChild(g2);
  auto s = std::make_shared<geometry::Sphere>();
  s->setTransform(transformations::translation(5, 0, 0));
  g2->addChild(s);

  auto p = worldPointToObjectPoint(s.get(), Point(-2, 0, -10));

  EXPECT_EQ(p, Point(0, 0, -1));
}

TEST(ShapeTests, ConvertingANormalFromObjectToWorldSpace) {
  auto g1 = std::make_shared<geometry::Group>();
  g1->setTransform(transformations::rotation_y(std::numbers::pi/2));
  auto g2 = std::make_shared<geometry::Group>();
  g2->setTransform(transformations::scaling(1, 2, 3));
  g1->addChild(g2);
  auto s = std::make_shared<geometry::Sphere>();
  s->setTransform(transformations::translation(5, 0, 0));
  g2->addChild(s);

  auto n = objectNormalToWorldNormal(s.get(), Vector(std::sqrt(3)/3, std::sqrt(3)/3, std::sqrt(3)/3));

  EXPECT_EQ(n, Vector(0.2857143, 0.4285714, -0.8571429));
}

TEST(ShapeTests, FindingNormalOnChildObject) {
  auto g1 = std::make_shared<geometry::Group>();
  g1->setTransform(transformations::rotation_y(std::numbers::pi/2));
  auto g2 = std::make_shared<geometry::Group>();
  g2->setTransform(transformations::scaling(1, 2, 3));
  g1->addChild(g2);
  auto s = std::make_shared<geometry::Sphere>();
  s->setTransform(transformations::translation(5, 0, 0));
  g2->addChild(s);

  auto n = s->normalAt(Point(1.7321, 1.1547, -5.5774));

  EXPECT_EQ(n, Vector(0.2857036, 0.4285431, -0.8571605));
}