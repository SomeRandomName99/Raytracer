#include <algorithm>
#include <ranges>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <memory>

#include "Intersections.hpp"
#include "FloatUtils.hpp"
#include "Shape.hpp"
#include "Transformations.hpp"

namespace raytracer {
namespace geometry {

bool operator==(Intersection const& lhs, Intersection const& rhs) noexcept{
  return utility::floatNearlyEqual(lhs.dist, rhs.dist) && lhs.object == rhs.object;
}

Computations prepareComputations(Intersection intersection, const utility::Ray& ray, 
                                 const std::vector<Intersection>& intersections) noexcept{
  Computations computations;
  std::vector<const geometry::ShapeBase*> unexitedShapes;

  for(const auto& i: intersections){
    if(i == intersection){
      computations.n1 = unexitedShapes.empty() ? 1.0 : unexitedShapes.back()->material().refractiveIndex();
    }

    auto found = std::find(unexitedShapes.begin(), unexitedShapes.end(), i.object);
    if(found != unexitedShapes.end()){
      unexitedShapes.erase(found);
    } else {
      unexitedShapes.push_back(i.object);
    }

    if(i == intersection){
      computations.n2 = unexitedShapes.empty() ? 1.0 : unexitedShapes.back()->material().refractiveIndex();
      break;
    }
  }

  // copy values for ease of accessibility
  computations.intersection = intersection;
  computations.point = ray.position(intersection.dist);
  computations.normalVector = intersection.object->normalAt(computations.point);
  computations.reflectVector = ray.direction_.reflect(computations.normalVector);
  computations.eyeVector = -ray.direction_;
  
  if(computations.normalVector.dot(computations.eyeVector) < 0){
    computations.inside = true;
    computations.normalVector = -computations.normalVector;
  }
  else{
    computations.inside = false;
  }

  computations.overPoint  = computations.point + computations.normalVector * SHADOW_OFFSET;
  computations.underPoint = computations.point - computations.normalVector * SHADOW_OFFSET;
  return computations;
}

/**
 * Finds the smallest positive intersection from a utility::Vector of intersections.
 *
 * @param intersections A utility::Vector of intersections to search through.
 * @return The smallest positive intersection, wrapped in std::optional. If no positive intersection is found, returns std::nullopt.
 */
std::optional<Intersection> hit(const std::vector<Intersection>& intersections) noexcept{
  auto positiveT = intersections | std::views::filter([](const auto& intersection) { return intersection.dist > 0.0; });
  auto smallestIntersection = std::ranges::min_element(positiveT, {}, [](const auto& intersection){return intersection.dist;});

  if(smallestIntersection == positiveT.end())
    return std::nullopt;
  return *smallestIntersection;
}

double schlick(const Computations& comps) noexcept{
  auto cos = comps.eyeVector.dot(comps.normalVector);
  if(comps.n1 > comps.n2){
    auto n = comps.n1 / comps.n2;
    auto sin2_t = n * n * (1 - cos * cos);
    if(sin2_t > 1) // total internal reflection
      return 1.0;

    auto cos_t = std::sqrt(1.0 - sin2_t);
    cos = cos_t;
  }

  auto r0 = std::pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2);
  return r0 + (1 - r0) * std::pow(1 - cos, 5);
}

} // namespace geometry
} // namespace raytracer