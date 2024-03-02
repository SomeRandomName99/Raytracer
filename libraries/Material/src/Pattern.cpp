#include <cmath>

#include "Pattern.hpp"

namespace raytracer {
namespace material {

utility::Color stripe_at(const StripePattern& pattern, const utility::Tuple& point) noexcept{
  if(static_cast<int>(std::floor(point.x())) % 2 == 0){
    return pattern.a;
  } else {
    return pattern.b;
  }
}

} // namespace material
} // namespace raytracer