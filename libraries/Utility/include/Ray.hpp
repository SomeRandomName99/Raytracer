#ifndef RAY_H
#define RAY_H

#include "Tuple.hpp"
#include "Matrix.hpp"

namespace raytracer {
namespace utility {

struct Ray {
  Ray(Tuple origin, Tuple direction) noexcept
      : origin{origin}, direction{direction} {}
  explicit Ray() noexcept = default;

  Tuple position(const float time) const noexcept;

  Tuple origin;
  Tuple direction;
};

} // namespace utility
} // namespace raytracer

#endif // RAY_H