#ifndef MATHS_H
#define MATHS_H

#include <concepts>
#include <cmath>

template <typename T> 
requires std::floating_point<T> 
extern bool floatEqual(T a, T b){
  // TODO: Investigate best practices and how to do things using machine 
  // machine epsilon
  const double epsilon = 1e-6;

  return (std::fabs(a-b) < epsilon);
}

#endif 