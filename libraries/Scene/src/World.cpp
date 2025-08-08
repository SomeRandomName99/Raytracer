#include <optional>
#include <algorithm>

#include "World.hpp"
#include "Intersections.hpp"
#include "Material.hpp"
#include "FloatUtils.hpp"
#include "Transformations.hpp"

namespace raytracer {
namespace scene {

using utility::Arena;
using utility::Ray;
using geometry::Intersection;
using geometry::Computations;
using utility::Color;
using geometry::hit;
using utility::Tuple;
using utility::Point;

void World::intersect(const Ray& ray, Arena<Intersection>& intersections) const noexcept{
  intersections.clear();
  for(const auto& object : objects_){
    object->intersect(ray, intersections);
  }
  std::ranges::sort(intersections, {}, [](const auto& intersection){ return intersection.dist; });
}

bool World::intersectShadow(const Ray& ray, double distanceToLight, Arena<Intersection>& intersections) const noexcept{
  return std::ranges::any_of(objects_, [&](const auto &object)
  {
    intersections.clear();
    object->intersect(ray, intersections);
    const auto hit = geometry::hit(intersections);
    return hit && hit->object->hasShadow() && hit->dist > 0.0f && hit->dist < distanceToLight; 
  });
}

Color World::shadeHit(const Computations& comps, Arena<Intersection>& intersections, size_t recursionLimit) const noexcept{
  auto surfaceColor   = Color{0,0,0};
  auto reflectedColor = Color{0,0,0};
  auto refractedColor = Color{0,0,0};
  for(const auto& light : this->lights_) {
    surfaceColor += scene::lighting(comps.intersection.object, light, comps.point, comps.eyeVector, 
                                    comps.normalVector, this->isShadowed(light, comps.overPoint, intersections));
    reflectedColor += this->reflectedColor(comps, intersections, recursionLimit);
    refractedColor += this->refractedColor(comps, intersections, recursionLimit);
  }
  if(comps.intersection.object->material().reflectance() > 0 && comps.intersection.object->material().transparency() > 0){
    auto reflectance = geometry::schlick(comps);
    return surfaceColor + reflectedColor * reflectance + refractedColor * (1 - reflectance);
  } else {
    return surfaceColor + reflectedColor + refractedColor;
  }
}

Color World::reflectedColor(const Computations& comps, Arena<Intersection>& intersections, size_t recursionLimit) const noexcept{
  if(comps.intersection.object->material().reflectance() == 0) return Color{0,0,0};
  else if (recursionLimit == 0) return Color{0,0,0};

  auto reflectedRay = utility::Ray(comps.overPoint, comps.reflectVector);
  intersections.clear();
  return this->colorAt(reflectedRay, intersections, recursionLimit - 1) * comps.intersection.object->material().reflectance();
}

Color World::refractedColor(const Computations& comps, Arena<Intersection>& intersections, size_t recursionLimit) const noexcept{
  if(comps.intersection.object->material().transparency() == 0) return Color{0,0,0};
  if(recursionLimit == 0) return Color{0,0,0};

  auto nRatio = comps.n1 / comps.n2;
  auto cosI = comps.eyeVector.dot(comps.normalVector);
  auto sin2T = nRatio * nRatio * (1 - cosI * cosI); // basically solving snell's law

  // Total internal reflection
  if(sin2T > 1) return Color{0,0,0};

  // Calculate refracted ray then its color
  auto cosT = std::sqrt(1.0 - sin2T);
  auto direction = comps.normalVector * (nRatio * cosI - cosT) - comps.eyeVector * nRatio;
  auto refractedRay = utility::Ray(comps.underPoint, direction);

  intersections.clear();
  return this->colorAt(refractedRay, intersections, recursionLimit - 1) * comps.intersection.object->material().transparency();
}

Color World::colorAt(const Ray& ray, Arena<Intersection>& intersections, size_t recursionLimit) const noexcept{
  this->intersect(ray, intersections);
  const auto hit = geometry::hit(intersections);
  if (!hit.has_value()) return utility::Color{0,0,0};
  const auto intersect_computations = geometry::prepareComputations(*hit, ray, intersections);
  return this->shadeHit(intersect_computations, intersections, recursionLimit);
}

bool World::isShadowed(const PointLight& light, const Tuple& point, Arena<Intersection>& intersections) const noexcept{
  const auto pointToLightVector = light.position_ - point;
  const auto pointToLightDistance = pointToLightVector.magnitude();
  const auto pointToLightDirection = pointToLightVector.normalize();
  const auto pointToLightRay = Ray(point, pointToLightDirection);

  return this->intersectShadow(pointToLightRay, pointToLightDistance, intersections);
}

World defaultWorld() noexcept{
  World world;

  const auto light = scene::PointLight{Color{1,1,1}, Point(-10,10,-10)};
  world.lights_.push_back(light);

  auto s1 = geometry::makeSphere();
  s1->setMaterial(material::Material(Color(0.8, 1.0, 0.6), 0.1, 0.7, 0.2));
  auto s2 = geometry::makeSphere();
  s2->setTransform(utility::transformations::scaling(0.5, 0.5, 0.5));
  world.objects_.emplace_back(s1);
  world.objects_.emplace_back(s2);

  return world;
}

} // namespace scene
} // namespace raytracer