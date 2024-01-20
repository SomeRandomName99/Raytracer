#include <algorithm>
#include <ranges>
#include <cmath>
#include <iostream>

#include "Intersections.hpp"

bool Intersection::operator==(const Intersection& other) const{
  return this->t      == other.t &&
         this->object == other.object;
}

/**
 * Calculates the intersections between a sphere and a ray.
 *
 * @param sphere The sphere to intersect with.
 * @param ray The ray to intersect with the sphere.
 * @return A vector of Intersection objects representing the points of intersection.
 *         If there are no intersections, an empty vector is returned.
 */
std::vector<Intersection> intersect(const Sphere& sphere, const Ray::Ray& ray){
  // For now we assume that the sphere is always at the origin
  const auto transformedRay    = Ray::transform(ray, inverse(sphere.transformation));
  const auto SphereCenterToRay = transformedRay.origin - Point(0, 0, 0);

  const auto a = transformedRay.direction.dot(transformedRay.direction);
  const auto b = 2 * transformedRay.direction.dot(SphereCenterToRay);
  const auto c = SphereCenterToRay.dot(SphereCenterToRay) - 1; // where 1 is the radius of the sphere

  const auto discriminant = b*b - 4*a*c;

  if (discriminant < 0.)
    return std::vector<Intersection>{};

  const auto t1 = (-b - std::sqrt(discriminant)) / (2*a);
  const auto t2 = (-b + std::sqrt(discriminant)) / (2*a);
  return std::vector<Intersection>{ Intersection{t1, sphere}, Intersection{t2, sphere} };
}

/**
 * Finds the smallest positive intersection from a vector of intersections.
 *
 * @param intersections A vector of intersections to search through.
 * @return The smallest positive intersection, wrapped in std::optional. If no positive intersection is found, returns std::nullopt.
 */
std::optional<Intersection> hit(const std::vector<Intersection>& intersections){
  auto positiveT = intersections | std::views::filter([](const auto& intersection) { return intersection.t > 0.0; });
  auto smallestIntersection = std::ranges::min_element(positiveT, {}, [](const auto& intersection){return intersection.t;});


  if(smallestIntersection == positiveT.end())
    return std::nullopt;
  return *smallestIntersection;
}