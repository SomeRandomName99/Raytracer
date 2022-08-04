#include <limits>
#include <cmath>

#include "include/maths.h"

template <typename T> 
requires std::floating_point<T> 
bool floatEqual(T a, T b){
  const auto epsilon = std::numeric_limits<T>::epsilon();

  return (std::fabs(a-b) < epsilon);
}

template bool floatEqual<float>(float, float);
template bool floatEqual<double>(double, double);