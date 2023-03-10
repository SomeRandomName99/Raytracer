#include "include/ray.h"

namespace Ray {
  Tuple position(const Ray &ray, const double &time){
    return ray.origin + ray.direction * time;
  }
}