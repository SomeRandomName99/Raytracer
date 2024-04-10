#include <memory>
#include <utility>

#include "Cube.hpp"
#include "Intersections.hpp"
#include "FloatUtils.hpp"

namespace raytracer {
namespace geometry {


// using the optimized slab intersection method
// https://tavianator.com/2015/ray_box_nan.html
std::vector<Intersection> Cube::localIntersect(const utility::Ray &transformedRay) const noexcept {
  double tx1 = (-1 - transformedRay.origin_.x()) / transformedRay.direction_.x();
  double tx2 = (1 - transformedRay.origin_.x()) / transformedRay.direction_.x();

  double tmin = std::min(tx1, tx2);
  double tmax = std::max(tx1, tx2);

  double ty1 = (-1 - transformedRay.origin_.y()) / transformedRay.direction_.y();
  double ty2 = (1 - transformedRay.origin_.y()) / transformedRay.direction_.y();

  tmin = std::max(tmin, std::min(ty1, ty2));
  tmax = std::min(tmax, std::max(ty1, ty2));

  double tz1 = (-1 - transformedRay.origin_.z()) / transformedRay.direction_.z();
  double tz2 = (1 - transformedRay.origin_.z()) / transformedRay.direction_.z();

  tmin = std::max(tmin, std::min(tz1, tz2));
  tmax = std::min(tmax, std::max(tz1, tz2));

  return tmin > tmax ? std::vector<Intersection>() : std::vector<Intersection>{Intersection(this, tmin), Intersection(this, tmax)};
}

utility::Tuple Cube::localNormalAt(const utility::Tuple &objectPoint) const noexcept {
  auto largestAbsComponent = std::max({std::abs(objectPoint.x()), std::abs(objectPoint.y()), std::abs(objectPoint.z())});

  if (utility::floatNearlyEqual(largestAbsComponent, std::abs(objectPoint.x()))) {
    return utility::Vector(objectPoint.x(), 0, 0);
  } else if (utility::floatNearlyEqual(largestAbsComponent, std::abs(objectPoint.y()))) {
    return utility::Vector(0, objectPoint.y(), 0);
  } else {
    return utility::Vector(0, 0, objectPoint.z());
  }
}

std::shared_ptr<Cube> normalCube() noexcept {
  return std::make_shared<Cube>();
}

} // namespace geometry
} // namespace raytracer