#include <fstream>
#include <numbers>

#include "libraries/Utility/include/Tuple.hpp"
#include "libraries/Canvas/include/Canvas.hpp"
#include "libraries/Utility/include/Transformations.hpp"
#include "libraries/Utility/include/Ray.hpp"
#include "libraries/Geometry/include/Intersections.hpp"
#include "libraries/Scene/include/Light.hpp"
#include "libraries/Material/include/Material.hpp"
#include "libraries/Scene/include/Camera.hpp"
#include "libraries/Material/include/Pattern.hpp"
#include "libraries/Scene/include/World.hpp"
#include "libraries/Geometry/include/Shape.hpp"

using namespace raytracer;
using namespace material;
using namespace geometry;
using namespace scene;

int main(){
  World world;

  auto floorPattern = Pattern{PatternType::Checker, 
                              PatternData{utility::Color(0.85, 0.85, 0.85), utility::Color(0.15, 0.15, 0.15)}};
  auto floorMaterial = createDefaultMaterial();
  floorMaterial.ambient = 0.8;
  floorMaterial.diffuse = 0.2;
  floorMaterial.specular = 0;

  auto glassMaterial = material::Material(utility::Color(1,1,1), 0, 0, 0.9, 300, 0.9, 0.9, 1.5);

  auto floor = WorldObject{ShapeTypeTag{ShapeType::Plane}};
  auto floorIndex = addObjectWithMaterial(world, floor, floorMaterial, floorPattern);
  addTransformToObject(world, floorIndex, utility::transformations::translation(0, 0, 10) * 
                                          utility::transformations::rotation_x(std::numbers::pi / 2));

  auto glassSphere = WorldObject{ShapeTypeTag{ShapeType::Sphere}};
  addObjectWithMaterial(world, glassSphere, glassMaterial);

  auto hollowCenter = WorldObject{ShapeTypeTag{ShapeType::Sphere}};
  glassMaterial.refractiveIndex = 1.0000034; // because there is air inside of it
  auto hollowCenterIndex = addObjectWithMaterial(world, hollowCenter, glassMaterial);
  addTransformToObject(world, hollowCenterIndex, utility::transformations::scaling(0.5, 0.5, 0.5));

  addLight(world, scene::PointLight(utility::Color(0.9, 0.9, 0.9), utility::Point(2, 10, -5)));

  auto camera = scene::Camera(5000, 5000, 0.45);
  camera.setTransform(utility::transformations::view_transform(utility::Point( 0, 0, -5),
                                                               utility::Point( 0, 0,  0), 
                                                               utility::Vector(0, 1,  0)));
  auto canvas = camera.render(world);

  std::ofstream image{"ConcentricGlassSpheres.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);
  return 0;
}