#ifndef RAY_H
#define RAY_H

#include "Tuple.hpp"
#include "Matrix.hpp"

namespace raytracer {
namespace utility {

class Ray {
public: 
  Ray(Tuple origin, Tuple direction)
      : origin_{origin}, direction_{direction} {}

  Tuple position(const double time) const;

  Tuple origin_;
  Tuple direction_;
};


Ray transform(const Ray &ray, const Matrix<4,4> &transformation);

} // namespace utility
} // namespace raytracer

#endif // RAY_H