#include "Ray.hpp"

namespace raytracer {
namespace utility {

Tuple Ray::position(const float distance) const noexcept {
  return this->origin + this->direction * distance;
}

} // namespace utility
} // namespace raytracer