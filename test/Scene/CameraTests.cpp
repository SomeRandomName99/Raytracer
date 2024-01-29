#include <gtest/gtest.h>
#include <numbers>

#include "Camera.hpp"
#include "Transformations.hpp"

using namespace raytracer;
using namespace scene;

TEST(camera_tests, ConstructingCamera){
  const auto numHorPixels = 160;
  const auto numVerPixels = 120;
  const auto fov = std::numbers::pi / 2;
  const auto c = scene::Camera(numHorPixels, numVerPixels, fov);

  EXPECT_EQ(c.numHorPixels_, 160);
  EXPECT_EQ(c.numVerPixels_, 120);
  EXPECT_FLOAT_EQ(c.fov_, std::numbers::pi / 2);
  EXPECT_EQ(c.transform_, (utility::Matrix<4,4>::identity()));
}

TEST(camera_tests, horizontalCanvas){
  const auto c = scene::Camera(200, 125, std::numbers::pi / 2);

  EXPECT_FLOAT_EQ(c.pixelSize_, 0.01);
}

TEST(camera_tests, verticalCanvas){
  const auto c = scene::Camera(125, 200, std::numbers::pi / 2);

  EXPECT_FLOAT_EQ(c.pixelSize_, 0.01);
}

// ================== Ray Construction Tests ==================
TEST(ray_construction_tests, rayThroughCenterOfCanvas){
  const auto c = scene::Camera(201, 101, std::numbers::pi / 2);
  const auto r = c.rayForPixel(100, 50);

  EXPECT_EQ(r.origin_, utility::Point(0,0,0));
  EXPECT_EQ(r.direction_, utility::Vector(0,0,-1));
}

TEST(ray_construction_tests, rayThroughCornerOfCanvas){
  const auto c = scene::Camera(201, 101, std::numbers::pi / 2);
  const auto r = c.rayForPixel(0, 0);

  EXPECT_EQ(r.origin_, utility::Point(0,0,0));
  EXPECT_EQ(r.direction_, utility::Vector(0.665186426, 0.332593213, -0.6685123582));
}

TEST(ray_construction_tests, rayWhenCameraIsTransformed){
  auto c = scene::Camera(201, 101, std::numbers::pi / 2);
  c.transform_ = utility::transformations::rotation_y(std::numbers::pi / 4) * utility::transformations::translation(0, -2, 5);
  const auto r = c.rayForPixel(100, 50);

  EXPECT_EQ(r.origin_, utility::Point(0,2,-5));
  EXPECT_EQ(r.direction_, utility::Vector(std::sqrt(2)/2, 0, -std::sqrt(2)/2));
}

// ================== Rendering Tests ==================
TEST(rendering_tests, renderingWorldWithCamera){
  const auto w = scene::defaultWorld();
  auto c = scene::Camera(11, 11, std::numbers::pi / 2);
  const auto from = utility::Point(0,0,-5);
  const auto to   = utility::Point(0,0,0);
  const auto up   = utility::Vector(0,1,0);
  c.transform_    = utility::transformations::view_transform(from, to, up);
  const auto image = c.render(w);

  EXPECT_EQ(image.pixelAt(5,5), utility::Color(0.380661107, 0.4758263837, 0.28549583));
}