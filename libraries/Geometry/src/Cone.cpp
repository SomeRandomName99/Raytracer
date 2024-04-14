#include <memory>

#include "Cone.hpp"
#include "Intersections.hpp"
#include "FloatUtils.hpp"

namespace raytracer {
namespace geometry {

static inline bool isIntersectionWithinRadius(const utility::Ray &ray, double radius, double t) noexcept {
  auto x = ray.origin_.x() + t * ray.direction_.x();
  auto z = ray.origin_.z() + t * ray.direction_.z();
  return (x * x + z * z) <= radius; // The radius of a cone will be the y coordinate at the intersectio plane
}

static inline std::vector<Intersection> intersectCaps(const Cone &cone, const utility::Ray &ray) {
  if (!cone.closed_ || utility::floatNearlyEqual(ray.direction_.y(), 0.0)) { return {}; }

  auto intersections = std::vector<Intersection>{};
  auto tLowerCap = (cone.minimum_ - ray.origin_.y()) / ray.direction_.y();
  if (isIntersectionWithinRadius(ray, std::abs(cone.minimum_), tLowerCap)) { intersections.emplace_back(Intersection(&cone, tLowerCap)); }

  auto tUpperCap = (cone.maximum_ - ray.origin_.y()) / ray.direction_.y();
  if (isIntersectionWithinRadius(ray, std::abs(cone.minimum_), tUpperCap)) { intersections.emplace_back(Intersection(&cone, tUpperCap)); }

  return intersections;
}

std::vector<Intersection> Cone::localIntersect(const utility::Ray &transformedRay) const noexcept {
  auto a = transformedRay.direction_.x() * transformedRay.direction_.x() - 
           transformedRay.direction_.y() * transformedRay.direction_.y() + 
           transformedRay.direction_.z() * transformedRay.direction_.z();
  auto b = 2 * transformedRay.origin_.x() * transformedRay.direction_.x() - 
           2 * transformedRay.origin_.y() * transformedRay.direction_.y() + 
           2 * transformedRay.origin_.z() * transformedRay.direction_.z();
  auto c = transformedRay.origin_.x() * transformedRay.origin_.x() - 
           transformedRay.origin_.y() * transformedRay.origin_.y() + 
           transformedRay.origin_.z() * transformedRay.origin_.z();

  auto intersections = std::vector<Intersection>{};

  // if a is nearly equal to 0, then the ray is parallel to one of the cone's halves
  if (utility::floatNearlyEqual(a, 0.0)) {
    if (utility::floatNearlyEqual(b, 0.0)) { return {}; }
    if (closed_){
      auto CapIntersections = intersectCaps(*this, transformedRay);
      intersections.insert(intersections.end(), CapIntersections.begin(), CapIntersections.end());
    }

    intersections.emplace_back(Intersection(this, -c / (2 * b)));
    return intersections;
  }

  auto discriminant = b * b - 4 * a * c;
  if (!utility::floatNearlyEqual(discriminant, 0.0) && discriminant < 0) { return {}; }

  auto t0 = (-b - utility::sqrt(discriminant)) / (2 * a);
  auto t1 = (-b + utility::sqrt(discriminant)) / (2 * a);

  auto isBetweenMinAndMax = [this](double y) { return y > minimum_ && y < maximum_; };

  auto firstIntersectionY = transformedRay.origin_.y() + t0 * transformedRay.direction_.y();
  if (isBetweenMinAndMax(firstIntersectionY)){intersections.emplace_back(Intersection(this, t0));}

  auto secondIntersectionY = transformedRay.origin_.y() + t1 * transformedRay.direction_.y();
  if (isBetweenMinAndMax(secondIntersectionY)){intersections.emplace_back(Intersection(this, t1));}

  auto capIntersections = intersectCaps(*this, transformedRay);
  intersections.insert(intersections.end(), capIntersections.begin(), capIntersections.end());

  return intersections;
}

utility::Tuple Cone::localNormalAt(const utility::Tuple &objectPoint) const noexcept{
  auto distanceFromYSquared = objectPoint.x() * objectPoint.x() + objectPoint.z() * objectPoint.z();
  if (distanceFromYSquared < 1 && objectPoint.y() >= maximum_ - utility::EPSILON<double>) { return utility::Vector(0, 1, 0); }
  else if (distanceFromYSquared < 1 && objectPoint.y() <= minimum_ + utility::EPSILON<double>) { return utility::Vector(0, -1, 0); }

  auto normalYCoord = objectPoint.y() > 0 ? -utility::sqrt(distanceFromYSquared) : utility::sqrt(distanceFromYSquared);
  return utility::Vector(objectPoint.x(), normalYCoord, objectPoint.z());
}

std::shared_ptr<Cone> normalCone() noexcept {
  return std::make_shared<Cone>();  
}

} // namespace geometry
} // namespace raytracer