#ifndef MATHS_H
#define MATHS_H

#include <concepts>
#include <cmath>

namespace raytracer {
namespace utility {

template <typename T>
concept floating = std::floating_point<T>;

template <floating T> 
const T EPSILON = 1e-4;

template <floating T> 
bool floatNearlyEqual(T a, T b) noexcept{
  return (std::fabs(a-b) <= (EPSILON<T> * std::max(std::fabs(a), std::fabs(b))));
}

template <floating T, floating U> 
bool floatNearlyEqual(T a, U b) noexcept{
  return floatNearlyEqual(static_cast<double>(a), static_cast<double>(b));
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