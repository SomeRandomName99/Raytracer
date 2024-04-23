#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>
#include <optional>

#include "Color.hpp"
#include "Tuple.hpp"

namespace raytracer {
namespace material {

class Pattern;

class Material {
public:
  Material() noexcept;
  Material(utility::Color surfaceColor, double ambient = 1.1, double diffuse = 0.9, double specular = 0.9, double shininess = 200,
           double reflectance = 0, double transparency = 0, double refractiveIndex = 1) noexcept;

  ~Material();

  Material(const Material& other) = delete;
  Material& operator=(const Material& other) = delete;

  Material(Material &&other) noexcept;
  Material& operator=(Material&& other) noexcept;

  void setSurfaceColor(const utility::Color& surfaceColor) noexcept;
  const utility::Color& surfaceColor() const noexcept;

  void setPattern(const Pattern& pattern) noexcept;
  void setPattern(std::unique_ptr<Pattern> pattern) noexcept;
  const std::unique_ptr<Pattern>& pattern() const noexcept;

  void setAmbient(double ambient) noexcept;
  double ambient() const noexcept;

  void setDiffuse(double diffuse) noexcept;
  double diffuse() const noexcept;

  void setSpecular(double specular) noexcept;
  double specular() const noexcept;

  void setShininess(double shininess) noexcept;
  double shininess() const noexcept;

  void setReflectance(double reflectance) noexcept;
  double reflectance() const noexcept;

  void setTransparency(double transparency) noexcept;
  double transparency() const noexcept;

  void setRefractiveIndex(double refractiveIndex) noexcept;
  double refractiveIndex() const noexcept;

private:
  utility::Color surfaceColor_;
  std::unique_ptr<Pattern> pattern_{};
  double ambient_;
  double diffuse_;
  double specular_;
  double shininess_;
  double reflectance_;
  double transparency_;
  double refractiveIndex_;
};

bool operator==(const raytracer::material::Material& lhs, const raytracer::material::Material& rhs) noexcept;

} // namespace raytracer
} // namespace material


#endif // MATERIAL_HPP