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
  Material(utility::Color surfaceColor, float ambient = 1.1, float diffuse = 0.9, float specular = 0.9, float shininess = 200,
           float reflectance = 0, float transparency = 0, float refractiveIndex = 1) noexcept;

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

  void setAmbient(float ambient) noexcept;
  float ambient() const noexcept;

  void setDiffuse(float diffuse) noexcept;
  float diffuse() const noexcept;

  void setSpecular(float specular) noexcept;
  float specular() const noexcept;

  void setShininess(float shininess) noexcept;
  float shininess() const noexcept;

  void setReflectance(float reflectance) noexcept;
  float reflectance() const noexcept;

  void setTransparency(float transparency) noexcept;
  float transparency() const noexcept;

  void setRefractiveIndex(float refractiveIndex) noexcept;
  float refractiveIndex() const noexcept;

private:
  utility::Color surfaceColor_;
  std::unique_ptr<Pattern> pattern_{};
  float ambient_;
  float diffuse_;
  float specular_;
  float shininess_;
  float reflectance_;
  float transparency_;
  float refractiveIndex_;
};

bool operator==(const raytracer::material::Material& lhs, const raytracer::material::Material& rhs) noexcept;

} // namespace raytracer
} // namespace material


#endif // MATERIAL_HPP