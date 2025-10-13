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
using utility::Color;
using utility::Tuple;
using utility::Point;

// Buffer reused across recursive calls to avoid allocations
static thread_local Arena<Intersection> intersectionsBuffer(GB(10));

void intersect(const Ray& ray) const noexcept{
  intersectionsBuffer.clear();
  for(const auto& object : objects_){
    object->intersect(ray, intersectionsBuffer);
  }
}

inline bool isShadowed(const PointLight& light, const Tuple& point) const noexcept{
  const auto pointToLightVector = light.position - point;
  const auto pointToLightDistance = pointToLightVector.magnitude();
  const auto pointToLightDirection = pointToLightVector.normalize();
  const auto pointToLightRay = Ray(point, pointToLightDirection);

  for(const auto& object : objects_){
    if(!object->hasShadow()) continue;
    intersect(pointToLightRay);
    for(const auto& intersection : intersectionsBuffer){
      if(intersection.dist > 0.0f && intersection.dist < pointToLightDistance){
        return true;
      }
    }
  }
  return false;
}

Color lighting(const geometry::ShapeBase* object, const PointLight& light, const Tuple& point, 
                        const Tuple& eyeVector, const Tuple& normalVector, const bool inShadow) noexcept{
  Color color;
  if (object->material().pattern()) {
    color = (*object->material().pattern()).drawPatternAt(object, point);
  } else {
    color = object->material().surfaceColor();
  }
  const auto effectiveColor = color * light.intensity;
  const auto lightVector = (light.position - point).normalize();
  const auto ambient = effectiveColor * object->material().ambient();

  if (inShadow) {
    return ambient; // specular and diffuse lighting are not relevant if the point is in shadow
  }

  auto lightDotNormal = lightVector.dot(normalVector);
  Color diffuse;
  Color specular;
  if (lightDotNormal < 0) {
    diffuse = Color(0,0,0);
    specular = Color(0,0,0);
  } else {
    diffuse = effectiveColor * object->material().diffuse() * lightDotNormal;

    auto reflectVector = (-lightVector).reflect(normalVector);
    auto reflectDotEye = reflectVector.dot(eyeVector);
    if (reflectDotEye <= 0) {
      specular = Color(0,0,0);
    } else {
      auto factor = std::pow(reflectDotEye, object->material().shininess());
      specular = light.intensity * object->material().specular() * factor;
    }
  }

  return ambient + diffuse + specular;
}

Color reflect(const Tuple& reflectVector, const Tuple& surfaceOffsetPoint, size_t recursionLimit) const noexcept{
  if(comps.intersection.object->material().reflectance() == 0) return Color{0,0,0};
  else if (recursionLimit == 0) return Color{0,0,0};

  auto reflectedRay = Ray(comps.surfaceOffsetPoint, comps.reflectVector);
  return this->colorAt(reflectedRay, recursionLimit - 1) * comps.intersection.object->material().reflectance();
}

Color refract(const Tuple& normalVector, const Tuple& eyeVector, const Tuple& internalOffsetPoint, size_t recursionLimit) const noexcept{
  if(comps.intersection.object->material().transparency() == 0) return Color{0,0,0};
  if(recursionLimit == 0) return Color{0,0,0};

  static thread_local Arena<const geometry::ShapeBase*> unexitedShapes(GB(10));
  unexitedShapes.clear();
  float n1,n2; 
  for(const auto& i: intersections){
    if(i == intersection){
      size_t size = unexitedShapes.size;
      n1 = size == 0 ? 1.0 : unexitedShapes[size - 1]->material().refractiveIndex();
    }

    auto found = std::find(unexitedShapes.begin(), unexitedShapes.end(), i.object);
    if(found != unexitedShapes.end()){
      size_t size = unexitedShapes.size;
      size_t index = found - unexitedShapes.begin();
      unexitedShapes[index] = unexitedShapes[size - 1];
      unexitedShapes.popBack();
    } else {
      unexitedShapes.pushBack(i.object);
    }

    if(i == intersection){
      size_t size = unexitedShapes.size;
      n2 = size == 0 ? 1.0 : unexitedShapes[size - 1]->material().refractiveIndex();
      break;
    }
  }

  auto nRatio = n1/n2;
  auto cosI = comps.eyeVector.dot(comps.normalVector);
  auto sin2T = nRatio * nRatio * (1 - cosI * cosI); // basically solving snell's law

  // Total internal reflection
  if(sin2T > 1) return Color{0,0,0};

  // Calculate refracted ray then its color
  auto cosT = std::sqrt(1.0 - sin2T);
  auto direction = comps.normalVector * (nRatio * cosI - cosT) - comps.eyeVector * nRatio;
  auto refractedRay = Ray(comps.internalOffsetPoint, direction);

  return this->colorAt(refractedRay, recursionLimit - 1) * comps.intersection.object->material().transparency();
}

Color World::colorAt(const Ray& ray, size_t recursionLimit) const noexcept{
  this->intersect(ray, intersections);
  std::ranges::sort(intersections, {}, [](const auto& intersection){ return intersection.dist; });
  Intersection hit{nullptr, std::numeric_limits<double>::min()};
  for(auto& i : intersections){
    if(i.dist > hit.dist){
      hit = i;
    }
  }
  if (hit.dist < 0.0f) return Color{0,0,0};

  // Calculate shading info
  auto point = ray.position(hit.dist);
  auto normalVector = hit.object->normalAt(point);
  auto reflectVector = ray.direction_.reflect(normalVector);
  auto eyeVector = -ray.direction_;
  if(normalVector.dot(eyeVector) < 0){
    normalVector = -normalVector;
  }
  auto surfaceOffsetPoint  = point + normalVector * SHADOW_OFFSET;
  auto internalOffsetPoint = point - normalVector * SHADOW_OFFSET;

  auto surfaceColor   = Color{0,0,0};
  auto refractedColor = Color{0,0,0};
  auto reflectedColor = Color{0,0,0};
  /* Refract be called first because any recursive call will overwrite the intersections buffer and
     we will lose the ability to calculate the refractive indices. */ 
  refractedColor += refract(reflectVector, surfaceOffsetPoint, recursionLimit); 
  reflectedColor += reflect(normalVector, eyeVector, internalOffsetPoint, recursionLimit);
  for(const auto& light : this->lights_) {
    surfaceColor += scene::lighting(comps.intersection.object, light, comps.point, comps.eyeVector, 
                                    comps.normalVector, this->isShadowed(light, comps.surfaceOffsetPoint));
  }

  if(comps.intersection.object->material().reflectance() > 0 && comps.intersection.object->material().transparency() > 0){
    auto reflectance = geometry::schlick(comps);
    return surfaceColor + reflectedColor * reflectance + refractedColor * (1 - reflectance);
  } else {
    return surfaceColor + reflectedColor + refractedColor;
  } 
}

} // namespace scene
} // namespace raytracer