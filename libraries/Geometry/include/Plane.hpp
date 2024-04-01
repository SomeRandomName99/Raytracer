#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include <atomic>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Shape.hpp"

namespace raytracer {
namespace geometry {

class Plane final : public ShapeBase
{
public:
  Plane() noexcept {}
  bool operator==(Plane const& other) const noexcept;

  /**
   * Calculates the normal vector at a given point on the plane.
   */
  utility::Tuple localNormalAt(const utility::Tuple &objectPoint) const noexcept override;

  /**
   * Function to calculate the intersections between a ray and a plane.
   */
  std::vector<Intersection> localIntersect(const utility::Ray &transformedRay) const noexcept override;
};

std::shared_ptr<Plane> normalPlane() noexcept;

} // namespace geometry
} // namespace raytracer

#endif // PLANE_H