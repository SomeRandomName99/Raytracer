#include <memory>
#include <limits>

#include "Cone.hpp"
#include "Intersections.hpp"
#include "FloatUtils.hpp"
#include "AABB.hpp"
#include "Arena.hpp"
namespace raytracer {
namespace geometry {

using utility::AABB;

Cone::Cone() noexcept {
 this->setBoundingBox(AABB(utility::Point(-std::numeric_limits<double>::infinity(),
                                  -std::numeric_limits<double>::infinity(),
                                  -std::numeric_limits<double>::infinity()),
                           utility::Point(std::numeric_limits<double>::infinity(),
                                  std::numeric_limits<double>::infinity(),
                                  std::numeric_limits<double>::infinity())));
}

Cone::Cone(double minimum, double maximum, bool closed) noexcept : minimum_{minimum}, maximum_{maximum}, closed_{closed} {
  const auto limit = std::max(std::abs(minimum), std::abs(maximum));
  this->setBoundingBox(AABB(utility::Point(-limit, minimum, -limit), utility::Point(limit, maximum, limit)));
}


static inline bool isIntersectionWithinRadius(const utility::Ray &ray, double radius, double t) noexcept {
  auto x = ray.origin_.x() + t * ray.direction_.x();
  auto z = ray.origin_.z() + t * ray.direction_.z();
  return (x * x + z * z) <= radius; // The radius of a cone will be the y coordinate at the intersection plane
}

static inline void intersectCaps(const Cone &cone, const utility::Ray &ray, utility::Arena<Intersection>& intersections) noexcept {
  if (!cone.closed_ || utility::floatNearlyEqual(ray.direction_.y(), 0.0f)) { return; }

  auto tLowerCap = (cone.minimum_ - ray.origin_.y()) / ray.direction_.y();
  if (isIntersectionWithinRadius(ray, std::abs(cone.minimum_), tLowerCap)) { intersections.pushBack(Intersection{&cone, tLowerCap}); }

  auto tUpperCap = (cone.maximum_ - ray.origin_.y()) / ray.direction_.y();
  if (isIntersectionWithinRadius(ray, std::abs(cone.maximum_), tUpperCap)) { intersections.pushBack(Intersection{&cone, tUpperCap}); }

}

void Cone::localIntersect(const utility::Ray &transformedRay, utility::Arena<Intersection>& intersections) const noexcept {
  auto a = transformedRay.direction_.x() * transformedRay.direction_.x() - 
                   transformedRay.direction_.y() * transformedRay.direction_.y() + 
                   transformedRay.direction_.z() * transformedRay.direction_.z();
  auto b = 2 * transformedRay.origin_.x() * transformedRay.direction_.x() - 
                   2 * transformedRay.origin_.y() * transformedRay.direction_.y() + 
                   2 * transformedRay.origin_.z() * transformedRay.direction_.z();
  auto c = transformedRay.origin_.x() * transformedRay.origin_.x() - 
                   transformedRay.origin_.y() * transformedRay.origin_.y() + 
                   transformedRay.origin_.z() * transformedRay.origin_.z();

  // if a is nearly equal to 0, then the ray is parallel to one of the cone's halves
  if (utility::floatNearlyEqual(a, 0.0f)) {
    if (closed_){
      intersectCaps(*this, transformedRay, intersections);
    }
    if (utility::floatNearlyEqual(b, 0.0f)) { return; }
    intersections.pushBack(Intersection{this, -c / (2 * b)});
    return;
  }

  auto discriminant = b * b - 4 * a * c;
  if (!utility::floatNearlyEqual(discriminant, 0.0f) && discriminant < 0) { return; }

  auto t0 = (-b - utility::sqrt(discriminant)) / (2 * a);
  auto t1 = (-b + utility::sqrt(discriminant)) / (2 * a);

  auto isBetweenMinAndMax = [this](double y) { return y > minimum_ && y < maximum_; };
  auto firstIntersectionY = transformedRay.origin_.y() + t0 * transformedRay.direction_.y();
  if (isBetweenMinAndMax(firstIntersectionY)) { intersections.pushBack(Intersection{this, t0}); }

  auto secondIntersectionY = transformedRay.origin_.y() + t1 * transformedRay.direction_.y();
  if (isBetweenMinAndMax(secondIntersectionY)) { intersections.pushBack(Intersection{this, t1}); }

  intersectCaps(*this, transformedRay, intersections);
}

utility::Tuple Cone::localNormalAt(const utility::Tuple &objectPoint) const noexcept{
  auto distanceFromYSquared = objectPoint.x() * objectPoint.x() + objectPoint.z() * objectPoint.z();
  if (distanceFromYSquared < 1 && objectPoint.y() >= maximum_ - utility::EPSILON<double>) { return utility::Vector(0, 1, 0); }
  else if (distanceFromYSquared < 1 && objectPoint.y() <= minimum_ + utility::EPSILON<double>) { return utility::Vector(0, -1, 0); }

  auto normalYCoord = objectPoint.y() > 0 ? -utility::sqrt(distanceFromYSquared) : utility::sqrt(distanceFromYSquared);
  return utility::Vector(objectPoint.x(), normalYCoord, objectPoint.z());
}

std::shared_ptr<Cone> makeCone() noexcept {
  return std::make_shared<Cone>();  
}

} // namespace geometry
} // namespace raytracer