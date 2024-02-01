#include "Material.hpp"

bool operator==(const raytracer::material::Material& lhs, const raytracer::material::Material& rhs) noexcept{
  return lhs.surfaceColor_ == rhs.surfaceColor_ &&
         lhs.ambient_ == rhs.ambient_ &&
         lhs.diffuse_ == rhs.diffuse_ &&
         lhs.specular_ == rhs.specular_ &&
         lhs.shininess_ == rhs.shininess_;
}