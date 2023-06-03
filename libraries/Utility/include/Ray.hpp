#ifndef RAY_H
#define RAY_H

#include "Tuple.hpp"
#include "Matrix.hpp"

namespace Ray{

  struct Ray {
    Tuple origin;
    Tuple direction;
  };

  Tuple position(const Ray &ray, const double time);
  Ray   transform(const Ray &ray, const Matrix<4,4> &transformation);
}

#endif // RAY_H