#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Color.hpp"
#include "Tuple.hpp"

namespace raytracer {
namespace scene {

class Shape;
class Material;

class PointLight{
public:
  PointLight(utility::Color intensity, utility::Tuple position) noexcept: intensity_{intensity}, position_{position} {}

  utility::Color intensity_;
  utility::Tuple position_;
};

utility::Color lighting(const Material& material, const Shape& object, const PointLight& light, const utility::Tuple& position, 
                        const utility::Tuple& eyeVector, const utility::Tuple& normalVector, const bool inShadow) noexcept;

} // namespace raytracer
} // namespace scene

#endif // LIGHT_HPP