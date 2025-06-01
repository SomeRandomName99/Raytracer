#include <memory>

#include "Plane.hpp"
#include "Intersections.hpp"
#include "AABB.hpp"

namespace raytracer {
namespace geometry {

Plane::Plane() noexcept {
  this->setBoundingBox(utility::AABB(utility::Point(-std::numeric_limits<double>::infinity(), 0, -std::numeric_limits<double>::infinity()),
                                     utility::Point(std::numeric_limits<double>::infinity(), 0, std::numeric_limits<double>::infinity())));
}

std::vector<Intersection> Plane::localIntersect(const utility::Ray &transformedRay) const noexcept {
  if (std::abs(transformedRay.direction_.y()) < utility::EPSILON<double>) {
    return std::vector<Intersection>();
  }

  double t = -transformedRay.origin_.y() / transformedRay.direction_.y();
  return std::vector<Intersection>{Intersection(this, t)};
}

utility::Tuple Plane::localNormalAt([[maybe_unused]]const utility::Tuple &objectPoint) const noexcept {
  return utility::Vector(0, 1, 0);
}

std::shared_ptr<Plane> makePlane() noexcept {
  return std::make_shared<Plane>();
}

} // namespace geometry
} // namespace raytracer