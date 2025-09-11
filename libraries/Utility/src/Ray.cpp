#include "Ray.hpp"

namespace raytracer {
namespace utility {

Tuple Ray::position(const double distance) const noexcept {
  return this->origin_ + this->direction_ * distance;
}

} // namespace utility
} // namespace raytracer