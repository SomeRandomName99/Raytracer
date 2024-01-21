#include "Ray.hpp"

namespace raytracer {
namespace utility {

Tuple Ray::position(const double time) const {
  return this->origin_ + this->direction_ * time;
}

Ray transform(const Ray &ray, const Matrix<4,4> &transformation){
  return Ray{transformation*ray.origin_, transformation*ray.direction_};
}

} // namespace utility
} // namespace raytracer