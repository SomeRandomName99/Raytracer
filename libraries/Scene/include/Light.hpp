#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Color.hpp"
#include "Tuple.hpp"

namespace raytracer {
namespace scene {

class PointLight{
public:
  PointLight(utility::Color intensity, utility::Tuple position): intensity_{intensity}, position_{position} {}

  utility::Color intensity_;
  utility::Tuple position_;
};

} // namespace raytracer
} // namespace scene

#endif // LIGHT_HPP