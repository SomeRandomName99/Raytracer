#include <memory>

#include "Plane.hpp"
#include "Shape.hpp"
#include "Intersections.hpp"

namespace raytracer {
namespace geometry {

std::vector<Intersection> Plane::localIntersect(const utility::Ray &transformedRay) const noexcept {
  if (std::abs(transformedRay.direction_.y()) < utility::EPSILON<double>) {
    return std::vector<Intersection>();
  }

  double t = -transformedRay.origin_.y() / transformedRay.direction_.y();
  return std::vector<Intersection>{Intersection(std::make_shared<Shape>(*this), t)};
}

utility::Tuple Plane::localNormalAt([[maybe_unused]]const utility::Tuple &objectPoint) const noexcept {
  return utility::Vector(0, 1, 0);
}

} // namespace geometry
} // namespace raytracer