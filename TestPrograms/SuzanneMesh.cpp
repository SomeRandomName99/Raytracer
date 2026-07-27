#include <fstream>
#include <iostream>

#include "libraries/Canvas/include/Canvas.hpp"
#include "libraries/Material/include/Material.hpp"
#include "libraries/Scene/include/Camera.hpp"
#include "libraries/Scene/include/Light.hpp"
#include "libraries/Scene/include/World.hpp"
#include "libraries/Utility/include/Transformations.hpp"
#include "libraries/Utility/include/Tuple.hpp"

using namespace raytracer;
using namespace material;
using namespace geometry;
using namespace scene;

// Loads the Suzanne model from an OBJ file and renders it. This exercises the
// tinyobjloader based mesh loading, the quad triangulation, the mesh shape
// intersection over the triangle range, and the smooth vertex normals.
int main() {
  World world;

  const auto meshIndex = loadMeshFromObjFile(world, "suzanne.obj");
  if (!meshIndex.has_value()) {
    std::cerr << "Could not load suzanne.obj, run this from the repository root\n";
    return 1;
  }

  auto meshMaterial = material::Material(utility::Color(0.9f, 0.6f, 0.2f), // surface color
                                         0.1f,                             // ambient
                                         0.9f,                             // diffuse
                                         0.9f,                             // specular
                                         200.0f,                           // shininess
                                         0.0f,                             // reflectance
                                         0.0f,                             // transparency
                                         1.0f);                            // refractive index
  const size_t materialIndex = addMaterial(world, meshMaterial);
  world.objects[*meshIndex].MaterialIndex = static_cast<int16_t>(materialIndex);

  // Aim the camera at the center of the mesh's bounding box
  const auto &boundingBox = world.objects[*meshIndex].boundingBox;
  const auto center = utility::Point((boundingBox.min.x + boundingBox.max.x) / 2.0f,
                                     (boundingBox.min.y + boundingBox.max.y) / 2.0f,
                                     (boundingBox.min.z + boundingBox.max.z) / 2.0f);

  addLight(world, scene::PointLight{utility::Color(1.0f, 1.0f, 1.0f),
                                    center + utility::Vector(-8.0f, 8.0f, 8.0f)});

  // The model faces towards positive z, so the camera is placed on that side
  auto camera = scene::Camera(1000, 1000, 1.0f);
  camera.setTransform(utility::transformations::view_transform(center + utility::Vector(0.0f, 0.0f, 5.0f), center,
                                                               utility::Vector(0.0f, 1.0f, 0.0f)));

  auto canvas = camera.render(world);

  std::ofstream image{"SuzanneMesh.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);

  return 0;
}
