#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include <memory>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Shape.hpp"

namespace raytracer {
namespace geometry {

class Cylinder final : public ShapeBase
{
public:
  Cylinder() noexcept {}
  Cylinder(double minimum, double maximum, bool closed = false) noexcept : minimum_{minimum}, maximum_{maximum},
                                                                   closed_{closed} {}
  bool operator==(Cylinder const& other) const noexcept;

  /**
   * Calculates the normal vector at a given point on the plane.
   */
  utility::Tuple localNormalAt(const utility::Tuple &objectPoint) const noexcept override;

  /**
   * Function to calculate the intersections between a ray and a plane.
   */
  std::vector<Intersection> localIntersect(const utility::Ray &transformedRay) const noexcept override;

  double minimum_{-std::numeric_limits<double>::infinity()};
  double maximum_{std::numeric_limits<double>::infinity()};
  bool closed_{false};
};

std::shared_ptr<Cylinder> makeCylinder() noexcept;

} // namespace geometry
} // namespace raytracer

#endif // CYLINDER_HPP