#ifndef RAY_H
#define RAY_H

#include "include/tuple.h"

namespace Ray{

  struct Ray {
    Tuple origin;
    Tuple direction;
  };

  Tuple position(const Ray &ray, const double &time);
}

#endif // RAY_H