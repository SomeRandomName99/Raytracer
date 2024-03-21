#include "StripePattern.hpp"

namespace raytracer {
namespace material {

bool operator==(const StripePattern& lhs, const StripePattern& rhs) noexcept {
  return lhs.a == rhs.a && lhs.b == rhs.b;
}

} // namespace material
} // namespace raytracer