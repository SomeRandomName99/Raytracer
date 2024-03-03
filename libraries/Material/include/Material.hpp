#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <optional>

#include "Color.hpp"
#include "Tuple.hpp"

namespace raytracer {
namespace material {

class StripePattern;

class Material {
public:
  Material() noexcept;
  Material(utility::Color surfaceColor, std::optional<StripePattern> pattern = std::nullopt, double ambient = 0.1, double diffuse = 0.9, 
           double specular = 0.9, double shininess = 200) noexcept;

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