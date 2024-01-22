#include <algorithm>
#include <ranges>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <memory>

#include "Intersections.hpp"
#include "FloatAlmostEquals.hpp"

namespace raytracer {
namespace geometry {

bool operator==(Intersection const& lhs, Intersection const& rhs) {
  return utility::floatNearlyEqual(lhs.t_, rhs.t_) && lhs.object_ == rhs.object_;
}

/**
 * Calculates the intersections between a sphere and a ray.
 *
 * @param sphere The sphere to intersect with.
 * @param ray The ray to intersect with the sphere.
 * @return A utility::Vector of Intersection objects representing the points of intersection.
 *         If there are no intersections, an empty utility::Vector is returned.
 */
std::vector<Intersection> intersect(const Sphere& sphere, const utility::Ray& ray){
  // For now we assume that the sphere is always at the origin
  const auto transformedRay    = utility::transform(ray, inverse(sphere.transformation));
  const auto SphereCenterToRay = transformedRay.origin_ - utility::Point(0, 0, 0);

  const auto a = transformedRay.direction_.dot(transformedRay.direction_);
  const auto b = 2 * transformedRay.direction_.dot(SphereCenterToRay);
  const auto c = SphereCenterToRay.dot(SphereCenterToRay) - 1; // where 1 is the radius of the sphere

  const auto discriminant = b*b - 4*a*c;

  if (discriminant < 0.)
    return std::vector<Intersection>{};

  const auto t1 = (-b - std::sqrt(discriminant)) / (2*a);
  const auto t2 = (-b + std::sqrt(discriminant)) / (2*a);
  return std::vector<Intersection>{Intersection{t1, std::make_shared<Sphere>(sphere)}, 
                                   Intersection{t2, std::make_shared<Sphere>(sphere)}};
}


std::vector<Intersection> intersect_world(const scene::World& world, const utility::Ray& ray){
  std::vector<Intersection> intersections;
  for(const auto& object : world.objects_){
    const auto objectIntersections = intersect(object, ray);
    intersections.insert(intersections.end(), objectIntersections.begin(), objectIntersections.end());
  }
  std::ranges::sort(intersections, {}, [](const auto& intersection){return intersection.t_;});
  return intersections;
}

/**
 * Finds the smallest positive intersection from a utility::Vector of intersections.
 *
 * @param intersections A utility::Vector of intersections to search through.
 * @return The smallest positive intersection, wrapped in std::optional. If no positive intersection is found, returns std::nullopt.
 */
std::optional<Intersection> hit(const std::vector<Intersection>& intersections){
  auto positiveT = intersections | std::views::filter([](const auto& intersection) { return intersection.t_ > 0.0; });
  auto smallestIntersection = std::ranges::min_element(positiveT, {}, [](const auto& intersection){return intersection.t_;});


  if(smallestIntersection == positiveT.end())
    return std::nullopt;
  return *smallestIntersection;
}

} // namespace geometry
} // namespace raytracer