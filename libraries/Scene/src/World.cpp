#include "World.hpp"
#include "Material.hpp"
#include "FloatAlmostEquals.hpp"

namespace raytracer {
namespace scene {

std::vector<geometry::Intersection> World::intersect(const utility::Ray& ray) const noexcept{
  std::vector<geometry::Intersection> intersections;
  for(const auto& object : objects_){
    const auto objectIntersections = object.intersect(ray);
    intersections.insert(intersections.end(), objectIntersections.begin(), objectIntersections.end());
  }
  std::ranges::sort(intersections, {}, [](const auto& intersection){ return intersection.dist; });
  return intersections;
}

bool World::intersectShadow(const utility::Ray& ray, double distanceToLight) const noexcept{
  for(const auto& object : objects_){
    const auto objectIntersections = object.intersect(ray);
    const auto hit = geometry::hit(objectIntersections);
    if(hit && hit->dist > 0.0 && hit->dist < distanceToLight){return true;}
  }

  return false;
}


utility::Color World::shadeHit(const geometry::Computations& comps) const noexcept{
  auto color = utility::Color{0,0,0};
  for(const auto& light : this->lights_) {
    color += scene::lighting(*comps.intersection.material, light, comps.point, comps.eyeVector, 
                             comps.intersection.normalVector, this->isShadowed(light, comps.overPoint));
  }
  return color;
}

utility::Color World::colorAt(const utility::Ray& ray) const noexcept{
  const auto hit                    = geometry::hit(this->intersect(ray));
  if (!hit.has_value()) return utility::Color{0,0,0};
  const auto intersect_computations = geometry::prepareComputations(*hit, ray);
  return this->shadeHit(intersect_computations);
}

bool World::isShadowed(const PointLight& light, const utility::Tuple& point) const noexcept{
  const auto pointToLightVector = light.position_ - point;
  const auto pointToLightDistance = pointToLightVector.magnitude();
  const auto pointToLightDirection = pointToLightVector.normalize();
  const auto pointToLightRay = utility::Ray(point, pointToLightDirection);

  return this->intersectShadow(pointToLightRay, pointToLightDistance);
}

World defaultWorld() noexcept{
  World world;

  const auto light = scene::PointLight{utility::Color{1,1,1}, utility::Point(-10,10,-10)};
  world.lights_.push_back(light);

  auto s1 = geometry::Sphere{};
  s1.material_ = material::Material(utility::Color(0.8, 1.0, 0.6), 0.1, 0.7, 0.2);
  auto s2 = geometry::Sphere{};
  s2.setTransform(utility::transformations::scaling(0.5, 0.5, 0.5));
  world.objects_.emplace_back(s1);
  world.objects_.emplace_back(s2);

  return world;
}

} // namespace scene
} // namespace raytracer