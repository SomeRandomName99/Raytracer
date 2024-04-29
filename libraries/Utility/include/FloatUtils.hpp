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
concept floating = std::floating_point<T>;

template <floating T> 
bool floatNearlyEqual(T a, T b) noexcept{
  // TODO: Investigate best practices and how to do things using machine epsilon
  return (std::fabs(a-b) < EPSILON<T>);
}

template <floating T, floating U> 
bool floatNearlyEqual(T a, U b) noexcept{
  return floatNearlyEqual(static_cast<float>(a), static_cast<float>(b));
}

template <typename T> 
requires std::floating_point<T> 
T sqrt(T x) noexcept {
  if (floatNearlyEqual(x, static_cast<T>(0.0f))) { return 0.0f; }
  return std::sqrt(x);
}

} // namespace utility
} // namespace raytracer

#endif 