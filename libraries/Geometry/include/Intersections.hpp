#if !defined(INTERSECTIONS_H)
#define INTERSECTIONS_H

#include <vector>
#include <concepts>
#include <optional>
#include <memory>

#include "libraries/Utility/include/Tuple.hpp"
#include "libraries/Utility/include/Ray.hpp"
#include "libraries/Material/include/Material.hpp"
#include "libraries/Utility/include/Matrix.hpp"
#include "libraries/Utility/include/Arena.hpp"

constexpr double SHADOW_OFFSET = 1e-4;

namespace raytracer {
namespace geometry {
struct WorldObject;

/**
 * \class Intersection
 * \brief Save information about an intersection between a ray and a geometry object.
 */
struct Intersection
{
  const WorldObject* object;
  double dist; ///< Distance from the ray origin to the intersection point.
  float u = 0.0f; ///< Barycentric coordinate of the hit (used for smooth triangles).
  float v = 0.0f; ///< Barycentric coordinate of the hit (used for smooth triangles).
  int32_t triangleIndex = -1; ///< Index of the hit triangle in the world's triangle data (used for meshes).
};

bool operator==(Intersection const& lhs, Intersection const& rhs) noexcept;


} // namespace geometry
} // namespace raytracer

#endif // INTERSECTIONS_H