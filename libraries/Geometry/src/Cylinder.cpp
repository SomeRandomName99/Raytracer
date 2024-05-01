#include <memory>
#include <utility>
#include <algorithm>

#include "Cylinder.hpp"
#include "Intersections.hpp"
#include "floatUtils.hpp"
#include "AABB.hpp"

namespace raytracer {
namespace geometry {
Cylinder::Cylinder() noexcept {
  this->setBoundingBox(utility::AABB(utility::Point(-1, -std::numeric_limits<float>::infinity(), -1),
                                     utility::Point(1, std::numeric_limits<float>::infinity(), 1)));
}
Cylinder::Cylinder(float minimum, float maximum, bool closed) noexcept : minimum_{minimum}, maximum_{maximum}, closed_{closed} {
  this->setBoundingBox(utility::AABB(utility::Point(-1, minimum, -1), utility::Point(1, maximum, 1)));
}

static inline bool isIntersectionWithinRadius(const utility::Ray &ray, float t) noexcept {
  auto x = ray.origin_.x() + t * ray.direction_.x();
  auto z = ray.origin_.z() + t * ray.direction_.z();
  return (x * x + z * z) <= 1; // radius is always considered to be 1
}

static inline std::vector<Intersection> intersectCaps(const Cylinder &cylinder, const utility::Ray &ray) {
  if (!cylinder.closed_ || utility::floatNearlyEqual(ray.direction_.y(), 0.0f)) { return {}; }

  auto intersections = std::vector<Intersection>{};
  auto tLowerCap = (cylinder.minimum_ - ray.origin_.y()) / ray.direction_.y();
  if (isIntersectionWithinRadius(ray, tLowerCap)) { intersections.emplace_back(Intersection(&cylinder, tLowerCap)); }

  auto tUpperCap = (cylinder.maximum_ - ray.origin_.y()) / ray.direction_.y();
  if (isIntersectionWithinRadius(ray, tUpperCap)) { intersections.emplace_back(Intersection(&cylinder, tUpperCap)); }

  return intersections;
}

std::vector<Intersection> Cylinder::localIntersect(const utility::Ray &transformedRay) const noexcept {
  auto a = transformedRay.direction_.x() * transformedRay.direction_.x() + 
           transformedRay.direction_.z() * transformedRay.direction_.z();
  if (utility::floatNearlyEqual(a, 0.0f)) { return (closed_ ? intersectCaps(*this, transformedRay) : std::vector<Intersection>{}); }

  auto b = 2 * transformedRay.origin_.x() * transformedRay.direction_.x() + 
           2 * transformedRay.origin_.z() * transformedRay.direction_.z();
  auto c = transformedRay.origin_.x() * transformedRay.origin_.x() +
           transformedRay.origin_.z() * transformedRay.origin_.z() 
           - 1;

  auto discriminant = b * b - 4 * a * c;
  
  if (discriminant < 0) { return {}; }

  auto t0 = (-b - utility::sqrt(discriminant)) / (2 * a);
  auto t1 = (-b + utility::sqrt(discriminant)) / (2 * a);

  auto intersections = std::vector<Intersection>{};
  auto isBetweenMinAndMax = [this](float y) { return y > minimum_ && y < maximum_; };

  auto firstIntersectionY = transformedRay.origin_.y() + t0 * transformedRay.direction_.y();
  if (isBetweenMinAndMax(firstIntersectionY)){intersections.emplace_back(Intersection(this, t0));}

  auto secondIntersectionY = transformedRay.origin_.y() + t1 * transformedRay.direction_.y();
  if (isBetweenMinAndMax(secondIntersectionY)){intersections.emplace_back(Intersection(this, t1));}

  auto capIntersections = intersectCaps(*this, transformedRay);
  intersections.insert(intersections.end(), capIntersections.begin(), capIntersections.end());

  return intersections;
}

utility::Tuple Cylinder::localNormalAt(const utility::Tuple &objectPoint) const noexcept {
  auto distanceFromYSquared = objectPoint.x() * objectPoint.x() + objectPoint.z() * objectPoint.z();
  if (distanceFromYSquared < 1 && objectPoint.y() >= maximum_ - utility::EPSILON<float>) { return utility::Vector(0, 1, 0); }
  else if (distanceFromYSquared < 1 && objectPoint.y() <= minimum_ + utility::EPSILON<float>) { return utility::Vector(0, -1, 0); }
  return utility::Vector(objectPoint.x(), 0, objectPoint.z());
}

std::shared_ptr<Cylinder> makeCylinder() noexcept {
  return std::make_shared<Cylinder>();
}

} // namespace geometry
} // namespace raytracer