#ifndef RAY_H
#define RAY_H

#include "Tuple.hpp"
#include "Matrix.hpp"

namespace raytracer {
namespace utility {

namespace Ray{

  struct Ray {
    Tuple origin;
    Tuple direction;
  };

  Tuple position(const Ray &ray, const double time);
  Ray   transform(const Ray &ray, const Matrix<4,4> &transformation);
}

} // namespace utility
} // namespace raytracer

#endif // RAY_H