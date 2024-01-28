#include "World.hpp"
#include "Material.hpp"

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
    color += scene::lighting(comps.object->material_, light, comps.point, comps.eyeVector, comps.normalVector);
  }
  return color;
}

utility::Color World::colorAt(const utility::Ray& ray) const{
  const auto intersections          = this->intersect(ray);
  if (intersections.empty()) return utility::Color{0,0,0};
  const auto intersect_computations = geometry::hit(intersections)->prepareComputations(ray);
  return this->shadeHit(intersect_computations);
}

World defaultWorld(){
  World world;

  const auto light = scene::PointLight{utility::Color{1,1,1}, utility::Point(-10,10,-10)};
  world.lights_.push_back(light);

  auto s1 = geometry::Sphere{};
  s1.material_ = material::Material(utility::Color(0.8, 1.0, 0.6), 0.1, 0.7, 0.2);
  auto s2 = geometry::Sphere{};
  s2.setTransform(utility::transformations::scaling(0.5, 0.5, 0.5));
  world.objects_.push_back(std::make_shared<geometry::Sphere>(s1));
  world.objects_.push_back(std::make_shared<geometry::Sphere>(s2));

  return world;
}

} // namespace scene
} // namespace raytracer