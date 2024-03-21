#include "GradientPattern.hpp"

namespace raytracer {
namespace material {

bool operator==(const GradientPattern& lhs, const GradientPattern& rhs) noexcept {
  return lhs.a == rhs.a && lhs.b == rhs.b;
}

} // namespace material
} // namespace raytracer