#include <algorithm>
#include <ranges>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <memory>

#include "Intersections.hpp"
#include "FloatAlmostEquals.hpp"
#include "Sphere.hpp"

namespace raytracer {
namespace geometry {

bool operator==(Intersection const& lhs, Intersection const& rhs) {
  return utility::floatNearlyEqual(lhs.dist_, rhs.dist_) && lhs.object_ == rhs.object_;
}

Computations Intersection::prepareComputations(const utility::Ray& ray) const {
  Computations computations;

  // copy values for ease of accessibility
  computations.dist = dist_;
  computations.object = object_;

  computations.point = ray.position(dist_);
  computations.normalVector = computations.object->normalAt(computations.point);
  computations.eyeVector = -ray.direction_;
  if(computations.normalVector.dot(computations.eyeVector) < 0){
    computations.inside = true;
    computations.normalVector = -computations.normalVector;
  }
  else{
    computations.inside = false;
  }

  computations.overPoint = computations.point + computations.normalVector * SHADOW_OFFSET;
  return computations;
}

/**
 * Finds the smallest positive intersection from a utility::Vector of intersections.
 *
 * @param intersections A utility::Vector of intersections to search through.
 * @return The smallest positive intersection, wrapped in std::optional. If no positive intersection is found, returns std::nullopt.
 */
std::optional<Intersection> hit(const std::vector<Intersection>& intersections) {
  auto positiveT = intersections | std::views::filter([](const auto& intersection) { return intersection.dist_ > 0.0; });
  auto smallestIntersection = std::ranges::min_element(positiveT, {}, [](const auto& intersection){return intersection.dist_;});


  if(smallestIntersection == positiveT.end())
    return std::nullopt;
  return *smallestIntersection;
}

} // namespace geometry
} // namespace raytracer