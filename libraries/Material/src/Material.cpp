#include "Pattern.hpp"
#include "Material.hpp"

namespace raytracer {
namespace material {

Material::Material() noexcept
    : surfaceColor_{utility::Color(1.0, 1.0, 1.0)}, pattern_{}, ambient_{0.1}, diffuse_{0.9}, specular_{0.9}, 
      shininess_{200.0} {}

Material::Material(utility::Color surfaceColor, std::optional<StripePattern> pattern, double ambient, double diffuse, 
          double specular, double shininess) noexcept 
    : surfaceColor_{surfaceColor}, pattern_{pattern}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess} {}

} // namespace material
} // namespace raytracer

/* TODO: move inside the namespace */
bool operator==(const raytracer::material::Material& lhs, const raytracer::material::Material& rhs) noexcept{
  return lhs.surfaceColor_ == rhs.surfaceColor_ &&
         lhs.ambient_ == rhs.ambient_ &&
         lhs.diffuse_ == rhs.diffuse_ &&
         lhs.specular_ == rhs.specular_ &&
         lhs.shininess_ == rhs.shininess_;
}