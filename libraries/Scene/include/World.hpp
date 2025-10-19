#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <memory>
#include <cstdint>

#include "Sphere.hpp"
#include "Plane.hpp"
#include "Intersections.hpp"
#include "Shape.hpp"
#include "Light.hpp"
#include "Arena.hpp"
#include "Material.hpp"
#include "Pattern.hpp"

namespace raytracer::scene {

using namespace material;
using namespace geometry;

constexpr size_t MAX_INTERSECTIONS = 5;
struct World {
public:
  std::vector<PointLight> lights;
  std::vector<Material> materials;
  std::vector<Pattern> patterns;
  std::vector<WorldObject> objects;
  std::vector<GroupData> groupData;
  std::vector<circularSolidData> circularSolidData;
};

// Here we will have the functions that are going to construct the world
// For example add spheres and shapes
// Assign materials and patterns to them
// Make groups out of multiple shapes
// Add transformatoins to the shapes and groups
void setBoundingBox(const World& world, WorldObject& node) noexcept;

} // namespace raytracer::scene

#endif // WORLD_HPP