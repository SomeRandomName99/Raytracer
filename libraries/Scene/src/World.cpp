#include <optional>
#include <algorithm>

#include "World.hpp"
#include "Material.hpp"
#include "FloatUtils.hpp"

namespace raytracer {
namespace scene {

std::vector<geometry::Intersection> World::intersect(const utility::Ray& ray) const noexcept{
  std::vector<geometry::Intersection> intersections;
  for(const auto& object : objects_){
    const auto objectIntersections = object->intersect(ray);
    intersections.insert(intersections.end(), objectIntersections.begin(), objectIntersections.end());
  }
  std::ranges::sort(intersections, {}, [](const auto& intersection){ return intersection.dist; });
  return intersections;
}

bool World::intersectShadow(const utility::Ray& ray, double distanceToLight) const noexcept{
  return std::ranges::any_of(objects_, [&](const auto &object)
                             {
    const auto objectIntersections = object->intersect(ray);
    const auto hit = geometry::hit(objectIntersections);
    return hit && hit->object->hasShadow() && hit->dist > 0.0f && hit->dist < distanceToLight; });
}

utility::Color World::shadeHit(const geometry::Computations& comps, size_t recursionLimit) const noexcept{
  auto surfaceColor   = utility::Color{0,0,0};
  auto reflectedColor = utility::Color{0,0,0};
  auto refractedColor = utility::Color{0,0,0};
  for(const auto& light : this->lights_) {
    surfaceColor += scene::lighting(comps.intersection.object, light, comps.point, comps.eyeVector, 
                                    comps.normalVector, this->isShadowed(light, comps.overPoint));
    reflectedColor += this->reflectedColor(comps, recursionLimit);
    refractedColor += this->refractedColor(comps, recursionLimit);
  }
  if(comps.intersection.object->material().reflectance() > 0 && comps.intersection.object->material().transparency() > 0){
    auto reflectance = geometry::schlick(comps);
    return surfaceColor + reflectedColor * reflectance + refractedColor * (1 - reflectance);
  } else {
    return surfaceColor + reflectedColor + refractedColor;
  }
}

utility::Color World::reflectedColor(const geometry::Computations& comps, size_t recursionLimit) const noexcept{
  if(comps.intersection.object->material().reflectance() == 0) return utility::Color{0,0,0};
  else if (recursionLimit == 0) return utility::Color{0,0,0};

  auto reflectiedRay = utility::Ray(comps.overPoint, comps.reflectVector);
  return this->colorAt(reflectiedRay, recursionLimit - 1) * comps.intersection.object->material().reflectance();
}

utility::Color World::refractedColor(const geometry::Computations& comps, size_t recursionLimit) const noexcept{
  if(comps.intersection.object->material().transparency() == 0) return utility::Color{0,0,0};
  if(recursionLimit == 0) return utility::Color{0,0,0};

  auto nRatio = comps.n1 / comps.n2;
  auto cosI = comps.eyeVector.dot(comps.normalVector);
  auto sin2T = nRatio * nRatio * (1 - cosI * cosI); // basically solving snell's law

  // Total internal reflection
  if(sin2T > 1) return utility::Color{0,0,0};

  // Calculate refracted ray then its color
  auto cosT = std::sqrt(1.0 - sin2T);
  auto direction = comps.normalVector * (nRatio * cosI - cosT) - comps.eyeVector * nRatio;
  auto refractedRay = utility::Ray(comps.underPoint, direction);

  return this->colorAt(refractedRay, recursionLimit - 1) * comps.intersection.object->material().transparency();
}

utility::Color World::colorAt(const utility::Ray& ray, size_t recursionLimit) const noexcept{
  const auto intersections          = this->intersect(ray);
  const auto hit                    = geometry::hit(intersections);
  if (!hit.has_value()) return utility::Color{0,0,0};
  const auto intersect_computations = geometry::prepareComputations(*hit, ray, intersections);
  return this->shadeHit(intersect_computations, recursionLimit);
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

  auto s1 = geometry::makeSphere();
  s1->setMaterial(material::Material(utility::Color(0.8, 1.0, 0.6), 0.1, 0.7, 0.2));
  auto s2 = geometry::makeSphere();
  s2->setTransform(utility::transformations::scaling(0.5, 0.5, 0.5));
  world.objects_.emplace_back(s1);
  world.objects_.emplace_back(s2);

  return world;
}

} // namespace scene
} // namespace raytracer