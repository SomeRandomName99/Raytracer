#ifndef CUBE_H
#define CUBE_H

#include <memory>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Shape.hpp"

namespace raytracer {
namespace geometry {

class Cube final : public ShapeBase
{
public:
  Cube() noexcept {}
  bool operator==(Cube const& other) const noexcept;

  /**
   * Calculates the normal vector at a given point on the plane.
   */
  utility::Tuple localNormalAt(const utility::Tuple &objectPoint) const noexcept override;

  /**
   * Function to calculate the intersections between a ray and a plane.
   */
  std::vector<Intersection> localIntersect(const utility::Ray &transformedRay) const noexcept override;
};

std::shared_ptr<Cube> makeCube() noexcept;

} // namespace geometry
} // namespace raytracer

#endif // CUBE_H