#ifndef MATHS_H
#define MATHS_H

#include <concepts>
#include <cmath>

namespace raytracer {
namespace utility {

template <typename T> 
requires std::floating_point<T> 
extern bool floatNearlyEqual(T a, T b) noexcept{
  // TODO: Investigate best practices and how to do things using machine epsilon
  const T epsilon = 1e-6;

  return (std::fabs(a-b) < epsilon);
}

} // namespace utility
} // namespace raytracer

#endif 