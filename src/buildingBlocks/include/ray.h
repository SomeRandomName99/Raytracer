#ifndef RAY_H
#define RAY_H

#include "include/tuple.h"
#include "include/matrix.h"

namespace Ray{

  struct Ray {
    Tuple origin;
    Tuple direction;
  };

  Tuple position(const Ray &ray, const double time);
  Ray   transform(const Ray &ray, const Matrix<4,4> &transform);
}

#endif // RAY_H