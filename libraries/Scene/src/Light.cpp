#include "Light.hpp"

#include <cmath>

namespace raytracer {
namespace scene {

utility::Color lighting(const material::Material& material, const PointLight& light, const utility::Tuple& point, 
                        const utility::Tuple& eyeVector, const utility::Tuple& normalVector, const bool inShadow) noexcept{
  utility::Color color;
  if (material.pattern_.has_value()) {
    color = stripe_at(material.pattern_.value(), point);
  } else {
    color = material.surfaceColor_;
  }
  // combine the surface color with the light's color/intensity
  const auto effectiveColor = color * light.intensity_;

  // find the direction to the light source
  const auto lightVector = (light.position_ - point).normalize();

  // compute the ambient contribution
  const auto ambient = effectiveColor * material.ambient_;

  if (inShadow) {
    return ambient; // specular and diffuse lighting are not relevant if the point is in shadow
  }

  // lightDotNormal represents the cosine of the angle between the light vector and the normal vector. A negative number
  // means the light is on the other side of the surface.
  auto lightDotNormal = lightVector.dot(normalVector);
  utility::Color diffuse;
  utility::Color specular;
  if (lightDotNormal < 0) {
    diffuse = utility::Color(0,0,0);
    specular = utility::Color(0,0,0);
  } else {
    // compute the diffuse contribution
    diffuse = effectiveColor * material.diffuse_ * lightDotNormal;

    // reflectDotEye represents the cosine of the angle between the reflection vector and the eye vector. A negative number
    // means the light reflects away from the eye.
    auto reflectVector = (-lightVector).reflect(normalVector);
    auto reflectDotEye = reflectVector.dot(eyeVector);
    if (reflectDotEye <= 0) {
      specular = utility::Color(0,0,0);
    } else {
      // compute the specular contribution
      auto factor = std::pow(reflectDotEye, material.shininess_);
      specular = light.intensity_ * material.specular_ * factor;
    }
  }

  // Add the three contributions together to get the final shading
  return ambient + diffuse + specular;
}

} // namespace scene
} // namespace raytracer