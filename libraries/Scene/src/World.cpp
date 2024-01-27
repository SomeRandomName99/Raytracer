#include "World.hpp"

namespace raytracer {
namespace scene {

std::vector<geometry::Intersection> World::intersect(const utility::Ray& ray) const{
  std::vector<geometry::Intersection> intersections;
  for(const auto& object : objects_){
    const auto objectIntersections = object->intersect(ray);
    intersections.insert(intersections.end(), objectIntersections.begin(), objectIntersections.end());
  }
  std::ranges::sort(intersections, {}, [](const auto& intersection){ return intersection.dist_; });
  return intersections;
}

utility::Color World::shadeHit(const geometry::Computations& comps) const{
  auto color = utility::Color{0,0,0};
  for(const auto& light : this->lights_) {
    color += scene::lighting(comps.object->material, light, comps.point, comps.eyeVector, comps.normalVector);
  }
  return color;
}

utility::Color World::colorAt(const utility::Ray& ray) const{
  const auto intersections          = this->intersect(ray);
  if (intersections.empty()) return utility::Color{0,0,0};
  const auto intersect_computations = geometry::hit(intersections)->prepareComputations(ray);
  return this->shadeHit(intersect_computations);
}

} // namespace scene
} // namespace raytracer