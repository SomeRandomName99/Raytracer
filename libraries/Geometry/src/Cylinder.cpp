#include <memory>
#include <utility>

#include "Cylinder.hpp"
#include "Intersections.hpp"
#include "FloatAlmostEquals.hpp"

namespace raytracer {
namespace geometry {

std::vector<Intersection> Cylinder::localIntersect(const utility::Ray &transformedRay) const noexcept {
  auto a = transformedRay.direction_.x() * transformedRay.direction_.x() + 
           transformedRay.direction_.z() * transformedRay.direction_.z();
  if (utility::floatNearlyEqual(a, 0.0)) { return {}; }

  auto b = 2 * transformedRay.origin_.x() * transformedRay.direction_.x() + 
           2 * transformedRay.origin_.z() * transformedRay.direction_.z();
  auto c = transformedRay.origin_.x() * transformedRay.origin_.x() +
           transformedRay.origin_.z() * transformedRay.origin_.z() 
           - 1;

  auto discriminant = b * b - 4 * a * c;
  
  if (discriminant < 0) { return {}; }

  auto t0 = (-b - std::sqrt(discriminant)) / (2 * a);
  auto t1 = (-b + std::sqrt(discriminant)) / (2 * a);

  return {Intersection(this, t0), Intersection(this, t1)};
}

utility::Tuple Cylinder::localNormalAt(const utility::Tuple &objectPoint) const noexcept {
  return utility::Vector(objectPoint.x(), 0, objectPoint.z());
}

std::shared_ptr<Cylinder> normalCylinder() noexcept {
  return std::make_shared<Cylinder>();
}

} // namespace geometry
} // namespace raytracer