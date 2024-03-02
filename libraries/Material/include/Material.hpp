#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <optional>

#include "Color.hpp"
#include "Tuple.hpp"
#include "Pattern.hpp"

namespace raytracer {
namespace material {

class Material {
public:
  Material() noexcept
      : surfaceColor_{utility::Color(1.0, 1.0, 1.0)}, pattern_{}, ambient_{0.1}, diffuse_{0.9}, specular_{0.9}, 
        shininess_{200.0} {}
  Material(utility::Color surfaceColor, std::optional<StripePattern> pattern = std::nullopt, double ambient = 0.1, double diffuse = 0.9, 
           double specular = 0.9, double shininess = 200) noexcept 
      : surfaceColor_{surfaceColor}, pattern_{pattern}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess} {}

  utility::Color surfaceColor_;
  std::optional<StripePattern> pattern_;
  double ambient_;
  double diffuse_;
  double specular_;
  double shininess_;
};

} // namespace raytracer
} // namespace scene

bool operator==(const raytracer::material::Material& lhs, const raytracer::material::Material& rhs) noexcept;

#endif // MATERIAL_HPP