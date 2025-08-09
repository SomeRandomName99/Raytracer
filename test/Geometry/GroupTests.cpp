#include <gtest/gtest.h>
#include <vector>

#include "Group.hpp"
#include "Intersections.hpp"
#include "Shape.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Arena.hpp"

using namespace raytracer;
using namespace utility;
using namespace geometry;

class GroupTest : public ::testing::Test {
protected:
  Arena<geometry::Intersection> xs;
  
  GroupTest() : xs(MB(1)) {}
  
  void SetUp() override {
    // Clear arena before each test to ensure clean state
    xs.clear();
  }
  
  void TearDown() override {}
};

// =================== group creating tests ====================
TEST(GroupTests, createGroup)
{
  auto g = makeGroup();
  EXPECT_EQ(0, g->getChildren().size());
  EXPECT_EQ((Matrix<4,4>::identity()), g->transformation());
}

TEST(GroupTests, addChildToGroup)
{
  auto g = makeGroup();
  auto s = makeSphere();
  g->addChild(s);
  EXPECT_EQ(1, g->getChildren().size());
  EXPECT_EQ(s, g->getChildren().at(0));
  EXPECT_EQ(s->parent(), g);
}

// =================== group intersect tests ====================
TEST_F(GroupTest, intersectingRayWithEmptyGroup)
{
  auto g = makeGroup();
  auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  g->intersect(r, xs);
  EXPECT_EQ(0, xs.size);
}

TEST_F(GroupTest, intersectingRayWithNonEmptyGroup)
{
  auto g = makeGroup();
  auto s1 = makeSphere();
  auto s2 = makeSphere();
  s2->setTransform(transformations::translation(0, 0, -3));
  auto s3 = makeSphere();
  s3->setTransform(transformations::translation(5, 0, 0));
  g->addChild(s1);
  g->addChild(s2);
  g->addChild(s3);
  auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  g->intersect(r, xs);
  ASSERT_EQ(4, xs.size);
  EXPECT_EQ(s2.get(), xs[0].object);
  EXPECT_EQ(s2.get(), xs[1].object);
  EXPECT_EQ(s1.get(), xs[2].object);
  EXPECT_EQ(s1.get(), xs[3].object);
}

TEST_F(GroupTest, intersectingTransformedGroup)
{
  auto g = makeGroup();
  g->setTransform(transformations::scaling(2, 2, 2));
  auto s = makeSphere();
  s->setTransform(transformations::translation(5, 0, 0));
  g->addChild(s);
  auto r = Ray(Point(10, 0, -10), Vector(0, 0, 1));
  g->intersect(r, xs);
  ASSERT_EQ(2, xs.size);
  EXPECT_EQ(s.get(), xs[0].object);
  EXPECT_EQ(s.get(), xs[1].object);
}

TEST(GroupTests, localNormalAtNotSupported){
  auto g = makeGroup();
  auto s = makeSphere();
  g->addChild(s);
  EXPECT_THROW(g->localNormalAt(Point(0, 0, 0)), std::logic_error);
}

TEST(GroupTests, BoundingBoxTest){
  auto s1 = makeSphere();
  s1->setTransform(transformations::translation(2, 5, -3) * transformations::scaling(2, 2, 2));

  auto s2 = makeCylinder(-2, 2, true);
  s2->setTransform(transformations::translation(-4, -1, 4) * transformations::scaling(0.5, 1, 0.5));
  
  auto g = makeGroup();
  g->addChild(s1);
  g->addChild(s2);
  
  auto bbox = g->getBoundingBox();
  EXPECT_EQ(Point(-4.5, -3, -5), bbox.min);
  EXPECT_EQ(Point(4, 7, 4.5), bbox.max);
}