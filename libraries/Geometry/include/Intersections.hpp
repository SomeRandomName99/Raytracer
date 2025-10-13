#if !defined(INTERSECTIONS_H)
#define INTERSECTIONS_H

#include <vector>
#include <concepts>
#include <optional>
#include <memory>
#include <span>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Material.hpp"
#include "Matrix.hpp"
#include "Arena.hpp"

constexpr double SHADOW_OFFSET = 1e-4;

namespace raytracer {
namespace geometry {
class ShapeBase;

/**
 * \class Intersection
 * \brief Save information about an intersection between a ray and a geometry object.
 */
struct Intersection
{
  const ShapeBase* object;
  double dist; ///< Distance from the ray origin to the intersection point.
};

bool operator==(Intersection const& lhs, Intersection const& rhs) noexcept;


} // namespace geometry
} // namespace raytracer

#endif // INTERSECTIONS_H