#include "RingPattern.hpp"

namespace raytracer {
namespace material {

bool operator==(const RingPattern& lhs, const RingPattern& rhs) noexcept {
  return lhs.a == rhs.a && lhs.b == rhs.b;
}

} // namespace material
} // namespace raytracer