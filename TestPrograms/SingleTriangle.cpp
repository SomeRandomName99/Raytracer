#include <fstream>

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

// Renders a single triangle to a PPM image. This exercises the new triangle
// data layout, the Moller-Trumbore intersection (recording barycentric u/v),
// the interpolated triangle normal, and the triangle bounding box.
int main() {
  World world;

  // The three corners of the triangle, placed in the z = 0 plane facing the
  // camera (which sits at negative z).
  const auto p0 = utility::Point(0.0f, 1.0f, 0.0f);   // top
  const auto p1 = utility::Point(-1.0f, -1.0f, 0.0f); // bottom left
  const auto p2 = utility::Point(1.0f, -1.0f, 0.0f);  // bottom right

  // Per-vertex normals. Fanning them outwards slightly makes the smooth-normal
  // interpolation visible as a gentle shading gradient across the face.
  const auto n0 = utility::Vector(0.0f, 0.4f, -1.0f);
  const auto n1 = utility::Vector(-0.4f, -0.2f, -1.0f);
  const auto n2 = utility::Vector(0.4f, -0.2f, -1.0f);

  auto triangleMaterial = material::Material(utility::Color(0.9f, 0.2f, 0.2f), // surface color
                                             0.1f,                             // ambient
                                             0.9f,                             // diffuse
                                             0.9f,                             // specular
                                             200.0f,                           // shininess
                                             0.0f,                             // reflectance
                                             0.0f,                             // transparency
                                             1.0f);                            // refractive index
  const size_t materialIndex = addMaterial(world, triangleMaterial);

  const size_t triangleIndex = addTriangle(world, p0, p1, p2, n0, n1, n2);
  world.objects[triangleIndex].MaterialIndex = static_cast<int16_t>(materialIndex);

  addLight(world, scene::PointLight{utility::Color(1.0f, 1.0f, 1.0f), utility::Point(-10.0f, 10.0f, -10.0f)});

  auto camera = scene::Camera(400, 400, 1.0f);
  camera.setTransform(utility::transformations::view_transform(
      utility::Point(0.0f, 0.0f, -5.0f), utility::Point(0.0f, 0.0f, 0.0f), utility::Vector(0.0f, 1.0f, 0.0f)));

  auto canvas = camera.render(world);

  std::ofstream image{"SingleTriangle.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);

  return 0;
}
