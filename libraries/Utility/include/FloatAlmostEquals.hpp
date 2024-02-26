#ifndef MATHS_H
#define MATHS_H

#include <concepts>
#include <cmath>

namespace raytracer {
namespace utility {

template <typename T> 
requires std::floating_point<T> 
const T EPSILON = 1e-6;

template <typename T> 
requires std::floating_point<T> 
extern bool floatNearlyEqual(T a, T b) noexcept{
  // TODO: Investigate best practices and how to do things using machine epsilon
  return (std::fabs(a-b) < EPSILON<T>);
}

} // namespace utility
} // namespace raytracer

#endif 