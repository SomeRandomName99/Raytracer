#include <cmath>

#include "include/sphere.h"
#include "include/intersections.h"
#include "include/maths.h"

std::vector<Intersection> Sphere::intersect(const Ray::Ray& ray) const{
  // For now we assume that the sphere is always at the origin
  const auto SphereCenterToRay = ray.origin - Point(0, 0, 0);

  const auto a = ray.direction.dot(ray.direction);
  const auto b = 2 * ray.direction.dot(SphereCenterToRay);
  const auto c = SphereCenterToRay.dot(SphereCenterToRay) - 1; // where 1 is the radius of the sphere

  const auto discriminant = b*b - 4*a*c;

  if (discriminant < 0.)
    return std::vector<Intersection>{};

  const auto t1 = (-b - std::sqrt(discriminant)) / (2*a);
  const auto t2 = (-b + std::sqrt(discriminant)) / (2*a);
  return std::vector<Intersection>{ Intersection{t1, *this}, Intersection{t2, *this} };
}

bool Sphere::operator==(const Sphere& other) const{
  return this == &other;
}