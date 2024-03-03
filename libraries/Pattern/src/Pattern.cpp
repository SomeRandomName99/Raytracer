#include <cmath>

#include "Pattern.hpp"
#include "Shape.hpp"

namespace raytracer {
namespace material {

utility::Color stripe_at_object(const StripePattern& pattern, const Shape& object, const utility::Tuple& worldPoint) noexcept{
  const auto objectPoint  = object.inverseTransformation()* worldPoint;
  const auto patternPoint = pattern.inverseTransformation()* objectPoint;
  return stripe_at(pattern, patternPoint);
}

utility::Color stripe_at(const StripePattern& pattern, const utility::Tuple& point) noexcept{
  if(static_cast<int>(std::floor(point.x())) % 2 == 0){
    return pattern.a;
  } else {
    return pattern.b;
  }
}

} // namespace material
} // namespace raytracer