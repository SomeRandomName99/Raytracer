#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Color.hpp"
#include "Tuple.hpp"

namespace raytracer {
namespace material {

class Material {
public:
  Material()
      : surfaceColor_{utility::Color(1.0, 1.0, 1.0)}, ambient_{0.1}, diffuse_{0.9}, specular_{0.9}, 
        shininess_{200.0} {}
  Material(utility::Color surfaceColor, double ambient = 0.1, double diffuse = 0.9, 
           double specular = 0.9, double shininess = 200) 
        : surfaceColor_{surfaceColor}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess} {}

  utility::Color surfaceColor_;
  double ambient_;
  double diffuse_;
  double specular_;
  double shininess_;
};

} // namespace raytracer
} // namespace scene

bool operator==(const raytracer::material::Material& lhs, const raytracer::material::Material& rhs);

#endif // MATERIAL_HPP