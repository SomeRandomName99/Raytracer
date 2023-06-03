#include "Ray.hpp"

namespace Ray {
  Tuple position(const Ray &ray, const double time){
    return ray.origin + ray.direction * time;
  }

  Ray transform(const Ray &ray, const Matrix<4,4> &transformation){
    return Ray{transformation*ray.origin, transformation*ray.direction};
  }
}