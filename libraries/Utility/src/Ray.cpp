#include "Ray.hpp"

namespace raytracer {
namespace utility {

Tuple Ray::position(const double distance) const noexcept {
  return this->origin + this->direction * distance;
}

} // namespace utility
} // namespace raytracer