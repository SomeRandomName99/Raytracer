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
#include "Plane.hpp"

using namespace raytracer;

int main(){
  auto wallMaterial = material::Material();
  wallMaterial.setPattern(material::StripePattern(utility::Color(1, 0.9, 0.9), utility::Color(0.9, 1, 0.9)));
  wallMaterial.setSpecular(0);

  auto floor = geometry::Plane();
  floor.material_ = wallMaterial;

  auto wall = geometry::Plane();
  wall.material_ = wallMaterial;
  wall.setTransform(utility::transformations::translation(0, 0, 6) * 
                    utility::transformations::rotation_x(std::numbers::pi / 2));

  auto middle = geometry::Sphere();
  middle.setTransform(utility::transformations::translation(-0.5, 1, 0.5)*
                      utility::transformations::rotation_z(std::numbers::pi / 3));
  middle.material_.setPattern(material::StripePattern(utility::Color(1, 0.9, 0.9), utility::Color(0.9, 1, 0.9)));
  middle.material_.setDiffuse(0.7);
  middle.material_.setSpecular(0.3);

  auto right = geometry::Sphere();
  right.setTransform(utility::transformations::translation(1.5, 0.5, -0.5) * 
                     utility::transformations::scaling(0.5, 0.5, 0.5));
  right.material_.setSurfaceColor(utility::Color(0.5, 1, 0.1));
  right.material_.setDiffuse(0.7);
  right.material_.setSpecular(0.3);

  auto left = geometry::Sphere();
  left.setTransform(utility::transformations::translation(-1.5, 0.33, -0.75) * 
                    utility::transformations::scaling(0.33, 0.33, 0.33));
  left.material_.setSurfaceColor(utility::Color(1, 0.8, 0.1));
  left.material_.setDiffuse(0.7);
  left.material_.setSpecular(0.3);

  scene::World world;
  world.lights_.push_back(scene::PointLight(utility::Color(1, 1, 1), utility::Point(-10, 10, -10)));
  world.objects_.emplace_back(middle);
  world.objects_.emplace_back(right);
  world.objects_.emplace_back(left);
  world.objects_.emplace_back(floor);
  world.objects_.emplace_back(wall);

  auto camera = scene::Camera(1920, 1080, std::numbers::pi / 3);
  camera.transform_ = utility::transformations::view_transform(utility::Point(0, 1.5, -5),
                                                              utility::Point(0, 1, 0), 
                                                              utility::Vector(0, 1, 0));
  auto canvas = camera.render(world);

  std::ofstream image{"BallWorld.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);
  return 0;
}