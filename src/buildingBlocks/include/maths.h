#ifndef MATHS_H
#define MATHS_H

#include <concepts>

template <typename T> 
requires std::floating_point<T> 
extern bool floatEqual(T a, T b);

#endif 