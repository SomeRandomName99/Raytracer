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