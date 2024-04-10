#include <memory>

#include "Sphere.hpp"
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

  const auto dist1 = (-b - utility::sqrt(discriminant)) / (2*a);
  const auto dist2 = (-b + utility::sqrt(discriminant)) / (2*a);
  return std::vector<Intersection>{Intersection{this, dist1},
                                   Intersection{this, dist2}};
}

utility::Tuple Sphere::localNormalAt(const utility::Tuple &objectPoint) const noexcept{
  utility::Tuple objectNormal = objectPoint - utility::Point(0, 0, 0);
  return objectNormal;
}

std::shared_ptr<Sphere> normalSphere() noexcept{
  return std::make_shared<Sphere>();
}

std::shared_ptr<Sphere> glassSphere() noexcept{
  auto glassSphere = normalSphere();
  glassSphere->material().setTransparency(1.0);
  glassSphere->material().setRefractiveIndex(1.5); 

  return glassSphere;
}

} // namespace geometry
} // namespace raytracer