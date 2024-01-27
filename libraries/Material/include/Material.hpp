#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Color.hpp"
#include "Tuple.hpp"

namespace raytracer {
namespace material {

class Material {
public:
  Material()
      : surfaceColor_{utility::Color(1.0f, 1.0f, 1.0f)}, ambient_{0.1f}, diffuse_{0.9f}, specular_{0.9f}, 
        shininess_{200.0f} {}
  Material(utility::Color surfaceColor, float ambient = 0.1f, float diffuse = 0.9f, 
           float specular = 0.9f, float shininess = 200) 
        : surfaceColor_{surfaceColor}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess} {}

  utility::Color surfaceColor_;
  float ambient_;
  float diffuse_;
  float specular_;
  float shininess_;
};

} // namespace raytracer
} // namespace scene

bool operator==(const raytracer::material::Material& lhs, const raytracer::material::Material& rhs);

#endif // MATERIAL_HPP