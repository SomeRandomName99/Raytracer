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
#include "Pattern.hpp"
#include "CheckerPattern.hpp"
#include "StripePattern.hpp"
#include "PerturbedPattern.hpp"
#include "RingPattern.hpp"
#include "GradientPattern.hpp"

using namespace raytracer;

int main(){
  auto backWallMaterial = material::Material();
  backWallMaterial.setPattern(material::CheckerPattern(utility::Color(1,1,1), utility::Color(0,0,0)));

  auto floorMaterial = material::Material();
  floorMaterial.setPattern(material::StripePattern(utility::Color(1, 0.9, 0.9), utility::Color(0.9, 1, 0.9)));
  floorMaterial.setSpecular(0);

  auto floor = geometry::makePlane();
  floor->setMaterial(floorMaterial);

  auto wall = geometry::makePlane();
  wall->setMaterial(backWallMaterial);
  wall->setTransform(utility::transformations::translation(0, 0, 6) * 
                     utility::transformations::rotation_x(std::numbers::pi / 2));

  auto middle = geometry::makeSphere();
  middle->setTransform(utility::transformations::translation(-0.5, 1, 0.5)*
                      utility::transformations::rotation_z(std::numbers::pi / 3));
  auto middlePattern = material::Pattern(material::Perturb(material::RingPattern(utility::Color(1,1,1), utility::Color(1,0,0))));
  middlePattern.setTransform(utility::transformations::scaling(0.1, 0.1, 0.1) * 
                              utility::transformations::rotation_x(std::numbers::pi / 2));
                              utility::transformations::rotation_x(std::numbers::pi / 2);
  middle->material().setPattern(middlePattern);
  middle->material().setDiffuse(0.7);
  middle->material().setSpecular(0.3);
  middle->material().setReflectance(0.05);

  auto right = geometry::makeSphere();
  right->setTransform(utility::transformations::translation(1.5, 0.5, -0.5) * 
                     utility::transformations::scaling(0.5, 0.5, 0.5));
  auto rightPattern = material::Pattern(material::GradientPattern(utility::Color(1,0,0), utility::Color(0,1,0)));
  rightPattern.setTransform(utility::transformations::scaling(4, 4, 4) * utility::transformations::translation(0.5, 0, 0));
  right->material().setPattern(rightPattern);
  right->material().setDiffuse(0.7);
  right->material().setSpecular(0.3);
  right->material().setReflectance(0.1);

  auto left = geometry::makeSphere();
  left->setTransform(utility::transformations::translation(-1.5, 0.33, -0.75) * 
                     utility::transformations::scaling(0.33, 0.33, 0.33));
  left->material().setSurfaceColor(utility::Color(1, 0.8, 0.1));
  left->material().setDiffuse(0.7);
  left->material().setSpecular(0.3);

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