#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include <memory>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Shape.hpp"
#include "Arena.hpp"
namespace raytracer {
namespace geometry {

class Cylinder final : public ShapeBase
{
public:
  Cylinder() noexcept;
  Cylinder(double minimum, double maximum, bool closed = false) noexcept;

  bool operator==(Cylinder const& other) const noexcept;

  /**
   * Calculates the normal vector at a given point on the plane.
   */
  utility::Tuple localNormalAt(const utility::Tuple &objectPoint) const noexcept override;

  /**
   * Function to calculate the intersections between a ray and a plane.
   */
  void localIntersect(const utility::Ray &transformedRay, utility::Arena<Intersection>& intersections) const noexcept override;

  double minimum_{-std::numeric_limits<double>::infinity()};
  double maximum_{std::numeric_limits<double>::infinity()};
  bool closed_{false};
};

std::shared_ptr<Cylinder> makeCylinder() noexcept;
std::shared_ptr<Cylinder> makeCylinder(double minimum, double maximum, bool closed) noexcept;

} // namespace geometry
} // namespace raytracer

#endif // CYLINDER_HPP