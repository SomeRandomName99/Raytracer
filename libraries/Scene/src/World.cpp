#include "World.hpp"

namespace raytracer {
namespace scene {

std::vector<geometry::Intersection> World::intersect(const utility::Ray& ray){
  std::vector<geometry::Intersection> intersections;
  for(const auto& object : objects_){
    const auto objectIntersections = object.intersect(ray);
    intersections.insert(intersections.end(), objectIntersections.begin(), objectIntersections.end());
  }
  std::ranges::sort(intersections, {}, [](const auto& intersection){return intersection.t_;});
  return intersections;
}

} // namespace scene
} // namespace raytracer