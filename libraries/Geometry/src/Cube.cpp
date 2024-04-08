#include <memory>
#include <utility>

#include "Cube.hpp"
#include "Intersections.hpp"
#include "FloatAlmostEquals.hpp"

namespace raytracer {
namespace geometry {

static inline std::pair<double, double> checkAxis(double origin, double direction) {
  double tmin_numerator = (-1 - origin);
  double tmax_numerator = (1 - origin);

  if (utility::floatNearlyEqual(direction, 0.0)) {
    return {tmin_numerator * std::numeric_limits<double>::infinity(), 
            tmax_numerator * std::numeric_limits<double>::infinity()};
  }

  double tmin = tmin_numerator / direction;
  double tmax = tmax_numerator / direction;
  if(tmin > tmax) { std::swap(tmin, tmax); }

  return{tmin, tmax};
}

std::vector<Intersection> Cube::localIntersect(const utility::Ray &transformedRay) const noexcept {
  auto [xtmin, xtmax] = checkAxis(transformedRay.origin_.x(), transformedRay.direction_.x());
  auto [ytmin, ytmax] = checkAxis(transformedRay.origin_.y(), transformedRay.direction_.y());
  auto [ztmin, ztmax] = checkAxis(transformedRay.origin_.z(), transformedRay.direction_.z());

  auto tmin = std::max({xtmin, ytmin, ztmin});
  auto tmax = std::min({xtmax, ytmax, ztmax});

  if (tmin > tmax) {
    return std::vector<Intersection>();
  }

  return std::vector<Intersection>{Intersection(this, tmin), Intersection(this, tmax)};
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