#include "Ray.hpp"

namespace raytracer {
namespace utility {

Tuple Ray::position(const float distance) const noexcept {
  return this->origin_ + this->direction_ * distance;
}

Ray transform(const Ray &ray, const Matrix<4,4> &transformation) noexcept{
  return Ray{transformation*ray.origin_, transformation*ray.direction_};
}

} // namespace utility
} // namespace raytracer