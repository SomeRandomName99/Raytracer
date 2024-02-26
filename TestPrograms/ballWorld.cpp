#include <fstream>
#include <numbers>

#include "Tuple.hpp"
#include "Canvas.hpp"
#include "Transformations.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Intersections.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "World.hpp"
#include "Camera.hpp"

using namespace raytracer;

int main(){
  auto wallMaterial = material::Material(utility::Color(1, 0.9, 0.9));
  wallMaterial.specular_ = 0;

  auto floor = geometry::Sphere();
  floor.setTransform(utility::transformations::scaling(10, 0.01, 10));
  floor.material_ = wallMaterial;

  auto leftWall = geometry::Sphere();
  leftWall.setTransform(utility::transformations::translation(0, 0, 5) * 
                             utility::transformations::rotation_y(-std::numbers::pi / 4) * 
                             utility::transformations::rotation_x(std::numbers::pi / 2) * 
                             utility::transformations::scaling(10, 0.01, 10));
  leftWall.material_ = wallMaterial;

  auto rightWall = geometry::Sphere();
  rightWall.setTransform(utility::transformations::translation(0, 0, 5) * 
                              utility::transformations::rotation_y(std::numbers::pi / 4) * 
                              utility::transformations::rotation_x(std::numbers::pi / 2) * 
                              utility::transformations::scaling(10, 0.01, 10));
  rightWall.material_ = wallMaterial;

  auto middle = geometry::Sphere();
  middle.setTransform(utility::transformations::translation(-0.5, 1, 0.5));
  middle.material_.surfaceColor_ = utility::Color(0.1, 1, 0.5);
  middle.material_.diffuse_ = 0.7;
  middle.material_.specular_ = 0.3;

  auto right = geometry::Sphere();
  right.setTransform(utility::transformations::translation(1.5, 0.5, -0.5) * 
                          utility::transformations::scaling(0.5, 0.5, 0.5));
  right.material_.surfaceColor_ = utility::Color(0.5, 1, 0.1);
  right.material_.diffuse_ = 0.7;
  right.material_.specular_ = 0.3;

  auto left = geometry::Sphere();
  left.setTransform(utility::transformations::translation(-1.5, 0.33, -0.75) * 
                         utility::transformations::scaling(0.33, 0.33, 0.33));
  left.material_.surfaceColor_ = utility::Color(1, 0.8, 0.1);
  left.material_.diffuse_ = 0.7;
  left.material_.specular_ = 0.3;

  scene::World world;
  world.lights_.push_back(scene::PointLight(utility::Color(1, 1, 1), utility::Point(-10, 10, -10)));
  world.objects_.emplace_back(floor);
  world.objects_.emplace_back(leftWall);
  world.objects_.emplace_back(rightWall);
  world.objects_.emplace_back(middle);
  world.objects_.emplace_back(right);
  world.objects_.emplace_back(left);


  auto camera = scene::Camera(1920, 1080, std::numbers::pi / 3);
  camera.transform_ = utility::transformations::view_transform(utility::Point(0, 1.5, -5),
                                                              utility::Point(0, 1, 0), 
                                                              utility::Vector(0, 1, 0));
  auto canvas = camera.render(world);

  std::ofstream image{"BallWorld.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);
  return 0;
}