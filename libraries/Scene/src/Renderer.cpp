#include <optional>
#include <algorithm>
#include <utility>

#include "World.hpp"
#include "Intersections.hpp"
#include "Material.hpp"
#include "FloatUtils.hpp"
#include "Transformations.hpp"
#include "Shape.hpp"

namespace raytracer {
namespace scene {

using utility::Arena;
using utility::Ray;
using geometry::Intersection;
using utility::Color;
using utility::Tuple;
using utility::Point;

// Buffer reused across recursive calls to avoid allocations
// static thread_local Arena<Intersection> intersectionsBuffer(GB(10));
static Arena<Intersection> intersectionsBuffer(GB(10));

static inline void intersect(const Ray& ray, const World& world) noexcept{
  intersectionsBuffer.clear();
  for(const auto& object: world.objects){
    Ray transformedRay{object.inverseTransform * ray.origin, object.inverseTransform * ray.direction};
    // if(!object.boundingBox.intersect(transformedRay)){continue;}
    localIntersect(transformedRay, object, intersectionsBuffer, world.circularSolidData);
  }
}

inline Color lighting(const WorldObject& object, const PointLight& light, const utility::Tuple& point, 
                      const utility::Tuple& eyeVector, const utility::Tuple& normalVector, const World& world) noexcept{
  Color color;
  const auto& material = world.materials[object.MaterialIndex];
  if (material.patternIndex != -1) {
    // TODO: Handle groups where multiple transformations are applied(those from the parents)
    auto objectPoint = object.inverseTransform * point;
    color = drawPatternAt(world.patterns[material.patternIndex], objectPoint);
  } else {
    color = material.surfaceColor;
  }
  const auto pointToLightVector = light.position - point;
  const auto pointToLightDistance = pointToLightVector.magnitude();
  const auto pointToLightDirection = pointToLightVector.normalize();
  const auto effectiveColor = color * light.intensity;
  const auto lightVector = pointToLightDirection;
  const auto ambient = effectiveColor * material.ambient;


  bool inShadow = false;
  for(const auto& object : world.objects){
    if(!object.hasShadow) continue;
    intersect(Ray(point, pointToLightDirection), world);
    for(const auto& intersection : intersectionsBuffer){
      if(intersection.dist > 0.0f && intersection.dist < pointToLightDistance){
        inShadow = true;
      }
    }
  }

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
    diffuse = effectiveColor * material.diffuse * lightDotNormal;

    auto reflectVector = (-lightVector).reflect(normalVector);
    auto reflectDotEye = reflectVector.dot(eyeVector);
    if (reflectDotEye <= 0) {
      specular = Color(0,0,0);
    } else {
      auto factor = std::pow(reflectDotEye, material.shininess);
      specular = light.intensity * material.specular * factor;
    }
  }

  return ambient + diffuse + specular;
}

static inline float schlick(const Tuple& eyeVector, const Tuple& normalVector, float n1, float n2) {
  auto cos = eyeVector.dot(normalVector);
  if(n1 > n2){
    auto n = n1 / n2;
    auto sin2_t = n * n * (1 - cos * cos);
    if(sin2_t > 1) // total internal reflection
      return 1.0;

    auto cos_t = std::sqrt(1.0 - sin2_t);
    cos = cos_t;
  }

  auto r0 = std::pow((n1 - n2) / (n1 + n2), 2);
  return r0 + (1 - r0) * std::pow(1 - cos, 5);
}

static inline std::pair<float, float> calculateRefractiveIndices(const World& world, Intersection intersection) {
  static thread_local Arena<const WorldObject*> unexitedShapes(GB(1));
  unexitedShapes.clear();
  float n1,n2; 
  for(const auto& i: intersectionsBuffer){
    if(i == intersection){
      size_t size = unexitedShapes.size;
      n1 = size == 0 ? 1.0 : world.materials[unexitedShapes[size - 1]->MaterialIndex].refractiveIndex;
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
      n2 = size == 0 ? 1.0 : world.materials[unexitedShapes[size - 1]->MaterialIndex].refractiveIndex;
      break;
    }
  }
  return {n1, n2};
}

Color colorAt(const Ray& ray, const World& world, size_t recursionLimit) noexcept{
  intersect(ray, world);
  std::ranges::sort(intersectionsBuffer, {}, [](const auto& intersection){ return intersection.dist; });
  Intersection hit{nullptr, std::numeric_limits<float>::max()};
  for(auto& i : intersectionsBuffer){
    if(i.dist < hit.dist && i.dist > 0.0f){
      hit = i;
    }
  }
  if (hit.object==nullptr || hit.dist < 0.0f || recursionLimit == 0) return Color{0,0,0};

  auto point = ray.position(hit.dist);
  auto normalVector = normalAt(*hit.object, point, world.circularSolidData).normalize();
  auto reflectVector = ray.direction.reflect(normalVector);
  auto eyeVector = -ray.direction;
  if(normalVector.dot(eyeVector) < 0){
    normalVector = -normalVector;
  }
  auto surfaceOffsetPoint  = point + normalVector * SHADOW_OFFSET;
  auto internalOffsetPoint = point - normalVector * SHADOW_OFFSET;

  auto surfaceColor   = Color{0,0,0};
  auto refractedColor = Color{0,0,0};
  auto reflectedColor = Color{0,0,0};
  // This function has to be called before any calls to lighting or recursive calls to colorAt because the intersectionBuffer will then be modified
  auto [n1, n2] = calculateRefractiveIndices(world, hit);
  for(const auto& light : world.lights) {
    surfaceColor += scene::lighting(*hit.object, light, point, eyeVector, 
                                    normalVector, world);
  }

  const auto& material = world.materials[hit.object->MaterialIndex];
  if(material.reflectance != 0){
    auto reflectedRay = Ray(surfaceOffsetPoint, reflectVector);
    reflectedColor += colorAt(reflectedRay, world, recursionLimit-1) * material.reflectance;
  }

  if(material.transparency != 0){
    auto nRatio = n1/n2;
    auto cosI = eyeVector.dot(normalVector);
    auto sin2T = nRatio * nRatio * (1 - cosI * cosI); // basically solving snell's law

    // Total internal reflection
    if(sin2T > 1) return Color{0,0,0};

    // Calculate refracted ray then its color
    auto cosT = std::sqrt(1.0 - sin2T);
    auto direction = normalVector * (nRatio * cosI - cosT) - eyeVector * nRatio;
    auto refractedRay = Ray(internalOffsetPoint, direction);

    refractedColor += colorAt(refractedRay, world, recursionLimit - 1) * material.transparency;
  }

  if(material.reflectance > 0 && material.transparency > 0){
    auto reflectance = schlick(eyeVector, normalVector, n1, n2);
    return surfaceColor + reflectedColor * reflectance + refractedColor * (1 - reflectance);
  } else {
    return surfaceColor + reflectedColor + refractedColor;
  } 
}

} // namespace scene
} // namespace raytracer