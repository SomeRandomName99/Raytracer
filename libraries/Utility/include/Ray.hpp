#ifndef RAY_H
#define RAY_H

#include "Tuple.hpp"
#include "Matrix.hpp"

namespace raytracer {
namespace utility {

class Ray {
public: 
  Ray(Tuple origin, Tuple direction) noexcept
      : origin_{origin}, direction_{direction} {}
  explicit Ray() noexcept = default;

  Tuple position(const double time) const noexcept;

  Tuple origin_;
  Tuple direction_;
};

inline Ray transform(const Ray &ray, const Matrix<4,4> &transformation) noexcept {
  return Ray(transformation * ray.origin_, transformation * ray.direction_);
}

} // namespace utility
} // namespace raytracer

#endif // RAY_H