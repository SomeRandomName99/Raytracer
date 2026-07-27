#ifndef WORLD_HPP
#define WORLD_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "libraries/Geometry/include/Intersections.hpp"
#include "libraries/Geometry/include/Shape.hpp"
#include "libraries/Material/include/Material.hpp"
#include "libraries/Material/include/Pattern.hpp"
#include "libraries/Scene/include/Light.hpp"
#include "libraries/Utility/include/Arena.hpp"

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
  std::vector<CircularSolidData> circularSolidData;
  std::vector<TriangleData> triangleData;
  std::vector<MeshData> meshData;
};

// Here we will have the functions that are going to construct the world
// For example add spheres and shapes
// Assign materials and patterns to them
// Make groups out of multiple shapes
// Add transformatoins to the shapes and groups
void setBoundingBox(const World &world, WorldObject &node) noexcept;
size_t addMaterial(World &world, const Material &material) noexcept;
size_t addPattern(World &world, const Pattern &pattern) noexcept;
size_t addObject(World &world, const WorldObject &object) noexcept;
size_t addTriangle(World &world, const utility::Tuple &v0, const utility::Tuple &v1, const utility::Tuple &v2,
                   const utility::Tuple &n0, const utility::Tuple &n1, const utility::Tuple &n2, float u = 0.0f,
                   float v = 0.0f) noexcept;
size_t addTriangle(World &world, const utility::Tuple &v0, const utility::Tuple &v1, const utility::Tuple &v2,
                   float u = 0.0f, float v = 0.0f) noexcept;
size_t addLight(World &world, const PointLight &light) noexcept;
size_t addObjectWithMaterial(World &world, const WorldObject &object, const Material &material,
                             const std::optional<Pattern> &pattern = std::nullopt) noexcept;
void addTransformToObject(World &world, size_t objectIndex, const utility::Matrix<4, 4> &transform) noexcept;
void setObjectShadow(World &world, const size_t objectIndex, const bool hasShadow) noexcept;
std::optional<size_t> loadMeshFromObjFile(World &world, const std::string &inputFile);
} // namespace raytracer::scene

#endif // WORLD_HPP
