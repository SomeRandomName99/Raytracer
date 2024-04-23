#include "Pattern.hpp"
#include "Material.hpp"

namespace raytracer {
namespace material {

Material::Material() noexcept
    : surfaceColor_{utility::Color(1.0, 1.0, 1.0)}, ambient_{0.1}, diffuse_{0.9}, specular_{0.9}, 
      shininess_{200.0}, reflectance_{0}, transparency_{0}, refractiveIndex_{1} {}

Material::Material(utility::Color surfaceColor, double ambient, double diffuse, 
                   double specular, double shininess, double reflectance, double transparency, double refractiveIndex) noexcept 
    : surfaceColor_{surfaceColor}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess},
      reflectance_{reflectance}, transparency_{transparency}, refractiveIndex_{refractiveIndex} {}

Material::~Material() = default;

Material::Material(Material &&other) noexcept = default;
Material& Material::operator=(Material&& other) noexcept = default;

void Material::setSurfaceColor(const utility::Color& surfaceColor) noexcept {
  surfaceColor_ = surfaceColor;
}
const utility::Color& Material::surfaceColor() const noexcept {
  return surfaceColor_;
}

void Material::setPattern(const Pattern& pattern) noexcept {
  pattern_ = std::make_unique<Pattern>(pattern);
}
void Material::setPattern(std::unique_ptr<Pattern> pattern) noexcept {
  pattern_ = std::move(pattern);
}
const std::unique_ptr<Pattern>& Material::pattern() const noexcept{
  return pattern_;
}

void Material::setAmbient(double ambient) noexcept {
  ambient_ = ambient;
}
double Material::ambient() const noexcept {
  return ambient_;
}

void Material::setDiffuse(double diffuse) noexcept {
  diffuse_ = diffuse;
}
double Material::diffuse() const noexcept {
  return diffuse_;
}

void Material::setSpecular(double specular) noexcept {
  specular_ = specular;
}
double Material::specular() const noexcept {
  return specular_;
}

void Material::setShininess(double shininess) noexcept {
  shininess_ = shininess;
}
double Material::shininess() const noexcept {
  return shininess_;
}

void Material::setReflectance(double reflectance) noexcept {
  reflectance_ = reflectance;
}
double Material::reflectance() const noexcept {
  return reflectance_;
}

void Material::setTransparency(double transparency) noexcept {
  transparency_ = transparency;
}
double Material::transparency() const noexcept {
  return transparency_;
}

void Material::setRefractiveIndex(double refractiveIndex) noexcept {
  refractiveIndex_ = refractiveIndex;
}
double Material::refractiveIndex() const noexcept {
  return refractiveIndex_;
}

bool operator==(const raytracer::material::Material& lhs, const raytracer::material::Material& rhs) noexcept{
  return lhs.surfaceColor() == rhs.surfaceColor() &&
         lhs.pattern() == rhs.pattern() &&
         lhs.ambient() == rhs.ambient() &&
         lhs.diffuse() == rhs.diffuse() &&
         lhs.specular() == rhs.specular() &&
         lhs.shininess() == rhs.shininess();
}

} // namespace material
} // namespace raytracer
