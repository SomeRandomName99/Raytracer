#include "Sphere.hpp"
#include "FloatAlmostEquals.hpp"
#include "Intersections.hpp"

namespace raytracer {
namespace geometry {

bool Sphere::operator==(const Sphere& other) const noexcept{
  return this->id() == other.id();
}

std::vector<Intersection> Sphere::localIntersect(const utility::Ray& transformedRay) const noexcept{
  // For now we assume that the sphere is always at the origin
  const auto SphereCenterToRay = transformedRay.origin_ - utility::Point(0, 0, 0);

  const auto a = transformedRay.direction_.dot(transformedRay.direction_);
  const auto b = 2 * transformedRay.direction_.dot(SphereCenterToRay);
  const auto c = SphereCenterToRay.dot(SphereCenterToRay) - 1; // where 1 is the radius_ of the sphere

  const auto discriminant = b*b - 4*a*c;

  if (discriminant < 0.)
    return std::vector<Intersection>{};

  const auto dist1 = (-b - std::sqrt(discriminant)) / (2*a);
  const auto dist2 = (-b + std::sqrt(discriminant)) / (2*a);
  return std::vector<Intersection>{Intersection{localNormalAt(transformedRay.position(dist1)), &this->material_, dist1}, 
                                   Intersection{localNormalAt(transformedRay.position(dist2)), &this->material_, dist2}};
}

utility::Tuple Sphere::localNormalAt(const utility::Tuple &objectPoint) const noexcept{
  utility::Tuple objectNormal = objectPoint - utility::Point(0, 0, 0);
  // This is needed because the normal utility::Vector is a linear form, see below:
  // https://computergraphics.stackexchange.com/questions/1502/why-is-the-transposed-inverse-of-the-model-view-matrix-used-to-transform-the-nor
  utility::Tuple worldNormal  = inverseTransform().transpose() * objectNormal;
  worldNormal.w() = 0;

  return worldNormal.normalize();
}

} // namespace geometry
} // namespace raytracer