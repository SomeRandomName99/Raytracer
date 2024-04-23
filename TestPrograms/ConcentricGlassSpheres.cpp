#include <fstream>
#include <optional>

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

using namespace raytracer;

int main(){
  auto floorMaterial = material::Material();
  floorMaterial.setPattern(material::CheckerPattern(utility::Color(0.85, 0.85, 0.85), utility::Color(0.15, 0.15, 0.15)));
  floorMaterial.setAmbient(0.8);
  floorMaterial.setDiffuse(0.2);
  floorMaterial.setSpecular(0);

  auto glassMaterial = material::Material(utility::Color(1,1,1), 0, 0, 0.9, 300, 0.9, 0.9, 1.5);

  auto floor = geometry::makePlane();
  floor->setMaterial(floorMaterial);
  floor->setTransform(utility::transformations::translation(0, 0, 10) * 
                      utility::transformations::rotation_x(std::numbers::pi / 2));

  auto glassSphere = geometry::makeSphere();
  glassSphere->setMaterial(glassMaterial);

  auto hollowCenter = geometry::glassSphere();
  hollowCenter->setTransform(utility::transformations::scaling(0.5, 0.5, 0.5));
  hollowCenter->setMaterial(glassMaterial);
  hollowCenter->material().setRefractiveIndex(1.0000034); // because there is air inside of it

  scene::World world;
  world.objects_.emplace_back(floor);
  world.objects_.emplace_back(glassSphere);
  world.objects_.emplace_back(hollowCenter);
  world.lights_.emplace_back(scene::PointLight(utility::Color(0.9, 0.9, 0.9), utility::Point(2, 10, -5)));

  auto camera = scene::Camera(5000, 5000, 0.45);
  camera.transform_ = utility::transformations::view_transform(utility::Point( 0, 0, -5),
                                                               utility::Point( 0, 0,  0), 
                                                               utility::Vector(0, 1,  0));
  auto canvas = camera.render(world);

  std::ofstream image{"ConcentricGlassSpheres.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);
  return 0;
}