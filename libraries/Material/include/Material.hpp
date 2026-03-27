#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>
#include <optional>

#include "Color.hpp"
#include "Tuple.hpp"

namespace raytracer::material {

struct Material{
  utility::Color surfaceColor;
  float ambient;
  float diffuse;
  float specular;
  float shininess;
  float reflectance;
  float transparency;
  float refractiveIndex;
  int32_t patternIndex = -1;
};

static inline Material createDefaultMaterial() noexcept{
  return Material{utility::Color(1.0, 1.0, 1.0), 1.1, 0.9, 0.9, 200.0, 0, 0, 1.0, -1};
}

} // namespace raytracer


#endif // MATERIAL_HPP