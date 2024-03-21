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
  Material() noexcept;
  Material(utility::Color surfaceColor, std::optional<Pattern> pattern = std::nullopt, double ambient = 0.1, double diffuse = 0.9, 
           double specular = 0.9, double shininess = 200) noexcept;


// write setters and getters for all the private fields
  void setSurfaceColor(const utility::Color& surfaceColor) noexcept;
  const utility::Color& surfaceColor() const noexcept;

  template <typename PatternType>
  void setPattern(const PatternType& pattern) noexcept{
    pattern_ = pattern;
  }
  const std::optional<Pattern>& pattern() const noexcept;

  void setAmbient(double ambient) noexcept;
  double ambient() const noexcept;

  void setDiffuse(double diffuse) noexcept;
  double diffuse() const noexcept;

  void setSpecular(double specular) noexcept;
  double specular() const noexcept;

  void setShininess(double shininess) noexcept;
  double shininess() const noexcept;

private:
  utility::Color surfaceColor_;
  std::optional<Pattern> pattern_;
  double ambient_;
  double diffuse_;
  double specular_;
  double shininess_;
};

bool operator==(const raytracer::material::Material& lhs, const raytracer::material::Material& rhs) noexcept;

} // namespace raytracer
} // namespace material


#endif // MATERIAL_HPP